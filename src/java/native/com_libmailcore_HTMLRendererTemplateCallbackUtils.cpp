#include "com_libmailcore_HTMLRendererTemplateCallbackUtils.h"

#include "MCBaseTypes.h"
#include "JavaHandle.h"
#include "TypesUtils.h"
#include "MCHTMLRendererCallback.h"

using namespace mailcore;

static HTMLRendererTemplateCallback * callback = NULL;

JNIEXPORT jboolean JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_canPreviewPart
  (JNIEnv * env, jclass cls, jobject part)
{
    return (jboolean) callback->canPreviewPart(MC_FROM_JAVA(AbstractPart, part));
}

JNIEXPORT jboolean JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_shouldShowPart
  (JNIEnv * env, jclass cls, jobject part)
{
    return (jboolean) callback->canPreviewPart(MC_FROM_JAVA(AbstractPart, part));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateValuesForHeader
  (JNIEnv * env, jclass cls, jobject header)
{
    return MC_TO_JAVA(callback->templateValuesForHeader(MC_FROM_JAVA(MessageHeader, header)));
}

JNIEXPORT jobject JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateValuesForPart
  (JNIEnv * env, jclass cls, jobject part)
{
    return MC_TO_JAVA(callback->templateValuesForPart(MC_FROM_JAVA(AbstractPart, part)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForMainHeader
  (JNIEnv * env, jclass cls, jobject header)
{
    return (jstring) MC_TO_JAVA(callback->templateForMainHeader(MC_FROM_JAVA(MessageHeader, header)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForImage
  (JNIEnv * env, jclass cls, jobject part)
{
    return (jstring) MC_TO_JAVA(callback->templateForImage(MC_FROM_JAVA(AbstractPart, part)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForAttachment
  (JNIEnv * env, jclass cls, jobject part)
{
    return (jstring) MC_TO_JAVA(callback->templateForAttachment(MC_FROM_JAVA(AbstractPart, part)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForMessage
  (JNIEnv * env, jclass cls, jobject msg)
{
    return (jstring) MC_TO_JAVA(callback->templateForMessage(MC_FROM_JAVA(AbstractMessage, msg)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForEmbeddedMessage
  (JNIEnv * env, jclass cls, jobject msgPart)
{
    return (jstring) MC_TO_JAVA(callback->templateForEmbeddedMessage(MC_FROM_JAVA(AbstractMessagePart, msgPart)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForEmbeddedMessageHeader
  (JNIEnv * env, jclass cls, jobject header)
{
    return (jstring) MC_TO_JAVA(callback->templateForEmbeddedMessageHeader(MC_FROM_JAVA(MessageHeader, header)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_templateForAttachmentSeparator
  (JNIEnv * env, jclass cls)
{
    return (jstring) MC_TO_JAVA(callback->templateForAttachmentSeparator());
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_cleanHTML
  (JNIEnv * env, jclass cls, jstring html)
{
    return (jstring) MC_TO_JAVA(callback->cleanHTMLForPart(MC_FROM_JAVA(String, html)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_filterHTMLForPart
  (JNIEnv * env, jclass cls, jstring html)
{
    return (jstring) MC_TO_JAVA(callback->filterHTMLForPart(MC_FROM_JAVA(String, html)));
}

JNIEXPORT jstring JNICALL Java_com_libmailcore_HTMLRendererTemplateCallbackUtils_filterHTMLForMessage
  (JNIEnv * env, jclass cls, jstring html)
{
    return (jstring) MC_TO_JAVA(callback->filterHTMLForMessage(MC_FROM_JAVA(String, html)));
}

INITIALIZE(Java_com_libmailcore_HTMLRendererTemplateCallbackUtils)
{
    callback = new HTMLRendererTemplateCallback();
}
