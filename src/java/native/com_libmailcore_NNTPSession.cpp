#include "com_libmailcore_NNTPSession.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCNNTPAsyncSession.h"
#include "JavaOperationQueueCallback.h"
#include "JavaConnectionLogger.h"

using namespace mailcore;

#define nativeType NNTPAsyncSession
#define javaType NNTPSession

MC_JAVA_SYNTHESIZE_STRING(setHostname, hostname)
MC_JAVA_SYNTHESIZE_SCALAR(jint, unsigned int, setPort, port)
MC_JAVA_SYNTHESIZE_STRING(setUsername, username)
MC_JAVA_SYNTHESIZE_STRING(setPassword, password)
MC_JAVA_SYNTHESIZE_SCALAR(jint, ConnectionType, setConnectionType, connectionType)
MC_JAVA_SYNTHESIZE_SCALAR(jlong, time_t, setTimeout, timeout)
MC_JAVA_SYNTHESIZE_SCALAR(jboolean, bool, setCheckCertificateEnabled, isCheckCertificateEnabled)

JNIEXPORT jboolean JNICALL Java_com_libmailcore_NNTPSession_isOperationQueueRunning
    (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jboolean, isOperationQueueRunning);
}

JNIEXPORT void JNICALL Java_com_libmailcore_NNTPSession_cancelAllOperations
    (JNIEnv * env, jobject obj)
{
    MC_JAVA_NATIVE_INSTANCE->cancelAllOperations();
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchAllArticlesOperation
    (JNIEnv * env, jobject obj, jstring group)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchAllArticlesOperation(MC_FROM_JAVA(String, group)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchHeaderOperation
  (JNIEnv * env, jobject obj, jstring group, jint idx)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchHeaderOperation(MC_FROM_JAVA(String, group), (unsigned int) idx));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchArticleOperation
  (JNIEnv * env, jobject obj, jstring group, jint idx)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchArticleOperation(MC_FROM_JAVA(String, group), (unsigned int) idx));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchArticleByMessageIDOperation
  (JNIEnv * env, jobject obj, jstring group, jstring messageID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchArticleByMessageIDOperation(MC_FROM_JAVA(String, group), MC_FROM_JAVA(String, messageID)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchOverviewOperationWithIndexes
  (JNIEnv * env, jobject obj, jstring group, jobject indexes)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchOverviewOperationWithIndexes(MC_FROM_JAVA(String, group), MC_FROM_JAVA(IndexSet, indexes)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_fetchServerDateOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchServerDateOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_listAllNewsgroupsOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->listDefaultNewsgroupsOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_listDefaultNewsgroupsOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->listDefaultNewsgroupsOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_disconnectOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->disconnectOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPSession_checkAccountOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->checkAccountOperation());
}

JNIEXPORT void JNICALL Java_com_libmailcore_NNTPSession_finalizeNative
  (JNIEnv * env, jobject obj)
{
    JavaOperationQueueCallback * callback = (JavaOperationQueueCallback *) MC_JAVA_NATIVE_INSTANCE->operationQueueCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setOperationQueueCallback(NULL);

    JavaConnectionLogger * logger = (JavaConnectionLogger *) MC_JAVA_NATIVE_INSTANCE->connectionLogger();
    MC_SAFE_RELEASE(logger);
    MC_JAVA_NATIVE_INSTANCE->setConnectionLogger(NULL);
}

JNIEXPORT void JNICALL Java_com_libmailcore_NNTPSession_setupNativeOperationQueueListener
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

JNIEXPORT void JNICALL Java_com_libmailcore_NNTPSession_setupNativeConnectionLogger
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
