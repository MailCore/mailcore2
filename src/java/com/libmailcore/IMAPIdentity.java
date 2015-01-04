package com.libmailcore;

import java.util.List;

public class IMAPIdentity extends NativeObject {
    public IMAPIdentity()
    {
        setupNative();
    }

    public native void setVendor(String vendor);
    public native String vendor();
    
    public native void setName(String name);
    public native String name();
    
    public native void setVersion(String version);
    public native String version();

    public native void removeAllInfos();
    
    public native List<String> allInfoKeys();
    public native String infoForKey(String key);
    public native void setInfoForKey(String key, String value);
    
    private native void setupNative();
}
