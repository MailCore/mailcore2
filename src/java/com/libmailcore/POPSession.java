package com.libmailcore;

public class POPSession extends NativeObject {
    public POPSession()
    {
        setupNative();
    }
    
    protected void finalize() throws Throwable
    {
        finalizeNative();
        super.finalize();
    }
    
    public native void setHostname(String hostname);
    public native String hostname();
    
    public native void setPort(int port);
    public native int port();
    
    public native void setUsername(String username);
    public native String username();

    public native void setPassword(String password);
    public native String password();

    public native void setAuthType(int authType);
    public native int authType();
    
    public native void setConnectionType(int connectionType);
    public native int connectionType();
    
    public native void setTimeout(long seconds);
    public native long timeout();
    
    public native void setCheckCertificateEnabled(boolean enabled);
    public native boolean isCheckCertificateEnabled();

    public void setConnectionLogger(ConnectionLogger logger)
    {
        connectionLogger = logger;
        setupNativeConnectionLogger();
    }
    
    public ConnectionLogger connectionLogger()
    {
        return connectionLogger;
    }

    public void setOperationQueueListener(OperationQueueListener listener)
    {
        operationQueueListener = listener;
        setupNativeOperationQueueListener();
    }
    
    public OperationQueueListener operationQueueListener()
    {
        return operationQueueListener;
    }
    
    public native boolean isOperationQueueRunning();
    public native void cancelAllOperations();

    public native POPFetchMessagesOperation fetchMessagesOperation();
    public native POPFetchHeaderOperation fetchHeaderOperation(int index);
    public native POPFetchMessageOperation fetchMessageOperation(int index);
    public native POPOperation deleteMessagesOperation(IndexSet indexes);
    public native POPOperation disconnectOperation();
    public native POPOperation checkAccountOperation();
    public native POPOperation noopOperation();

    private native void setupNative();
    private native void finalizeNative();
    
    private ConnectionLogger connectionLogger;
    private OperationQueueListener operationQueueListener;
    
    private native void setupNativeOperationQueueListener();
    private native void setupNativeConnectionLogger();
}
