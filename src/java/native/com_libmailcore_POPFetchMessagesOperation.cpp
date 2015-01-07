#include "com_libmailcore_POPFetchMessagesOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCPOPFetchMessagesOperation.h"

using namespace mailcore;

#define nativeType POPFetchMessagesOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_POPFetchMessagesOperation_messages
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(messages);
}

MC_JAVA_BRIDGE
