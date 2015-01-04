#include "com_libmailcore_NativeObject.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"

using namespace mailcore;

JNIEXPORT void JNICALL Java_com_libmailcore_NativeObject_initWithNative
  (JNIEnv * env, jobject obj, jlong nativeHandle)
{
    setHandle(env, obj, (void *) nativeHandle);
    Object * mcObject = (Object *) nativeHandle;
    MC_SAFE_RETAIN(mcObject);
}

JNIEXPORT void JNICALL Java_com_libmailcore_NativeObject_unsetupNative
  (JNIEnv * env, jobject obj)
{
    Object * mcObj = (Object *) getHandle(env, obj);
    MC_SAFE_RELEASE(mcObj);
    setHandle(env, obj, NULL);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_NativeObject_toString
  (JNIEnv * env, jobject obj)
{
    Object * mcObj = (Object *) getHandle(env, obj);
    if (mcObj == NULL) {
        return (jstring) MC_TO_JAVA(MCSTR("<Uninitialized NativeObject>"));
    }
    else {
        return (jstring) MC_TO_JAVA(mcObj->description());
    }
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NativeObject_clone
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_FROM_JAVA(Object, obj)->copy()->autorelease());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NativeObject_serializable
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_FROM_JAVA(Object, obj)->serializable());
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NativeObject_objectWithSerializable
  (JNIEnv * env, jclass cls, jobject serializable)
{
    return MC_TO_JAVA(Object::objectWithSerializable(MC_FROM_JAVA(HashMap, serializable)));
}
