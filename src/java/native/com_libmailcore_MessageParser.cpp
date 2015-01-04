#include "com_libmailcore_MessageParser.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCMessageParser.h"
#include "MCDefines.h"
#include "JavaHTMLRendererTemplateCallback.h"

using namespace mailcore;

#define nativeType MessageParser
#define javaType nativeType

JNIEXPORT jobject JNICALL Java_com_libmailcore_MessageParser_messageParserWithData
  (JNIEnv * env, jclass cls, jbyteArray data)
{
    return MC_TO_JAVA(MessageParser::messageParserWithData(MC_FROM_JAVA(Data, data)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_MessageParser_messageParserWithContentsOfFile
  (JNIEnv * env, jclass cls, jstring filename)
{
    return MC_TO_JAVA(MessageParser::messageParserWithContentsOfFile(MC_FROM_JAVA(String, filename)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_MessageParser_mainPart
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET(mainPart);
}

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_MessageParser_data
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_DATA(data);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageParser_htmlRendering
  (JNIEnv * env, jobject obj, jobject javaCallback)
{
    JavaHTMLRendererTemplateCallback * callback = NULL;
    if (javaCallback != NULL) {
        callback = new JavaHTMLRendererTemplateCallback(env, javaCallback);
    }
    jstring result = (jstring) MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->htmlRendering(callback));
    if (callback != NULL) {
        delete callback;
    }
    return result;
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageParser_htmlBodyRendering
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_STRING(htmlBodyRendering);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageParser_plainTextRendering
  (JNIEnv * env, jobject obj)
{
    return MC_JAVA_BRIDGE_GET_STRING(plainTextRendering);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageParser_plainTextBodyRendering
  (JNIEnv * env, jobject obj, jboolean stripWhitespace)
{
    return (jstring) MC_TO_JAVA(MC_JAVA_NATIVE_INSTANCE->plainTextBodyRendering((bool) stripWhitespace));
}

MC_JAVA_BRIDGE
