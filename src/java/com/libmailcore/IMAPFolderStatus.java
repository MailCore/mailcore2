package com.libmailcore;

public class IMAPFolderStatus extends NativeObject {
    public IMAPFolderStatus()
    {
        setupNative();
    }
    
    public native void setUnseenCount(long unseen);
    public native long unseenCount();
    
    public native void setMessageCount(long messages);
    public native long messageCount();
    
    public native void setRecentCount(long recent);
    public native long recentCount();
    
    public native void setUidNext(long uidNext);
    public native long uidNext();
    
    public native void setUidValidity(long uidValidity);
    public native long uidValidity();
    
    public native void setHighestModSeqValue(long highestModSeqValue);
    public native long highestModSeqValue();
    
    private native void setupNative();
}
