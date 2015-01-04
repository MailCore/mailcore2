#include "com_libmailcore_IMAPFolderInfoOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPFolderInfoOperation.h"

using namespace mailcore;

#define nativeType IMAPFolderInfoOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPFolderInfoOperation_info
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(info);
}

MC_JAVA_BRIDGE
