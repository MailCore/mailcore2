package com.libmailcore;

import java.util.List;

public class SMTPSession extends NativeObject {
    public SMTPSession()
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

    public native void setOAuth2Token(String token);
    public native String OAuth2Token();
    
    public native void setAuthType(int authType);
    public native int authType();
    
    public native void setConnectionType(int connectionType);
    public native int connectionType();
    
    public native void setTimeout(long seconds);
    public native long timeout();
    
    public native void setCheckCertificateEnabled(boolean enabled);
    public native boolean isCheckCertificateEnabled();

    public native void setUseHeloIPEnabled(boolean enabled);
    public native boolean useHeloIPEnabled();

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

    public native SMTPOperation loginOperation();
    public native SMTPOperation sendMessageOperation(byte[] messageData);
    public native SMTPOperation sendMessageOperation(Address from, List<Address> recipients, byte[] messageData);
    public native SMTPOperation checkAccountOperation(Address from);
    public native SMTPOperation noopOperation();

    private native void setupNative();
    private native void finalizeNative();
    
    private ConnectionLogger connectionLogger;
    private OperationQueueListener operationQueueListener;
    
    private native void setupNativeOperationQueueListener();
    private native void setupNativeConnectionLogger();
}
