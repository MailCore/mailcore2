#include "com_libmailcore_NNTPFetchHeaderOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCNNTPFetchHeaderOperation.h"

using namespace mailcore;

#define nativeType NNTPFetchHeaderOperation
#define javaType nativeType


JNIEXPORT jobject JNICALL Java_com_libmailcore_NNTPFetchHeaderOperation_header
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(header);
}

MC_JAVA_BRIDGE
