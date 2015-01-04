#include "com_libmailcore_MessageBuilder.h"

#include "TypesUtils.h"
#include "JavaHandle.h"
#include "MCMessageBuilder.h"
#include "MCDefines.h"
#include "JavaHTMLRendererTemplateCallback.h"

using namespace mailcore;

#define nativeType MessageBuilder
#define javaType nativeType

MC_JAVA_SYNTHESIZE_STRING(setHTMLBody, htmlBody)
MC_JAVA_SYNTHESIZE_STRING(setTextBody, textBody)
MC_JAVA_SYNTHESIZE(Array, setAttachments, attachments)

JNIEXPORT void JNICALL Java_com_libmailcore_MessageBuilder_addAttachment
    (JNIEnv * env, jobject obj, jobject attachment)
{
    MC_JAVA_NATIVE_INSTANCE->addAttachment(MC_FROM_JAVA(Attachment, attachment));
}

MC_JAVA_SYNTHESIZE(Array, setRelatedAttachments, relatedAttachments)

JNIEXPORT void JNICALL Java_com_libmailcore_MessageBuilder_addRelatedAttachment
    (JNIEnv * env, jobject obj, jobject attachment)
{
    MC_JAVA_NATIVE_INSTANCE->addRelatedAttachment(MC_FROM_JAVA(Attachment, attachment));
}

MC_JAVA_SYNTHESIZE_STRING(setBoundaryPrefix, boundaryPrefix)

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_MessageBuilder_data
    (JNIEnv * env, jobject obj)
{
    return (jbyteArray) MC_JAVA_BRIDGE_GET(data);
}

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_MessageBuilder_dataForEncryption
    (JNIEnv * env, jobject obj)
{
    return (jbyteArray) MC_JAVA_BRIDGE_GET(dataForEncryption);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageBuilder_htmlRendering
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

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageBuilder_htmlBodyRendering
    (JNIEnv * env, jobject obj)
{
    return (jstring) MC_JAVA_BRIDGE_GET(htmlBodyRendering);
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageBuilder_plainTextRendering
    (JNIEnv * env, jobject obj)
{
    return (jstring) MC_JAVA_NATIVE_INSTANCE->plainTextRendering();
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_MessageBuilder_plainTextBodyRendering
    (JNIEnv * env, jobject obj, jboolean stripWhitespace)
{
    return (jstring) MC_JAVA_NATIVE_INSTANCE->plainTextBodyRendering((bool) stripWhitespace);
}

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_MessageBuilder_openPGPSignedMessageDataWithSignatureData
    (JNIEnv * env, jobject obj, jbyteArray signature)
{
    return (jbyteArray) MC_JAVA_NATIVE_INSTANCE->openPGPSignedMessageDataWithSignatureData(MC_FROM_JAVA(Data, signature));
}

JNIEXPORT jbyteArray JNICALL Java_com_libmailcore_MessageBuilder_openPGPEncryptedMessageDataWithEncryptedData
    (JNIEnv * env, jobject obj, jbyteArray encryptedData)
{
    return (jbyteArray) MC_JAVA_NATIVE_INSTANCE->openPGPEncryptedMessageDataWithEncryptedData(MC_FROM_JAVA(Data, encryptedData));
}

MC_JAVA_BRIDGE
