#include "com_libmailcore_AbstractPart.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCAbstractPart.h"

using namespace mailcore;

#define nativeType AbstractPart
#define javaType nativeType

MC_JAVA_SYNTHESIZE_SCALAR(jint, PartType, setPartType, partType)
MC_JAVA_SYNTHESIZE_STRING(setFilename, filename)
MC_JAVA_SYNTHESIZE_STRING(setCharset, charset)
MC_JAVA_SYNTHESIZE_STRING(setUniqueID, uniqueID)
MC_JAVA_SYNTHESIZE_STRING(setContentID, contentID)
MC_JAVA_SYNTHESIZE_STRING(setContentLocation, contentLocation)
MC_JAVA_SYNTHESIZE_STRING(setContentDescription, contentDescription)
MC_JAVA_SYNTHESIZE_SCALAR(jboolean, bool, setInlineAttachment, isInlineAttachment)

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractPart_partForContentID
  (JNIEnv * env, jobject obj, jstring contentID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->partForContentID(MC_FROM_JAVA(String, contentID)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractPart_partForUniqueID
  (JNIEnv * env, jobject obj, jstring uniqueID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->partForUniqueID(MC_FROM_JAVA(String, uniqueID)));
}

JNIEXPORT void JNICALL Java_com_libmailcore_AbstractPart_setContentTypeParameter
  (JNIEnv * env, jobject obj, jstring name, jstring value)
{
    MC_JAVA_NATIVE_INSTANCE->setContentTypeParameter(MC_FROM_JAVA(String, name), MC_FROM_JAVA(String, value));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_AbstractPart_contentTypeParameterValueForName
  (JNIEnv * env, jobject obj, jstring name)
{
    return (jstring) MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->contentTypeParameterValueForName(MC_FROM_JAVA(String, name)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractPart_allContentTypeParametersNames
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(allContentTypeParametersNames);
}
