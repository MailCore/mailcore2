package com.libmailcore;

import java.util.Map;

public interface HTMLRendererTemplateCallback {
    boolean canPreviewPart(AbstractPart part);
    boolean shouldShowPart(AbstractPart part);
    Map<String, String> templateValuesForHeader(MessageHeader header);
    Map<String, String> templateValuesForPart(AbstractPart part);
    String templateForMainHeader(MessageHeader header);
    String templateForImage(AbstractPart part);
    String templateForAttachment(AbstractPart part);
    String templateForMessage(AbstractMessage message);
    String templateForEmbeddedMessage(AbstractMessagePart messagePart);
    String templateForEmbeddedMessageHeader(MessageHeader header);
    String templateForAttachmentSeparator();
    String cleanHTML(String html);
    String filterHTMLForPart(String html);
    String filterHTMLForMessage(String html);
}
