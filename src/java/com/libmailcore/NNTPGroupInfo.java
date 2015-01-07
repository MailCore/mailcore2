package com.libmailcore;

public class NNTPGroupInfo extends NativeObject {
    public NNTPGroupInfo()
    {
        setupNative();
    }
    
    public native void setName(String name);
    public native String name();
    
    public native void setMessageCount(long count);
    public native long messageCount();
    
    private native void setupNative();
}
