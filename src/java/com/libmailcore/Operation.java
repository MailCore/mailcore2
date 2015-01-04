package com.libmailcore;

public class Operation extends NativeObject {
    public native void cancel();
    public native boolean isCancelled();
    
    public void start(OperationCallback aCallback)
    {
        callback = aCallback;
        nativeStart();
    }
    
    private native void nativeStart();
    
    protected void callCallback()
    {
        if (callback != null) {
            callback.succeeded();
        }
    }
    
    protected OperationCallback callback;
}
