package com.libmailcore;

import java.util.Map;

/** Default implementations for HTMLRendererTemplateCallback */
public class HTMLRendererTemplateCallbackUtils {
    native static boolean canPreviewPart(AbstractPart part);
    native static boolean shouldShowPart(AbstractPart part);
    native static Map<String, String> templateValuesForHeader(MessageHeader header);
    native static Map<String, String> templateValuesForPart(AbstractPart part);
    native static String templateForMainHeader(MessageHeader header);
    native static String templateForImage(AbstractPart part);
    native static String templateForAttachment(AbstractPart part);
    native static String templateForMessage(AbstractMessage message);
    native static String templateForEmbeddedMessage(AbstractMessagePart messagePart);
    native static String templateForEmbeddedMessageHeader(MessageHeader header);
    native static String templateForAttachmentSeparator();
    native static String cleanHTML(String html);
    native static String filterHTMLForPart(String html);
    native static String filterHTMLForMessage(String html);
}
