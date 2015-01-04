#include "com_libmailcore_IMAPQuotaOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPQuotaOperation.h"

using namespace mailcore;

#define nativeType IMAPQuotaOperation
#define javaType nativeType

JNIEXPORT jint JNICALL Java_com_libmailcore_IMAPQuotaOperation_usage
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, usage);
}

JNIEXPORT jint JNICALL Java_com_libmailcore_IMAPQuotaOperation_limit
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, limit);
}

MC_JAVA_BRIDGE
