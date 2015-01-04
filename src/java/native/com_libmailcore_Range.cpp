#include "com_libmailcore_Range.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCRange.h"
#include "MCDefines.h"

using namespace mailcore;

JNIEXPORT jobject JNICALL Java_com_libmailcore_Range_removeRange
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, obj);
    Range otherRange = rangeFromJava(env, range);
    return MC_TO_JAVA(RangeRemoveRange(mcRange, otherRange));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_Range_union
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, obj);
    Range otherRange = rangeFromJava(env, range);
    return MC_TO_JAVA(RangeUnion(mcRange, otherRange));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_Range_intersection
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, obj);
    Range otherRange = rangeFromJava(env, range);
    return rangeToJava(env, RangeIntersection(mcRange, otherRange));
}

JNIEXPORT jboolean JNICALL Java_com_libmailcore_Range_hasIntersection
  (JNIEnv * env, jobject obj, jobject range)
{
    Range mcRange = rangeFromJava(env, obj);
    Range otherRange = rangeFromJava(env, range);
    return (jboolean) RangeHasIntersection(mcRange, otherRange);
}

JNIEXPORT jlong JNICALL Java_com_libmailcore_Range_leftBound
  (JNIEnv * env, jobject obj)
{
    Range mcRange = rangeFromJava(env, obj);
    return (jlong) RangeLeftBound(mcRange);
}

JNIEXPORT jlong JNICALL Java_com_libmailcore_Range_rightBound
  (JNIEnv * env, jobject obj)
{
    Range mcRange = rangeFromJava(env, obj);
    return (jlong) RangeRightBound(mcRange);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_Range_toString
  (JNIEnv * env, jobject obj)
{
    return (jstring) MC_TO_JAVA(RangeToString(rangeFromJava(env, obj)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_Range_rangeWithString
  (JNIEnv * env, jclass cls, jstring rangeString)
{
    Range mcRange = RangeFromString(MC_FROM_JAVA(String, rangeString));
    return rangeToJava(env, mcRange);
}
