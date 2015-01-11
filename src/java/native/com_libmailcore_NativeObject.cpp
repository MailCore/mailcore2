#include "com_libmailcore_NativeObject.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"

#define nativeType Object

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
        return MC_JAVA_BRIDGE_GET_STRING(description);
    }
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_NativeObject_clone
  (JNIEnv * env, jobject obj)
{
    return MC_TO_JAVA(MC_FROM_JAVA(Object, obj)->copy()->autorelease());
}

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_NativeObject_serializableData
  (JNIEnv * env, jobject obj)
{
    return (jbyteArray) MC_TO_JAVA(JSON::objectToJSONData(MC_JAVA_NATIVE_INSTANCE->serializable()));
}

JNIEXPORT void JNICALL Java_com_libmailcore_NativeObject_importSerializableData
  (JNIEnv * env, jobject obj, jbyteArray data)
{
    MC_JAVA_NATIVE_INSTANCE->importSerializable((HashMap *) JSON::objectFromJSONData(MC_FROM_JAVA(Data, data)));
}
