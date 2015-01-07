#include "com_libmailcore_SMTPOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCSMTPOperation.h"

using namespace mailcore;

#define nativeType SMTPOperation
#define javaType nativeType

JNIEXPORT jint JNICALL Java_com_libmailcore_SMTPOperation_errorCode
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, error);
}

MC_JAVA_BRIDGE
