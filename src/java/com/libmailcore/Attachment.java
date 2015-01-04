package com.libmailcore;

public class Attachment extends AbstractPart {
    public static native String mimeTypeForFilename(String filename);
    
    public native static Attachment attachmentWithContentsOfFile(String filename);
    public native static Attachment attachmentWithData(String filename, byte[] data);
    public native static Attachment attachmentWithHTMLString(String htmlString);
    public native static Attachment attachmentWithRFC822Message(byte[] messageData);
    public native static Attachment attachmentWithText(String text);
    
    public native byte[] data();
    public native void setData(byte[] data);
    
    public native String decodedString();
}