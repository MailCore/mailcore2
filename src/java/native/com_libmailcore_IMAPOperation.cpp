#include "com_libmailcore_IMAPOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPOperation.h"

using namespace mailcore;

#define nativeType IMAPOperation
#define javaType nativeType

JNIEXPORT jint JNICALL Java_com_libmailcore_IMAPOperation_errorCode
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, error);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPOperation_errorMessage
  (JNIEnv * env, jobject obj)
{
    return (jstring) MC_TO_JAVA(MCSTR("Operation failed"));
}

MC_JAVA_BRIDGE
