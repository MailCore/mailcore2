#include <MailCore/MailCore.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <map>
#include <string>

using namespace mailcore;

static std::string trim(const std::string & value)
{
    size_t begin = 0;
    size_t end = value.size();

    while (begin < end && isspace((unsigned char) value[begin]))
        begin++;
    while (end > begin && isspace((unsigned char) value[end - 1]))
        end--;
    return value.substr(begin, end - begin);
}

static std::map<std::string, std::string> loadState(const char * path)
{
    std::map<std::string, std::string> result;
    std::ifstream input(path);
    std::string line;

    while (std::getline(input, line)) {
        size_t equal = line.find('=');
        if (equal == std::string::npos)
            continue;
        result[trim(line.substr(0, equal))] = trim(line.substr(equal + 1));
    }
    return result;
}

static bool saveState(const char * path, const std::map<std::string, std::string> & state)
{
    std::ofstream output(path);
    if (!output)
        return false;

    std::map<std::string, std::string>::const_iterator cur;
    for (cur = state.begin(); cur != state.end(); ++cur)
        output << cur->first << "=" << cur->second << "\n";
    return output.good();
}

static const char * stateValue(const std::map<std::string, std::string> & state,
    const char * key, const char * fallback)
{
    std::map<std::string, std::string>::const_iterator cur = state.find(key);
    if (cur == state.end() || cur->second.empty())
        return fallback;
    return cur->second.c_str();
}

static String * mcString(const char * value)
{
    if (value == NULL)
        return NULL;
    return String::stringWithUTF8Characters(value);
}

static unsigned int arrayCount(Array * array)
{
    return array != NULL ? array->count() : 0;
}

static bool stringEquals(String * value, const char * expected)
{
    return value != NULL && expected != NULL && strcmp(value->UTF8Characters(), expected) == 0;
}

static String * findInboxID(Array * folders)
{
    if (folders == NULL)
        return NULL;

    for (unsigned int i = 0; i < folders->count(); i++) {
        ActiveSyncFolder * folder = (ActiveSyncFolder *) folders->objectAtIndex(i);
        if (stringEquals(folder->displayName(), "Inbox"))
            return folder->serverID();
    }
    return NULL;
}

static int fail(const char * step, ErrorCode error)
{
    fprintf(stderr, "%s failed: error=%d\n", step, (int) error);
    return 1;
}

static int failStatus(const char * step, int status)
{
    fprintf(stderr, "%s failed: status=%d\n", step, status);
    return 1;
}

static void usage(const char * progname)
{
    fprintf(stderr,
        "usage: %s --server URL --login USER --oauth-token TOKEN --state-file PATH\n",
        progname);
}

int main(int argc, char ** argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    const char * server = NULL;
    const char * login = NULL;
    const char * oauthToken = NULL;
    const char * stateFile = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--server") == 0 && i + 1 < argc)
            server = argv[++i];
        else if (strcmp(argv[i], "--login") == 0 && i + 1 < argc)
            login = argv[++i];
        else if (strcmp(argv[i], "--oauth-token") == 0 && i + 1 < argc)
            oauthToken = argv[++i];
        else if (strcmp(argv[i], "--state-file") == 0 && i + 1 < argc)
            stateFile = argv[++i];
        else {
            usage(argv[0]);
            return 2;
        }
    }

    if (server == NULL || login == NULL || oauthToken == NULL || stateFile == NULL) {
        usage(argv[0]);
        return 2;
    }

    AutoreleasePool * pool = new AutoreleasePool();
    std::map<std::string, std::string> state = loadState(stateFile);
    ErrorCode error = ErrorNone;
    ActiveSyncSession * session = new ActiveSyncSession();

    session->setServerURL(mcString(server));
    session->setUsername(mcString(login));
    session->setOAuth2Token(mcString(oauthToken));
    session->setDeviceID(MCSTR("mailcore2activesynclive001"));

    printf("MailCore ActiveSync server=%s\n", server);
    printf("MailCore ActiveSync login=%s\n", login);
    printf("MailCore ActiveSync state_file=%s\n", stateFile);

    session->loginOAuth2(&error);
    if (error != ErrorNone)
        return fail("loginOAuth2", error);
    printf("loginOAuth2 ok\n");

    printf("requesting options\n");
    ActiveSyncOptions * options = session->options(&error);
    if (error != ErrorNone || options == NULL)
        return fail("options", error);
    printf("options ok protocol_versions=%u commands=%u\n",
        arrayCount(options->protocolVersions()), arrayCount(options->commands()));

    const char * folderSyncKey = stateValue(state, "folder_sync_key", "0");
    printf("requesting folderSync sync_key=%s\n", folderSyncKey);
    ActiveSyncFolderSyncResult * folderSync = session->folderSync(mcString(folderSyncKey), &error);
    if (error != ErrorNone || folderSync == NULL)
        return fail("folderSync", error);
    if (folderSync->status() != ActiveSyncFolderSyncStatusSuccess)
        return failStatus("folderSync", folderSync->status());

    if (folderSync->syncKey() != NULL)
        state["folder_sync_key"] = folderSync->syncKey()->UTF8Characters();

    String * inboxID = findInboxID(folderSync->added());
    if (inboxID == NULL)
        inboxID = findInboxID(folderSync->updated());
    if (inboxID != NULL)
        state["folder.Inbox"] = inboxID->UTF8Characters();

    if (state.find("folder.Inbox") == state.end()) {
        printf("requesting folderSync reset sync_key=0\n");
        folderSync = session->folderSync(MCSTR("0"), &error);
        if (error != ErrorNone || folderSync == NULL)
            return fail("folderSync reset", error);
        if (folderSync->status() != ActiveSyncFolderSyncStatusSuccess)
            return failStatus("folderSync reset", folderSync->status());
        if (folderSync->syncKey() != NULL)
            state["folder_sync_key"] = folderSync->syncKey()->UTF8Characters();
        inboxID = findInboxID(folderSync->added());
        if (inboxID == NULL)
            inboxID = findInboxID(folderSync->updated());
        if (inboxID != NULL)
            state["folder.Inbox"] = inboxID->UTF8Characters();
    }

    printf("folderSync ok sync_key=%s added=%u updated=%u deleted=%u inbox=%s\n",
        stateValue(state, "folder_sync_key", ""),
        arrayCount(folderSync->added()), arrayCount(folderSync->updated()),
        arrayCount(folderSync->deleted()), stateValue(state, "folder.Inbox", ""));

    if (state.find("folder.Inbox") == state.end()) {
        fprintf(stderr, "Inbox was not found in FolderSync results.\n");
        return 1;
    }

    const char * inboxSyncKey = stateValue(state, "sync.Inbox", "0");
    if (strcmp(inboxSyncKey, "0") == 0 || inboxSyncKey[0] == '\0') {
        ActiveSyncSyncRequest * initialRequest = new ActiveSyncSyncRequest();
        initialRequest->setCollectionID(mcString(state["folder.Inbox"].c_str()));
        initialRequest->setSyncKey(MCSTR("0"));
        initialRequest->setCollectionClass(MCSTR("Email"));
        initialRequest->setGetChanges(false);

        printf("requesting initial sync sync_key=0\n");
        ActiveSyncSyncResult * initialSync = session->sync(initialRequest, &error);
        initialRequest->release();
        if (error != ErrorNone || initialSync == NULL)
            return fail("initial sync", error);
        if (initialSync->status() != ActiveSyncSyncStatusSuccess)
            return failStatus("initial sync", initialSync->status());
        if (initialSync->syncKey() == NULL) {
            fprintf(stderr, "Initial sync did not return an Inbox sync key.\n");
            return 1;
        }
        state["sync.Inbox"] = initialSync->syncKey()->UTF8Characters();
        inboxSyncKey = state["sync.Inbox"].c_str();
        printf("initial sync ok sync_key=%s\n", inboxSyncKey);

        if (!saveState(stateFile, state)) {
            fprintf(stderr, "Could not save state file %s.\n", stateFile);
            return 1;
        }
    }

    printf("requesting itemEstimate sync_key=%s\n", inboxSyncKey);
    ActiveSyncItemEstimateResult * estimate = session->itemEstimate(
        mcString(state["folder.Inbox"].c_str()), mcString(inboxSyncKey), &error);
    if (error != ErrorNone || estimate == NULL)
        return fail("itemEstimate", error);
    printf("itemEstimate ok status=%d collection_status=%d estimate=%u\n",
        (int) estimate->status(), (int) estimate->collectionStatus(),
        estimate->estimate());

    unsigned int totalAdded = 0;
    unsigned int totalChanged = 0;
    unsigned int totalDeleted = 0;
    bool moreAvailable = false;
    int pageCount = 0;

    do {
        ActiveSyncSyncRequest * syncRequest = new ActiveSyncSyncRequest();
        inboxSyncKey = stateValue(state, "sync.Inbox", "0");
        syncRequest->setCollectionID(mcString(state["folder.Inbox"].c_str()));
        syncRequest->setSyncKey(mcString(inboxSyncKey));
        syncRequest->setCollectionClass(MCSTR("Email"));
        syncRequest->setGetChanges(true);
        syncRequest->setDeletesAsMoves(false);
        syncRequest->setWindowSize(5);
        syncRequest->setBodyPreference(ActiveSyncBodyTypePlainText, 0);

        printf("requesting sync page=%d sync_key=%s\n", pageCount + 1, inboxSyncKey);
        ActiveSyncSyncResult * syncResult = session->sync(syncRequest, &error);
        syncRequest->release();
        if (error != ErrorNone || syncResult == NULL)
            return fail("sync", error);
        if (syncResult->status() != ActiveSyncSyncStatusSuccess)
            return failStatus("sync", syncResult->status());
        if (syncResult->syncKeyFromResponse() && syncResult->syncKey() != NULL)
            state["sync.Inbox"] = syncResult->syncKey()->UTF8Characters();

        totalAdded += arrayCount(syncResult->added());
        totalChanged += arrayCount(syncResult->changed());
        totalDeleted += arrayCount(syncResult->deleted());
        moreAvailable = syncResult->moreAvailable();
        pageCount++;

        printf("sync page ok sync_key=%s more_available=%d added=%u changed=%u deleted=%u\n",
            stateValue(state, "sync.Inbox", ""), moreAvailable ? 1 : 0,
            arrayCount(syncResult->added()), arrayCount(syncResult->changed()),
            arrayCount(syncResult->deleted()));
    } while (moreAvailable && pageCount < 20);

    printf("sync ok pages=%d sync_key=%s more_available=%d added=%u changed=%u deleted=%u\n",
        pageCount, stateValue(state, "sync.Inbox", ""), moreAvailable ? 1 : 0,
        totalAdded, totalChanged, totalDeleted);

    if (!saveState(stateFile, state)) {
        fprintf(stderr, "Could not save state file %s.\n", stateFile);
        return 1;
    }

    session->release();
    pool->release();
    return 0;
}
