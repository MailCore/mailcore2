#include "com_libmailcore_IMAPMessageRenderingOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPMessageRenderingOperation.h"

using namespace mailcore;

#define nativeType IMAPMessageRenderingOperation
#define javaType nativeType

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPMessageRenderingOperation_result
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_STRING(result);
}

MC_JAVA_BRIDGE
