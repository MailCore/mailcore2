#include "com_libmailcore_IMAPAppendMessageOperation.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPAppendMessageOperation.h"

using namespace mailcore;

#define nativeType IMAPAppendMessageOperation
#define javaType nativeType

JNIEXPORT void JNICALL Java_com_libmailcore_IMAPAppendMessageOperation_setDate
  (JNIEnv * env, jobject obj, jobject date)
{
    MC_JAVA_NATIVE_INSTANCE->setDate(javaDateToTime(env, date));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPAppendMessageOperation_date
  (JNIEnv * env, jobject obj)
{
    return timeToJavaDate(env, MC_JAVA_NATIVE_INSTANCE->date());
}

JNIEXPORT jlong JNICALL Java_com_libmailcore_IMAPAppendMessageOperation_createdUID
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jlong, createdUID);
}

MC_JAVA_BRIDGE
