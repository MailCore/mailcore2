package com.libmailcore;

public class MessageParser extends AbstractMessage {
    public static native MessageParser messageParserWithData(byte[] messageData);
    public static native MessageParser messageParserWithContentsOfFile(String filename);
    
    public native AbstractPart mainPart();
    public native byte[] data();
    
    public native String htmlRendering(HTMLRendererTemplateCallback callback);
    public String htmlRendering()
    {
        return htmlRendering(null);
    }
    public native String htmlBodyRendering();
    public native String plainTextRendering();
    public native String plainTextBodyRendering(boolean stripWhitespace);
    
    private native void setupNative(byte[] messageData);
}
