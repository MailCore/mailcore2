#include "com_libmailcore_IMAPFolderStatusOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPFolderStatusOperation.h"

using namespace mailcore;

#define nativeType IMAPFolderStatusOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPFolderStatusOperation_status
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(status);
}

MC_JAVA_BRIDGE
