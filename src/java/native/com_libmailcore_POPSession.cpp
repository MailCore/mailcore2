#include "com_libmailcore_POPSession.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCPOPAsyncSession.h"
#include "JavaOperationQueueCallback.h"
#include "JavaConnectionLogger.h"

using namespace mailcore;

#define nativeType POPAsyncSession
#define javaType POPSession

MC_JAVA_SYNTHESIZE_STRING(setHostname, hostname)
MC_JAVA_SYNTHESIZE_SCALAR(jint, unsigned int, setPort, port)
MC_JAVA_SYNTHESIZE_STRING(setUsername, username)
MC_JAVA_SYNTHESIZE_STRING(setPassword, password)
MC_JAVA_SYNTHESIZE_SCALAR(jint, AuthType, setAuthType, authType)
MC_JAVA_SYNTHESIZE_SCALAR(jint, ConnectionType, setConnectionType, connectionType)
MC_JAVA_SYNTHESIZE_SCALAR(jlong, time_t, setTimeout, timeout)
MC_JAVA_SYNTHESIZE_SCALAR(jboolean, bool, setCheckCertificateEnabled, isCheckCertificateEnabled)

JNIEXPORT jboolean JNICALL Java_com_libmailcore_POPSession_isOperationQueueRunning
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jboolean, isOperationQueueRunning);
}

JNIEXPORT void JNICALL Java_com_libmailcore_POPSession_cancelAllOperations
  (JNIEnv * env, jobject obj)
{
    MC_JAVA_NATIVE_INSTANCE->cancelAllOperations();
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_fetchMessagesOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessagesOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_fetchHeaderOperation
  (JNIEnv * env, jobject obj, jint idx)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchHeaderOperation((unsigned int) idx));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_fetchMessageOperation
  (JNIEnv * env, jobject obj, jint idx)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessageOperation((unsigned int) idx));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_deleteMessagesOperation
  (JNIEnv * env, jobject obj, jobject indexes)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->deleteMessagesOperation(MC_FROM_JAVA(IndexSet, indexes)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_disconnectOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->disconnectOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_checkAccountOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->checkAccountOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPSession_noopOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->noopOperation());
}

JNIEXPORT void JNICALL Java_com_libmailcore_POPSession_finalizeNative
  (JNIEnv * env, jobject obj)
{
    JavaOperationQueueCallback * callback = (JavaOperationQueueCallback *) MC_JAVA_NATIVE_INSTANCE->operationQueueCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setOperationQueueCallback(NULL);

    JavaConnectionLogger * logger = (JavaConnectionLogger *) MC_JAVA_NATIVE_INSTANCE->connectionLogger();
    MC_SAFE_RELEASE(logger);
    MC_JAVA_NATIVE_INSTANCE->setConnectionLogger(NULL);
}

JNIEXPORT void JNICALL Java_com_libmailcore_POPSession_setupNativeOperationQueueListener
  (JNIEnv * env, jobject obj)
{
    JavaOperationQueueCallback * callback = (JavaOperationQueueCallback *) MC_JAVA_NATIVE_INSTANCE->operationQueueCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setOperationQueueCallback(NULL);

    jobject javaListener = getObjectField(env, obj, "operationQueueListener");
    if (javaListener != NULL) {
        callback = new JavaOperationQueueCallback(env, javaListener);
        MC_JAVA_NATIVE_INSTANCE->setOperationQueueCallback(callback);
    }
}

JNIEXPORT void JNICALL Java_com_libmailcore_POPSession_setupNativeConnectionLogger
  (JNIEnv * env, jobject obj)
{
    JavaConnectionLogger * logger = (JavaConnectionLogger *) MC_JAVA_NATIVE_INSTANCE->connectionLogger();
    MC_SAFE_RELEASE(logger);
    MC_JAVA_NATIVE_INSTANCE->setConnectionLogger(NULL);

    jobject javaLogger = getObjectField(env, obj, "connectionLogger");
    if (javaLogger != NULL) {
        logger = new JavaConnectionLogger(env, javaLogger);
        MC_JAVA_NATIVE_INSTANCE->setConnectionLogger(logger);
    }
}

MC_JAVA_BRIDGE
