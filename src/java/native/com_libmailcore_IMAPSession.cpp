#include "com_libmailcore_IMAPSession.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPAsyncSession.h"
#include "JavaOperationQueueCallback.h"
#include "JavaConnectionLogger.h"

using namespace mailcore;

#define nativeType IMAPAsyncSession
#define javaType IMAPSession

MC_JAVA_SYNTHESIZE_STRING(setHostname, hostname)
MC_JAVA_SYNTHESIZE_SCALAR(jint, unsigned int, setPort, port)
MC_JAVA_SYNTHESIZE_STRING(setUsername, username)
MC_JAVA_SYNTHESIZE_STRING(setPassword, password)
MC_JAVA_SYNTHESIZE_STRING(setOAuth2Token, OAuth2Token)
MC_JAVA_SYNTHESIZE_SCALAR(jint, AuthType, setAuthType, authType)
MC_JAVA_SYNTHESIZE_SCALAR(jint, ConnectionType, setConnectionType, connectionType)
MC_JAVA_SYNTHESIZE_SCALAR(jlong, time_t, setTimeout, timeout)
MC_JAVA_SYNTHESIZE_SCALAR(jboolean, bool, setCheckCertificateEnabled, isCheckCertificateEnabled)
MC_JAVA_SYNTHESIZE(IMAPNamespace, setDefaultNamespace, defaultNamespace)
MC_JAVA_SYNTHESIZE_SCALAR(jboolean, bool, setAllowsFolderConcurrentAccessEnabled, allowsFolderConcurrentAccessEnabled)
MC_JAVA_SYNTHESIZE_SCALAR(jint, unsigned int, setMaximumConnections, maximumConnections)

JNIEXPORT jboolean JNICALL Java_com_libmailcore_IMAPSession_isOperationQueueRunning
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jboolean, isOperationQueueRunning);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPSession_cancelAllOperations
  (JNIEnv * env, jobject obj)
{
    MC_JAVA_NATIVE_INSTANCE->cancelAllOperations();
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_serverIdentity
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(serverIdentity);
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_clientIdentity
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(clientIdentity);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPSession_gmailUserDisplayName
  (JNIEnv * env, jobject obj)
{
    return (jstring) MC_JAVA_BRIDGE_GET(gmailUserDisplayName);
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_folderInfoOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->folderInfoOperation(MC_FROM_JAVA(String, path)));
}

/*
 * Class:     com_libmailcore_IMAPSession
 * Method:    folderStatusOperation
 * Signature: (Ljava/lang/String;)Lcom/libmailcore/IMAPFolderStatusOperation;
 */
JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_folderStatusOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->folderInfoOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchSubscribedFoldersOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchSubscribedFoldersOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchAllFoldersOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchAllFoldersOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_renameFolderOperation
  (JNIEnv * env, jobject obj, jstring currentName, jstring otherName)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->renameFolderOperation(MC_FROM_JAVA(String, currentName), MC_FROM_JAVA(String, otherName)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_deleteFolderOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->deleteFolderOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_createFolderOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->createFolderOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_subscribeFolderOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->subscribeFolderOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_unsubscribeFolderOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->unsubscribeFolderOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_appendMessageOperation
  (JNIEnv * env, jobject obj, jstring path, jbyteArray data, jint flags, jobject customFlags)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->appendMessageOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(Data, data), (MessageFlag) flags, MC_FROM_JAVA(Array, customFlags)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_copyMessagesOperation
  (JNIEnv * env, jobject obj, jstring sourcePath, jobject uids, jstring destPath)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->copyMessagesOperation(MC_FROM_JAVA(String, sourcePath),
        MC_FROM_JAVA(IndexSet, uids), MC_FROM_JAVA(String, destPath)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_expungeOperation
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->expungeOperation(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessagesByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jint kind, jobject uids)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessagesByUIDOperation(MC_FROM_JAVA(String, path), (IMAPMessagesRequestKind) kind, MC_FROM_JAVA(IndexSet, uids)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessagesByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jint kind, jobject numbers)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessagesByNumberOperation(MC_FROM_JAVA(String, path), (IMAPMessagesRequestKind) kind, MC_FROM_JAVA(IndexSet, numbers)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_syncMessagesByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jint kind, jobject uids, jlong modSeq)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->syncMessagesByUIDOperation(MC_FROM_JAVA(String, path), (IMAPMessagesRequestKind) kind, MC_FROM_JAVA(IndexSet, uids), (uint64_t) modSeq));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessageByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jlong uid, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessageByUIDOperation(MC_FROM_JAVA(String, path), (uint32_t) uid, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessageAttachmentByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jlong uid, jstring partID, jint encoding, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessageAttachmentByUIDOperation(MC_FROM_JAVA(String, path),
        (uint32_t) uid, MC_FROM_JAVA(String, partID), (Encoding) encoding, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessageByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jlong number, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessageByNumberOperation(MC_FROM_JAVA(String, path),
        (uint32_t) number, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchMessageAttachmentByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jlong number, jstring partID, jint encoding, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchMessageAttachmentByNumberOperation(MC_FROM_JAVA(String, path),
        (uint32_t) number, MC_FROM_JAVA(String, partID), (Encoding) encoding, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchParsedMessageByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jlong uid, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchParsedMessageByUIDOperation(MC_FROM_JAVA(String, path),
        (uint32_t) uid, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchParsedMessageByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jlong number, jboolean urgent)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchParsedMessageByNumberOperation(MC_FROM_JAVA(String, path),
        (uint32_t) number, (bool) urgent));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_storeFlagsByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jobject uids, jint kind, jint flags, jobject customFlags)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->storeFlagsByUIDOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(IndexSet, uids), (IMAPStoreFlagsRequestKind) kind, (MessageFlag) flags, MC_FROM_JAVA(Array, customFlags)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_storeFlagsByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jobject uids, jint kind, jint flags, jobject customFlags)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->storeFlagsByUIDOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(IndexSet, uids), (IMAPStoreFlagsRequestKind) kind, (MessageFlag) flags, MC_FROM_JAVA(Array, customFlags)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_storeLabelsByUIDOperation
  (JNIEnv * env, jobject obj, jstring path, jobject uids, jint kind, jobject labels)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->storeLabelsByUIDOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(IndexSet, uids), (IMAPStoreFlagsRequestKind) kind, MC_FROM_JAVA(Array, labels)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_storeLabelsByNumberOperation
  (JNIEnv * env, jobject obj, jstring path, jobject numbers, jint kind, jobject labels)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->storeLabelsByNumberOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(IndexSet, numbers), (IMAPStoreFlagsRequestKind) kind, MC_FROM_JAVA(Array, labels)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_searchOperation__Ljava_lang_String_2ILjava_lang_String_2
  (JNIEnv * env, jobject obj, jstring path, jint kind, jstring searchString)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->searchOperation(MC_FROM_JAVA(String, path),
        (IMAPSearchKind) kind, MC_FROM_JAVA(String, searchString)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_searchOperation__Ljava_lang_String_2Lcom_libmailcore_IMAPSearchExpression_2
  (JNIEnv * env, jobject obj, jstring path, jobject expression)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->searchOperation(MC_FROM_JAVA(String, path),
        MC_FROM_JAVA(IMAPSearchExpression, expression)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_idleOperation
  (JNIEnv * env, jobject obj, jstring path, jlong lastKnownUID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->idleOperation(MC_FROM_JAVA(String, path), (uint32_t) lastKnownUID));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_fetchNamespaceOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->fetchNamespaceOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_identityOperation
  (JNIEnv * env, jobject obj, jobject identity)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->identityOperation(MC_FROM_JAVA(IMAPIdentity, identity)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_connectOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->connectOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_checkAccountOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->checkAccountOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_disconnectOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->disconnectOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_capabilityOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->capabilityOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_quotaOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->quotaOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_noopOperation
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->noopOperation());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_htmlRenderingOperation
  (JNIEnv * env, jobject obj, jobject msg, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->htmlRenderingOperation(MC_FROM_JAVA(IMAPMessage, msg), MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_htmlBodyRenderingOperation
  (JNIEnv * env, jobject obj, jobject msg, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->htmlBodyRenderingOperation(MC_FROM_JAVA(IMAPMessage, msg), MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_plainTextRenderingOperation
  (JNIEnv * env, jobject obj, jobject msg, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->plainTextRenderingOperation(MC_FROM_JAVA(IMAPMessage, msg), MC_FROM_JAVA(String, path)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSession_plainTextBodyRenderingOperation
  (JNIEnv * env, jobject obj, jobject msg, jstring path, jboolean stripWhitespace)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->plainTextBodyRenderingOperation(MC_FROM_JAVA(IMAPMessage, msg), MC_FROM_JAVA(String, path), (bool) stripWhitespace));
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPSession_finalizeNative
  (JNIEnv * env, jobject obj)
{
    JavaOperationQueueCallback * callback = (JavaOperationQueueCallback *) MC_JAVA_NATIVE_INSTANCE->operationQueueCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setOperationQueueCallback(NULL);

    JavaConnectionLogger * logger = (JavaConnectionLogger *) MC_JAVA_NATIVE_INSTANCE->connectionLogger();
    MC_SAFE_RELEASE(logger);
    MC_JAVA_NATIVE_INSTANCE->setConnectionLogger(NULL);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPSession_setupNativeOperationQueueListener
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

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPSession_setupNativeConnectionLogger
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
