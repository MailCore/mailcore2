package com.libmailcore;

public class IMAPFolder extends NativeObject {
    public IMAPFolder()
    {
        setupNative();
    }
    
    public native void setPath(String path);
    public native String path();
    
    public native void setDelimiter(char delimiter);
    public native char delimiter();
    
    public native void setFlags(int flags);
    public native int flags();
    
    private native void setupNative();
}
