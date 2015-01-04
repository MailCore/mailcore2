#include "com_libmailcore_IMAPSearchOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPSearchOperation.h"

using namespace mailcore;

#define nativeType IMAPSearchOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPSearchOperation_uids
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(uids);
}

MC_JAVA_BRIDGE
