#include "com_libmailcore_IMAPFetchParsedContentOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPFetchParsedContentOperation.h"
#include "JavaIMAPOperationCallback.h"

using namespace mailcore;

#define nativeType IMAPFetchParsedContentOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPFetchParsedContentOperation_parser
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(parser);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPFetchParsedContentOperation_finalizeNative
  (JNIEnv * env, jobject obj)
{
    JavaIMAPOperationCallback * callback = (JavaIMAPOperationCallback *) MC_JAVA_NATIVE_INSTANCE->imapCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setImapCallback(NULL);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPFetchParsedContentOperation_setupNativeOperationProgressListener
  (JNIEnv * env, jobject obj)
{
    JavaIMAPOperationCallback * callback = (JavaIMAPOperationCallback *) MC_JAVA_NATIVE_INSTANCE->imapCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setImapCallback(NULL);

    jobject javaListener = getObjectField(env, obj, "listener");
    if (javaListener != NULL) {
        callback = new JavaIMAPOperationCallback(env, javaListener);
        MC_JAVA_NATIVE_INSTANCE->setImapCallback(callback);
    }
}

MC_JAVA_BRIDGE
