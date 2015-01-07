#include "com_libmailcore_POPOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCPOPOperation.h"

using namespace mailcore;

#define nativeType POPOperation
#define javaType nativeType

JNIEXPORT jint JNICALL Java_com_libmailcore_POPOperation_errorCode
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, error);
}

MC_JAVA_BRIDGE
