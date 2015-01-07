#include "com_libmailcore_NNTPFetchServerTimeOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCNNTPFetchServerTimeOperation.h"

using namespace mailcore;

#define nativeType NNTPFetchServerTimeOperation
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPFetchServerTimeOperation_time
  (JNIEnv * env, jobject obj)
{
    return timeToJavaDate(env, MC_JAVA_NATIVE_INSTANCE->time());
}

MC_JAVA_BRIDGE
