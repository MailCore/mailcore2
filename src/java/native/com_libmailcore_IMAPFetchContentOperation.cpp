#include "com_libmailcore_IMAPFetchContentOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPFetchContentOperation.h"
#include "JavaIMAPOperationCallback.h"

using namespace mailcore;

#define nativeType IMAPFetchContentOperation
#define javaType nativeType

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_IMAPFetchContentOperation_data
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_DATA(data);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPFetchContentOperation_finalizeNative
  (JNIEnv * env, jobject obj)
{
    JavaIMAPOperationCallback * callback = (JavaIMAPOperationCallback *) MC_JAVA_NATIVE_INSTANCE->imapCallback();
    MC_SAFE_RELEASE(callback);
    MC_JAVA_NATIVE_INSTANCE->setImapCallback(NULL);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPFetchContentOperation_setupNativeOperationProgressListener
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
