package com.libmailcore;

import java.util.AbstractList;

public class AbstractPart extends NativeObject {
    public native int partType();
    public native void setPartType(int partType);
    
    public native String filename();
    public native void setFilename(String filename);
    
    public native String charset();
    public native void setCharset(String charset);
    
    public native String uniqueID();
    public native void setUniqueID(String uniqueID);
    
    public native String contentID();
    public native void setContentID(String contentID);
    
    public native String contentLocation();
    public native void setContentLocation(String contentLocation);
    
    public native String contentDescription();
    public native void setContentDescription(String contentDescription);
    
    public native boolean isInlineAttachment();
    public native void setInlineAttachment(boolean inlineAttachment);
    
    public native AbstractPart partForContentID(String contentID);
    public native AbstractPart partForUniqueID(String uniqueID);
    
    public native void setContentTypeParameter(String name, String value);
    public native String contentTypeParameterValueForName(String name);
    public native AbstractList<String> allContentTypeParametersNames();
}
