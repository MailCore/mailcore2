# Files to build

set(async_imap_files
  async/imap/MCIMAPAppendMessageOperation.cc
  async/imap/MCIMAPAsyncConnection.cc
  async/imap/MCIMAPAsyncSession.cc
  async/imap/MCIMAPCapabilityOperation.cc
  async/imap/MCIMAPCheckAccountOperation.cc
  async/imap/MCIMAPConnectOperation.cc
  async/imap/MCIMAPCopyMessagesOperation.cc
  async/imap/MCIMAPCreateFolderOperation.cc
  async/imap/MCIMAPDeleteFolderOperation.cc
  async/imap/MCIMAPDisconnectOperation.cc
  async/imap/MCIMAPExpungeOperation.cc
  async/imap/MCIMAPFetchContentOperation.cc
  async/imap/MCIMAPFetchParsedContentOperation.cc
  async/imap/MCIMAPFetchFoldersOperation.cc
  async/imap/MCIMAPFetchMessagesOperation.cc
  async/imap/MCIMAPFetchNamespaceOperation.cc
  async/imap/MCIMAPFolderInfoOperation.cc
  async/imap/MCIMAPFolderStatusOperation.cc
  async/imap/MCIMAPIdentityOperation.cc
  async/imap/MCIMAPIdleOperation.cc
  async/imap/MCIMAPMessageRenderingOperation.cc
  async/imap/MCIMAPMultiDisconnectOperation.cc
  async/imap/MCIMAPOperation.cc
  async/imap/MCIMAPQuotaOperation.cc
  async/imap/MCIMAPRenameFolderOperation.cc
  async/imap/MCIMAPSearchOperation.cc
  async/imap/MCIMAPStoreFlagsOperation.cc
  async/imap/MCIMAPStoreLabelsOperation.cc
  async/imap/MCIMAPSubscribeFolderOperation.cc
  async/imap/MCIMAPNoopOperation.cc
)

set(async_pop_files
  async/pop/MCPOPAsyncSession.cc
  async/pop/MCPOPCheckAccountOperation.cc
  async/pop/MCPOPDeleteMessagesOperation.cc
  async/pop/MCPOPFetchHeaderOperation.cc
  async/pop/MCPOPFetchMessageOperation.cc
  async/pop/MCPOPFetchMessagesOperation.cc
  async/pop/MCPOPNoopOperation.cc
  async/pop/MCPOPOperation.cc
)

set(async_smtp_files
  async/smtp/MCSMTPAsyncSession.cc
  async/smtp/MCSMTPCheckAccountOperation.cc
  async/smtp/MCSMTPDisconnectOperation.cc
  async/smtp/MCSMTPOperation.cc
  async/smtp/MCSMTPLoginOperation.cc
  async/smtp/MCSMTPSendWithDataOperation.cc
  async/smtp/MCSMTPNoopOperation.cc
)

set(async_nntp_files
  async/nntp/MCNNTPAsyncSession.cc
  async/nntp/MCNNTPCheckAccountOperation.cc
  async/nntp/MCNNTPDisconnectOperation.cc
  async/nntp/MCNNTPFetchArticleOperation.cc
  async/nntp/MCNNTPFetchAllArticlesOperation.cc
  async/nntp/MCNNTPFetchHeaderOperation.cc
  async/nntp/MCNNTPListNewsgroupsOperation.cc
  async/nntp/MCNNTPFetchOverviewOperation.cc
  async/nntp/MCNNTPFetchServerTimeOperation.cc
  async/nntp/MCNNTPOperation.cc
)

set(async_files
  ${async_imap_files}
  ${async_pop_files}
  ${async_smtp_files}
  ${async_nntp_files}
)

# Includes for build

set(async_includes
  "${CMAKE_CURRENT_SOURCE_DIR}/async"
  "${CMAKE_CURRENT_SOURCE_DIR}/async/imap"
  "${CMAKE_CURRENT_SOURCE_DIR}/async/pop"
  "${CMAKE_CURRENT_SOURCE_DIR}/async/smtp"
  "${CMAKE_CURRENT_SOURCE_DIR}/async/nntp"
)
