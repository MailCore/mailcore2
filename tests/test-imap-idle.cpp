//
//  test-imap-idle.cpp
//  mailcore2
//

#include "test-all.h"

#if !defined(_MSC_VER) && !defined(ANDROID) && !defined(__ANDROID__)

#include <MailCore/MailCore.h>

#if __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif
#if __linux__
#include <glib.h>
#endif

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

using namespace mailcore;

static bool waitUntilAbsolute(pthread_cond_t * cond, pthread_mutex_t * lock, bool * value, int timeoutMS)
{
    struct timeval tv;
    struct timespec ts;
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + timeoutMS / 1000;
    ts.tv_nsec = (tv.tv_usec + (timeoutMS % 1000) * 1000) * 1000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec ++;
        ts.tv_nsec -= 1000000000;
    }

    while (!* value) {
        int r = pthread_cond_timedwait(cond, lock, &ts);
        if (r == ETIMEDOUT) {
            return false;
        }
    }

    return true;
}

class FakeIdleIMAPServer {
public:
    FakeIdleIMAPServer()
    {
        mListenFD = -1;
        mClientFD = -1;
        mPort = 0;
        mStarted = false;
        mStopRequested = false;
        mIdleEntered = false;
        mDoneReceived = false;
        mConnectionClosed = false;
        mServerError = false;
        pthread_mutex_init(&mLock, NULL);
        pthread_cond_init(&mCond, NULL);
    }

    ~FakeIdleIMAPServer()
    {
        stop();
        pthread_cond_destroy(&mCond);
        pthread_mutex_destroy(&mLock);
    }

    bool start()
    {
        mListenFD = socket(AF_INET, SOCK_STREAM, 0);
        if (mListenFD < 0) {
            markError();
            return false;
        }

        int enabled = 1;
        setsockopt(mListenFD, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        addr.sin_port = htons(0);

        if (bind(mListenFD, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            markError();
            return false;
        }

        if (listen(mListenFD, 1) < 0) {
            markError();
            return false;
        }

        socklen_t addrLen = sizeof(addr);
        if (getsockname(mListenFD, (struct sockaddr *) &addr, &addrLen) < 0) {
            markError();
            return false;
        }
        mPort = ntohs(addr.sin_port);

        if (pthread_create(&mThread, NULL, &FakeIdleIMAPServer::runThread, this) != 0) {
            markError();
            return false;
        }
        mStarted = true;

        return true;
    }

    void stop()
    {
        pthread_mutex_lock(&mLock);
        mStopRequested = true;
        pthread_cond_broadcast(&mCond);
        int listenFD = mListenFD;
        int clientFD = mClientFD;
        pthread_mutex_unlock(&mLock);

        if (listenFD >= 0) {
            shutdown(listenFD, SHUT_RDWR);
            close(listenFD);
        }
        if (clientFD >= 0) {
            shutdown(clientFD, SHUT_RDWR);
            close(clientFD);
        }

        if (mStarted) {
            pthread_join(mThread, NULL);
            mStarted = false;
        }

        pthread_mutex_lock(&mLock);
        mListenFD = -1;
        mClientFD = -1;
        pthread_mutex_unlock(&mLock);
    }

    unsigned int port()
    {
        return mPort;
    }

    bool waitForIdleEntered(int timeoutMS)
    {
        pthread_mutex_lock(&mLock);
        bool result = waitUntilAbsolute(&mCond, &mLock, &mIdleEntered, timeoutMS);
        pthread_mutex_unlock(&mLock);
        return result;
    }

    bool sawDoneOrClose()
    {
        pthread_mutex_lock(&mLock);
        bool result = mDoneReceived || mConnectionClosed;
        pthread_mutex_unlock(&mLock);
        return result;
    }

    bool hasError()
    {
        pthread_mutex_lock(&mLock);
        bool result = mServerError;
        pthread_mutex_unlock(&mLock);
        return result;
    }

private:
    static void * runThread(void * context)
    {
        ((FakeIdleIMAPServer *) context)->run();
        return NULL;
    }

    void markError()
    {
        pthread_mutex_lock(&mLock);
        mServerError = true;
        pthread_cond_broadcast(&mCond);
        pthread_mutex_unlock(&mLock);
    }

    void markClientAccepted(int fd)
    {
        pthread_mutex_lock(&mLock);
        mClientFD = fd;
        pthread_cond_broadcast(&mCond);
        pthread_mutex_unlock(&mLock);
    }

    void markIdleEntered()
    {
        pthread_mutex_lock(&mLock);
        mIdleEntered = true;
        pthread_cond_broadcast(&mCond);
        pthread_mutex_unlock(&mLock);
    }

    void markDoneReceived()
    {
        pthread_mutex_lock(&mLock);
        mDoneReceived = true;
        pthread_cond_broadcast(&mCond);
        pthread_mutex_unlock(&mLock);
    }

    void markConnectionClosed()
    {
        pthread_mutex_lock(&mLock);
        mConnectionClosed = true;
        pthread_cond_broadcast(&mCond);
        pthread_mutex_unlock(&mLock);
    }

    bool sendString(const char * str)
    {
        size_t len = strlen(str);
        const char * bytes = str;
        while (len > 0) {
            ssize_t written = send(mClientFD, bytes, len, 0);
            if (written <= 0) {
                return false;
            }
            bytes += written;
            len -= written;
        }
        return true;
    }

    bool readLine(std::string * line)
    {
        line->clear();
        while (true) {
            char ch;
            ssize_t count = recv(mClientFD, &ch, 1, 0);
            if (count == 0) {
                markConnectionClosed();
                return false;
            }
            if (count < 0) {
                if (errno == EINTR) {
                    continue;
                }
                markConnectionClosed();
                return false;
            }
            if (ch == '\n') {
                if (!line->empty() && (*line)[line->length() - 1] == '\r') {
                    line->resize(line->length() - 1);
                }
                return true;
            }
            line->push_back(ch);
        }
    }

    static std::string tagFromLine(const std::string & line)
    {
        size_t pos = line.find(' ');
        if (pos == std::string::npos) {
            return line;
        }
        return line.substr(0, pos);
    }

    static bool hasCommand(const std::string & line, const char * command)
    {
        std::string token = " ";
        token += command;
        return line.find(token) != std::string::npos;
    }

    bool respondTagged(const std::string & tag, const char * text)
    {
        std::string response = tag;
        response += text;
        response += "\r\n";
        return sendString(response.c_str());
    }

    void run()
    {
        int fd = accept(mListenFD, NULL, NULL);
        if (fd < 0) {
            pthread_mutex_lock(&mLock);
            bool stopping = mStopRequested;
            pthread_mutex_unlock(&mLock);
            if (!stopping) {
                markError();
            }
            return;
        }
        markClientAccepted(fd);

        if (!sendString("* OK fake imap ready\r\n")) {
            markConnectionClosed();
            return;
        }

        std::string idleTag;
        while (true) {
            std::string line;
            if (!readLine(&line)) {
                return;
            }

            if (line == "DONE") {
                markDoneReceived();
                if (!idleTag.empty()) {
                    respondTagged(idleTag, " OK IDLE done");
                }
                continue;
            }

            std::string tag = tagFromLine(line);
            if (hasCommand(line, "CAPABILITY")) {
                if (!sendString("* CAPABILITY IMAP4rev1 IDLE\r\n") ||
                    !respondTagged(tag, " OK CAPABILITY done")) {
                    markConnectionClosed();
                    return;
                }
            }
            else if (hasCommand(line, "LOGIN")) {
                if (!respondTagged(tag, " OK LOGIN done")) {
                    markConnectionClosed();
                    return;
                }
            }
            else if (hasCommand(line, "SELECT")) {
                if (!sendString("* FLAGS (\\Seen)\r\n") ||
                    !sendString("* 0 EXISTS\r\n") ||
                    !sendString("* 0 RECENT\r\n") ||
                    !sendString("* OK [UIDVALIDITY 1]\r\n") ||
                    !sendString("* OK [UIDNEXT 1]\r\n") ||
                    !sendString("* OK [PERMANENTFLAGS (\\Seen)]\r\n") ||
                    !respondTagged(tag, " OK [READ-WRITE] SELECT done")) {
                    markConnectionClosed();
                    return;
                }
            }
            else if (hasCommand(line, "IDLE")) {
                idleTag = tag;
                if (!sendString("+ idling\r\n")) {
                    markConnectionClosed();
                    return;
                }
                markIdleEntered();
            }
            else if (hasCommand(line, "NOOP")) {
                if (!respondTagged(tag, " OK NOOP done")) {
                    markConnectionClosed();
                    return;
                }
            }
            else if (hasCommand(line, "LOGOUT")) {
                sendString("* BYE logging out\r\n");
                respondTagged(tag, " OK LOGOUT done");
                markConnectionClosed();
                return;
            }
            else {
                if (!respondTagged(tag, " OK ignored")) {
                    markConnectionClosed();
                    return;
                }
            }
        }
    }

private:
    int mListenFD;
    int mClientFD;
    unsigned int mPort;
    pthread_t mThread;
    bool mStarted;
    bool mStopRequested;
    bool mIdleEntered;
    bool mDoneReceived;
    bool mConnectionClosed;
    bool mServerError;
    pthread_mutex_t mLock;
    pthread_cond_t mCond;
};

static void pumpCallbacksOnce()
{
#if __APPLE__
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.01, true);
#elif __linux__
    g_main_context_iteration(NULL, FALSE);
    usleep(10000);
#else
    usleep(10000);
#endif
}

static bool waitForQueueStopped(IMAPAsyncSession * session, int timeoutMS)
{
    struct timeval start;
    gettimeofday(&start, NULL);

    while (true) {
        pumpCallbacksOnce();
        if (!session->isOperationQueueRunning()) {
            return true;
        }

        struct timeval now;
        gettimeofday(&now, NULL);
        long elapsedMS = (long) ((now.tv_sec - start.tv_sec) * 1000 +
            (now.tv_usec - start.tv_usec) / 1000);
        if (elapsedMS >= timeoutMS) {
            return false;
        }
    }
}

static int idleStressIterations()
{
    const char * value = getenv("MAILCORE_IDLE_STRESS_ITERS");
    if (value == NULL) {
        return 50;
    }
    int result = atoi(value);
    if (result <= 0) {
        return 50;
    }
    return result;
}

static void configureSession(IMAPAsyncSession * session, unsigned int port)
{
    session->setHostname(MCSTR("127.0.0.1"));
    session->setPort(port);
    session->setUsername(MCSTR("user"));
    session->setPassword(MCSTR("password"));
    session->setConnectionType(ConnectionTypeClear);
    session->setTimeout(2);
    session->setMaximumConnections(1);
}

static void configureCoreSession(IMAPSession * session, unsigned int port)
{
    session->setHostname(MCSTR("127.0.0.1"));
    session->setPort(port);
    session->setUsername(MCSTR("user"));
    session->setPassword(MCSTR("password"));
    session->setConnectionType(ConnectionTypeClear);
    session->setTimeout(2);
}

static bool testCancelWakesIdleIteration(int iteration)
{
    FakeIdleIMAPServer server;
    if (!server.start()) {
        fprintf(stderr, "idle stress cancel: server start failed at iteration %i\n", iteration);
        return false;
    }

    AutoreleasePool * pool = new AutoreleasePool();
    IMAPAsyncSession * session = new IMAPAsyncSession();
    configureSession(session, server.port());

    IMAPIdleOperation * op = session->idleOperation(MCSTR("INBOX"), 0);
    op->start();

    bool idleEntered = false;
    struct timeval start;
    gettimeofday(&start, NULL);
    while (!idleEntered) {
        pumpCallbacksOnce();
        idleEntered = server.waitForIdleEntered(10);
        if (server.hasError()) {
            break;
        }
        struct timeval now;
        gettimeofday(&now, NULL);
        long elapsedMS = (long) ((now.tv_sec - start.tv_sec) * 1000 +
            (now.tv_usec - start.tv_usec) / 1000);
        if (elapsedMS >= 2000) {
            break;
        }
    }

    if (!idleEntered) {
        fprintf(stderr, "idle stress cancel: IDLE not entered at iteration %i\n", iteration);
        session->cancelAllOperations();
        waitForQueueStopped(session, 2000);
        session->release();
        pool->release();
        return false;
    }

    session->cancelAllOperations();
    bool stopped = waitForQueueStopped(session, 2000);
    bool sawDoneOrClose = server.sawDoneOrClose();

    session->release();
    pool->release();

    if (!stopped) {
        fprintf(stderr, "idle stress cancel: queue did not stop at iteration %i\n", iteration);
        return false;
    }
    if (!sawDoneOrClose) {
        fprintf(stderr, "idle stress cancel: server saw neither DONE nor close at iteration %i\n", iteration);
        return false;
    }

    return !server.hasError();
}

struct CoreIdleThreadContext {
    IMAPSession * session;
    ErrorCode error;
};

static void * coreIdleThread(void * context)
{
    CoreIdleThreadContext * ctx = (CoreIdleThreadContext *) context;
    ctx->session->idle(MCSTR("INBOX"), 0, &ctx->error);
    return NULL;
}

static bool testDisconnectDuringIdleIteration(int iteration)
{
    FakeIdleIMAPServer server;
    if (!server.start()) {
        fprintf(stderr, "idle stress disconnect: server start failed at iteration %i\n", iteration);
        return false;
    }

    AutoreleasePool * pool = new AutoreleasePool();
    IMAPSession * session = new IMAPSession();
    configureCoreSession(session, server.port());

    ErrorCode error;
    session->selectIfNeeded(MCSTR("INBOX"), &error);
    if (error != ErrorNone) {
        fprintf(stderr, "idle stress disconnect: select failed with %i at iteration %i\n", error, iteration);
        session->release();
        pool->release();
        return false;
    }
    if (!session->setupIdle()) {
        fprintf(stderr, "idle stress disconnect: setupIdle failed at iteration %i\n", iteration);
        session->release();
        pool->release();
        return false;
    }

    CoreIdleThreadContext context;
    context.session = session;
    context.error = ErrorNone;

    pthread_t thread;
    if (pthread_create(&thread, NULL, coreIdleThread, &context) != 0) {
        fprintf(stderr, "idle stress disconnect: pthread_create failed at iteration %i\n", iteration);
        session->unsetupIdle();
        session->release();
        pool->release();
        return false;
    }

    if (!server.waitForIdleEntered(2000)) {
        fprintf(stderr, "idle stress disconnect: IDLE not entered at iteration %i\n", iteration);
        session->interruptIdle();
        pthread_join(thread, NULL);
        session->unsetupIdle();
        session->release();
        pool->release();
        return false;
    }

    session->disconnect();
    pthread_join(thread, NULL);

    bool sawDoneOrClose = server.sawDoneOrClose();
    session->release();
    pool->release();

    if (!sawDoneOrClose) {
        fprintf(stderr, "idle stress disconnect: server saw neither DONE nor close at iteration %i\n", iteration);
        return false;
    }

    return !server.hasError();
}

void testIMAPIdleStress()
{
    if (getenv("MAILCORE_RUN_IMAP_IDLE_STRESS") == NULL) {
        return;
    }

    signal(SIGPIPE, SIG_IGN);

    int iterations = idleStressIterations();
    printf("testIMAPIdleStress: %i iterations\n", iterations);

    for (int i = 0 ; i < iterations ; i ++) {
        if (!testCancelWakesIdleIteration(i)) {
            MCAssert(0);
        }
        if (!testDisconnectDuringIdleIteration(i)) {
            MCAssert(0);
        }
    }

    printf("testIMAPIdleStress ok\n");
}

#else

void testIMAPIdleStress()
{
}

#endif
