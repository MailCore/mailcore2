package com.libmailcore;

public class NNTPSession extends NativeObject {
    public NNTPSession()
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

    public native NNTPFetchAllArticlesOperation fetchAllArticlesOperation(String group);
    public native NNTPFetchHeaderOperation fetchHeaderOperation(String group, int idx);
    public native NNTPFetchArticleOperation fetchArticleOperation(String group, int idx);
    public native NNTPFetchArticleOperation fetchArticleByMessageIDOperation(String group, String messageID);
    public native NNTPFetchOverviewOperation fetchOverviewOperationWithIndexes(String group, IndexSet indexes);
    public native NNTPFetchServerTimeOperation fetchServerDateOperation();
    public native NNTPListNewsgroupsOperation listAllNewsgroupsOperation();
    public native NNTPListNewsgroupsOperation listDefaultNewsgroupsOperation();
    public native NNTPOperation disconnectOperation();
    public native NNTPOperation checkAccountOperation();
    
    private native void setupNative();
    private native void finalizeNative();
    
    private ConnectionLogger connectionLogger;
    private OperationQueueListener operationQueueListener;
    
    private native void setupNativeOperationQueueListener();
    private native void setupNativeConnectionLogger();
}
