#include "com_libmailcore_AbstractMessage.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCAbstractMessage.h"
#include "MCAbstractPart.h"
#include "MCMessageHeader.h"

using namespace mailcore;

#define nativeType AbstractMessage
#define javaType nativeType

MC_JAVA_SYNTHESIZE(MessageHeader, setHeader, header)

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractMessage_partForContentID
  (JNIEnv * env, jobject obj, jstring contentID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->partForContentID(MC_FROM_JAVA(String, contentID)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractMessage_partForUniqueID
  (JNIEnv * env, jobject obj, jstring uniqueID)
{
    return MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->partForUniqueID(MC_FROM_JAVA(String, uniqueID)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractMessage_attachments
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(attachments);
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractMessage_htmlInlineAttachments
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(htmlInlineAttachments);
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_AbstractMessage_requiredPartsForRendering
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(requiredPartsForRendering);
}
