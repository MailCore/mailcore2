package com.libmailcore;

import java.util.List;

public class IMAPFetchMessagesOperation extends IMAPOperation {
    protected void finalize() throws Throwable
    {
        finalizeNative();
        super.finalize();
    }
    
    public void setProgressListener(IMAPOperationItemProgressListener aListener) {
        listener = aListener;
        setupNativeOperationProgressListener();
    }
    
    public IMAPOperationItemProgressListener progressListener() {
        return listener;
    }
    
    public native List<IMAPMessage> messages();
    public native IndexSet vanishedMessages();
    
    private native void finalizeNative();
    
    private IMAPOperationItemProgressListener listener;
    private native void setupNativeOperationProgressListener();
    private long nativeOperationProgressListener;
}
