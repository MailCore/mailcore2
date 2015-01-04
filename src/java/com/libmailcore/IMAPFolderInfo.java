package com.libmailcore;

public class IMAPFolderInfo extends NativeObject {
    public IMAPFolderInfo()
    {
        setupNative();
    }
    
    public native long uidNext();
    public native void setUidNext(long uidNext);
    
    public native long uidValidity();
    public native void setUidValidity(long uidValidity);
    
    public native long modSequenceValue();
    public native void setModSequenceValue(long modSequenceValue);
    
    public native int messageCount();
    public native void setMessageCount(int messageCount);
    
    public native long firstUnseenUid();
    public native void setFirstUnseenUid(long firstUnseenUid);
    
    public native boolean allowsNewPermanentFlags();
    public native void setAllowsNewPermanentFlags(boolean allowsNewPermanentFlags);
        
    private native void setupNative();
}
