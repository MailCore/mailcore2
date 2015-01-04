package com.libmailcore;

import java.util.List;

public class AbstractMessage extends NativeObject {
    public native MessageHeader header();
    public native void setHeader(MessageHeader header);
    
    public native AbstractPart partForContentID(String contentID);
    public native AbstractPart partForUniqueID(String uniqueID);
    public native List<AbstractPart> attachments();
    public native List<AbstractPart> htmlInlineAttachments();
    public native List<AbstractPart> requiredPartsForRendering();
}
