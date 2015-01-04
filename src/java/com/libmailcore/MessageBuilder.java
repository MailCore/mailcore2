package com.libmailcore;

import java.util.List;

public class MessageBuilder extends AbstractMessage {
    public MessageBuilder()
    {
        setupNative();
    }

    public native void setHTMLBody(String htmlBody);
    public native String htmlBody();
    
    public native void setTextBody(String textBody);
    public native String textBody();
    
    public native void setAttachments(List<Attachment> attachments);
    public native List<AbstractPart> attachments();
    public native void addAttachment(Attachment attachment);
    
    public native void setRelatedAttachments(List<Attachment> attachments);
    public native List<Attachment> relatedAttachments();
    public native void addRelatedAttachment(Attachment attachment);
    
    public native void setBoundaryPrefix(String boundaryPrefix);
    public native String boundaryPrefix();
    
    public native byte[] data();
    public native byte[] dataForEncryption();
    
    public native String htmlRendering(HTMLRendererTemplateCallback callback);
    public native String htmlBodyRendering();
    
    public native String plainTextRendering();
    public native String plainTextBodyRendering(boolean stripWhitespace);
    
    public native byte[] openPGPSignedMessageDataWithSignatureData(byte[] signature);
    public native byte[] openPGPEncryptedMessageDataWithEncryptedData(byte[] encryptedData);
    
    private native void setupNative();
}
