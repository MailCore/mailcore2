#include "com_libmailcore_IndexSet.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCDefines.h"
#include "MCIndexSet.h"

using namespace mailcore;

#define nativeType IndexSet
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_IndexSet_indexSet
  (JNIEnv * env, jclass cls)
{
    return MC_TO_JAVA(IndexSet::indexSet());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IndexSet_indexSetWithRange
  (JNIEnv * env, jclass cls, jobject range)
{
    Range mcRange = rangeFromJava(env, range);
    return MC_TO_JAVA(IndexSet::indexSetWithRange(mcRange));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IndexSet_indexSetWithIndex
  (JNIEnv * env, jclass cls, jlong idx)
{
    return MC_TO_JAVA(IndexSet::indexSetWithIndex((uint64_t) idx));
}

JNIEXPORT jint JNICALL Java_com_libmailcore_IndexSet_count
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, count);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_addIndex
  (JNIEnv * env, jobject obj, jlong idx)
{
    MC_JAVA_NATIVE_INSTANCE->addIndex((uint64_t) idx);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_removeIndex
  (JNIEnv * env, jobject obj, jlong idx)
{
    MC_JAVA_NATIVE_INSTANCE->removeIndex((uint64_t) idx);
}

JNIEXPORT jboolean JNICALL Java_com_libmailcore_IndexSet_containsIndex
  (JNIEnv * env, jobject obj, jlong idx)
{
    return (jboolean) MC_JAVA_NATIVE_INSTANCE->containsIndex((uint64_t) idx);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_addRange
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, range);
    MC_JAVA_NATIVE_INSTANCE->addRange(mcRange);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_removeRange
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, range);
    MC_JAVA_NATIVE_INSTANCE->removeRange(mcRange);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_intersectsRange
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, range);
    MC_JAVA_NATIVE_INSTANCE->intersectsRange(mcRange);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_addIndexSet
  (JNIEnv * env, jobject obj, jobject otherIndexSet)
{
    MC_JAVA_NATIVE_INSTANCE->addIndexSet(MC_FROM_JAVA(IndexSet, otherIndexSet));
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_removeIndexSet
  (JNIEnv * env, jobject obj, jobject otherIndexSet)
{
    MC_JAVA_NATIVE_INSTANCE->removeIndexSet(MC_FROM_JAVA(IndexSet, otherIndexSet));
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_intersectsIndexSet
  (JNIEnv * env, jobject obj, jobject otherIndexSet)
{
    MC_JAVA_NATIVE_INSTANCE->intersectsIndexSet(MC_FROM_JAVA(IndexSet, otherIndexSet));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IndexSet_allRanges
  (JNIEnv * env, jobject obj)
{
    jclass cls = env->FindClass("java/util/Vector");
    jmethodID constructor = env->GetMethodID(cls, "<init>", "(I)V");
    unsigned int count = MC_JAVA_NATIVE_INSTANCE->rangesCount();
    jobject javaVector = env->NewObject(cls, constructor, count);
    jmethodID method = env->GetMethodID(cls, "add", "(Ljava.lang.Object;)Z");
    Range * ranges = MC_JAVA_NATIVE_INSTANCE->allRanges();
    for(unsigned int i = 0 ; i < count ; i ++) {
        jobject javaObject = rangeToJava(env, ranges[i]);
        env->CallBooleanMethod(javaVector, method, javaObject);
    }
    return javaVector;
}

JNIEXPORT jint JNICALL Java_com_libmailcore_IndexSet_rangesCount
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jint, rangesCount);
}

JNIEXPORT void JNICALL Java_com_libmailcore_IndexSet_removeAllIndexes
  (JNIEnv * env, jobject obj)
{
    MC_JAVA_NATIVE_INSTANCE->removeAllIndexes();
}

MC_JAVA_BRIDGE
