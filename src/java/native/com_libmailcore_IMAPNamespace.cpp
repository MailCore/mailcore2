#include "com_libmailcore_IMAPNamespace.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCIMAPNamespace.h"

using namespace mailcore;

#define nativeType IMAPNamespace
#define javaType nativeType

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPNamespace_mainPrefix
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_STRING(mainPrefix);
}

JNIEXPORT jchar JNICALL Java_com_libmailcore_IMAPNamespace_mainDelimiter
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_SCALAR(jchar, mainDelimiter);
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPNamespace_prefixes
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(prefixes);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPNamespace_pathForComponents
  (JNIEnv * env, jobject obj, jobject components)
{
    return (jstring) MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->pathForComponents(MC_FROM_JAVA(Array, components)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_IMAPNamespace_pathForComponentsAndPrefix
  (JNIEnv * env, jobject obj, jobject components, jstring prefix)
{
    return (jstring) MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->pathForComponentsAndPrefix(MC_FROM_JAVA(Array, components),
        MC_FROM_JAVA(String, prefix)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPNamespace_componentsFromPath
  (JNIEnv * env, jobject obj, jstring path)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->componentsFromPath(MC_FROM_JAVA(String, path)));
}

JNIEXPORT jboolean JNICALL Java_com_libmailcore_IMAPNamespace_containsFolderPath
  (JNIEnv * env, jobject obj, jstring path)
{
    return (jboolean) MC_JAVA_NATIVE_INSTANCE->containsFolderPath(MC_FROM_JAVA(String, path));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_IMAPNamespace_namespaceWithPrefix
  (JNIEnv * env, jclass cls, jstring prefix, jchar delimiter)
{
    return MC_TO_JAVA(IMAPNamespace::namespaceWithPrefix(MC_FROM_JAVA(String, prefix), (char) delimiter));
}

MC_JAVA_BRIDGE
