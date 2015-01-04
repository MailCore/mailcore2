#include "com_libmailcore_IMAPFetchFoldersOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPFetchFoldersOperation.h"

using namespace mailcore;

#define nativeType IMAPFetchFoldersOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPFetchFoldersOperation_folders
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(folders);
}

MC_JAVA_BRIDGE
