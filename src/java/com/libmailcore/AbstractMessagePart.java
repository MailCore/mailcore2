package com.libmailcore;

public class AbstractMessagePart extends AbstractPart {
    public native MessageHeader header();
    public native void setHeader(MessageHeader header);
    
    public native AbstractPart mainPart();    
    public native void setMainPart(AbstractPart part);
}
