package com.libmailcore;

public class IMAPFetchParsedContentOperation extends IMAPOperation {
    protected void finalize() throws Throwable
    {
        finalizeNative();
        super.finalize();
    }
    
    public void setProgressListener(IMAPOperationProgressListener aListener)
    {
        listener = aListener;
        setupNativeOperationProgressListener();
    }
    
    public IMAPOperationProgressListener progressListener()
    {
        return listener;
    }
    
    public native MessageParser parser();
    
    private native void finalizeNative();
    
    private IMAPOperationProgressListener listener;
    private native void setupNativeOperationProgressListener();
    private long nativeOperationProgressListener;
}
