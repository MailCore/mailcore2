package com.libmailcore;

import java.util.List;

public class IMAPSession extends NativeObject {
    public IMAPSession()
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
    
    public native void setDefaultNamespace(IMAPNamespace ns);
    public native IMAPNamespace defaultNamespace();
    
    public native void setAllowsFolderConcurrentAccessEnabled(boolean enabled);
    public native boolean allowsFolderConcurrentAccessEnabled();
    
    public native void setMaximumConnections(int maxConnections);
    public native int maximumConnections();
    
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
    
    public native IMAPIdentity serverIdentity();
    public native IMAPIdentity clientIdentity();
    public native String gmailUserDisplayName();
    
    public native IMAPFolderInfoOperation folderInfoOperation(String folder);
    public native IMAPFolderStatusOperation folderStatusOperation(String folder);
    
    public native IMAPFetchFoldersOperation fetchSubscribedFoldersOperation();
    public native IMAPFetchFoldersOperation fetchAllFoldersOperation();
    
    public native IMAPOperation renameFolderOperation(String folder, String otherName);
    public native IMAPOperation deleteFolderOperation(String folder);
    public native IMAPOperation createFolderOperation(String folder);
    
    public native IMAPOperation subscribeFolderOperation(String folder);
    public native IMAPOperation unsubscribeFolderOperation(String folder);
    
    public native IMAPAppendMessageOperation appendMessageOperation(String folder, byte[] messageData, int messageFlags, List<String> customFlags);
    public IMAPAppendMessageOperation appendMessageOperation(String folder, byte[] messageData, int messageFlags)
    {
        return appendMessageOperation(folder, messageData, messageFlags, null);
    }
    
    public native IMAPCopyMessagesOperation copyMessagesOperation(String folder, IndexSet uids, String destFolder);
    
    public native IMAPOperation expungeOperation(String folder);
    
    public native IMAPFetchMessagesOperation fetchMessagesByUIDOperation(String folder, int requestKind, IndexSet indexes);
    public native  IMAPFetchMessagesOperation fetchMessagesByNumberOperation(String folder, int requestKind, IndexSet indexes);
    public native  IMAPFetchMessagesOperation syncMessagesByUIDOperation(String folder, int requestKind, IndexSet indexes, long modSeq);
    
    public native IMAPFetchContentOperation fetchMessageByUIDOperation(String folder, long uid, boolean urgent);
    public IMAPFetchContentOperation fetchMessageByUIDOperation(String folder, long uid)
    {
        return fetchMessageByUIDOperation(folder, uid, false);
    }
    
    public native IMAPFetchContentOperation fetchMessageAttachmentByUIDOperation(String folder, long uid, String partID,
                                                                                 int encoding, boolean urgent);
    public IMAPFetchContentOperation fetchMessageAttachmentByUIDOperation(String folder, long uid, String partID,
                                                                          int encoding)
    {
        return fetchMessageAttachmentByUIDOperation(folder, uid, partID, encoding, false);
    }
    
    public native IMAPFetchContentOperation fetchMessageByNumberOperation(String folder, long number, boolean urgent);
    public IMAPFetchContentOperation fetchMessageByNumberOperation(String folder, long number)
    {
        return fetchMessageByNumberOperation(folder, number, false);
    }
    
    public native IMAPFetchContentOperation fetchMessageAttachmentByNumberOperation(String folder, long number, String partID,
                                                                                    int encoding, boolean urgent);
    public IMAPFetchContentOperation fetchMessageAttachmentByNumberOperation(String folder, long number, String partID,
                                                                                    int encoding)
    {
        return fetchMessageAttachmentByNumberOperation(folder, number, partID, encoding, false);
    }
    
    public native IMAPFetchParsedContentOperation fetchParsedMessageByUIDOperation(String folder, long uid, boolean urgent);
    public IMAPFetchParsedContentOperation fetchParsedMessageByUIDOperation(String folder, long uid)
    {
        return fetchParsedMessageByUIDOperation(folder, uid, false);
    }
    
    public native IMAPFetchParsedContentOperation fetchParsedMessageByNumberOperation(String folder, long number, boolean urgent);
    public IMAPFetchParsedContentOperation fetchParsedMessageByNumberOperation(String folder, long number)
    {
        return fetchParsedMessageByNumberOperation(folder, number, false);
    }

    public native IMAPOperation storeFlagsByUIDOperation(String folder, IndexSet uids, int kind, int flags, List<String> customFlags);
    public IMAPOperation storeFlagsByUIDOperation(String folder, IndexSet uids, int kind, int flags)
    {
        return storeFlagsByUIDOperation(folder, uids, kind, flags, null);
    }
    public native IMAPOperation storeFlagsByNumberOperation(String folder, IndexSet numbers, int kind, int flags, List<String> customFlags);
    public IMAPOperation storeFlagsByNumberOperation(String folder, IndexSet numbers, int kind, int flags)
    {
        return storeFlagsByNumberOperation(folder, numbers, kind, flags, null);
    }
    public native IMAPOperation storeLabelsByUIDOperation(String folder, IndexSet uids, int kind, List<String> labels);
    public native IMAPOperation storeLabelsByNumberOperation(String folder, IndexSet numbers, int kind, List<String> labels);
    
    public native IMAPSearchOperation searchOperation(String folder, int kind, String searchString);
    public native IMAPSearchOperation searchOperation(String folder, IMAPSearchExpression expression);
    
    public native IMAPIdleOperation idleOperation(String folder, long lastKnownUID);
    
    public native IMAPFetchNamespaceOperation fetchNamespaceOperation();
    
    public native IMAPIdentityOperation identityOperation(IMAPIdentity identity);
    
    public native IMAPOperation connectOperation();
    public native IMAPOperation checkAccountOperation();
    public native IMAPOperation disconnectOperation();
    
    public native IMAPCapabilityOperation capabilityOperation();
    public native IMAPQuotaOperation quotaOperation();
    
    public native IMAPOperation noopOperation();
    
    public native IMAPMessageRenderingOperation htmlRenderingOperation(IMAPMessage message, String folder);
    public native IMAPMessageRenderingOperation htmlBodyRenderingOperation(IMAPMessage message, String folder);
    public native IMAPMessageRenderingOperation plainTextRenderingOperation(IMAPMessage message, String folder);
    public native IMAPMessageRenderingOperation plainTextBodyRenderingOperation(IMAPMessage message, String folder, boolean stripWhitespace);
    
    private native void setupNative();
    private native void finalizeNative();
    
    private ConnectionLogger connectionLogger;
    private OperationQueueListener operationQueueListener;
    
    private native void setupNativeOperationQueueListener();
    private native void setupNativeConnectionLogger();
}