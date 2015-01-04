package com.libmailcore;

public class IMAPFetchContentOperation extends IMAPOperation {
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
    
    public native byte[] data();
    
    private native void finalizeNative();
    
    private IMAPOperationProgressListener listener;
    private native void setupNativeOperationProgressListener();
    private long nativeOperationProgressListener;
}
