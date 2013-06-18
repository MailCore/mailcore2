# public headers

set(public_headers_async_imap
  async/imap/MCAsyncIMAP.h
  async/imap/MCIMAPAppendMessageOperation.h
  async/imap/MCIMAPAsyncSession.h
  async/imap/MCIMAPCapabilityOperation.h
  async/imap/MCIMAPCopyMessagesOperation.h
  async/imap/MCIMAPFetchContentOperation.h
  async/imap/MCIMAPFetchFoldersOperation.h
  async/imap/MCIMAPFetchMessagesOperation.h
  async/imap/MCIMAPFetchNamespaceOperation.h
  async/imap/MCIMAPFolderInfoOperation.h
  async/imap/MCIMAPFolderStatusOperation.h
  async/imap/MCIMAPIdentityOperation.h
  async/imap/MCIMAPIdleOperation.h
  async/imap/MCIMAPOperation.h
  async/imap/MCIMAPOperationCallback.h
  async/imap/MCIMAPSearchOperation.h
)

set(public_headers_async_pop
  async/pop/MCAsyncPOP.h
  async/pop/MCPOPAsyncSession.h
  async/pop/MCPOPFetchHeaderOperation.h
  async/pop/MCPOPFetchMessageOperation.h
  async/pop/MCPOPFetchMessagesOperation.h
  async/pop/MCPOPOperation.h
  async/pop/MCPOPOperationCallback.h
)

set(public_headers_async_smtp
  async/smtp/MCAsyncSMTP.h
  async/smtp/MCSMTPAsyncSession.h
  async/smtp/MCSMTPOperation.h
  async/smtp/MCSMTPOperationCallback.h
)

set(public_headers_async
  async/MCAsync.h
  ${public_headers_async_pop}
  ${public_headers_async_smtp}
  ${public_headers_async_imap}
)

# Files to build

set(async_imap_files
  async/imap/MCIMAPAppendMessageOperation.cc
  async/imap/MCIMAPAsyncConnection.cc
  async/imap/MCIMAPAsyncSession.cc
  async/imap/MCIMAPCapabilityOperation.cc
  async/imap/MCIMAPCheckAccountOperation.cc
  async/imap/MCIMAPCopyMessagesOperation.cc
  async/imap/MCIMAPCreateFolderOperation.cc
  async/imap/MCIMAPDeleteFolderOperation.cc
  async/imap/MCIMAPExpungeOperation.cc
  async/imap/MCIMAPFetchContentOperation.cc
  async/imap/MCIMAPFetchFoldersOperation.cc
  async/imap/MCIMAPFetchMessagesOperation.cc
  async/imap/MCIMAPFetchNamespaceOperation.cc
  async/imap/MCIMAPFolderInfoOperation.cc
  async/imap/MCIMAPIdentityOperation.cc
  async/imap/MCIMAPIdleOperation.cc
  async/imap/MCIMAPOperation.cc
  async/imap/MCIMAPRenameFolderOperation.cc
  async/imap/MCIMAPSearchOperation.cc
  async/imap/MCIMAPStoreFlagsOperation.cc
  async/imap/MCIMAPStoreLabelsOperation.cc
  async/imap/MCIMAPSubscribeFolderOperation.cc
)

set(async_pop_files
  async/pop/MCPOPAsyncSession.cc
  async/pop/MCPOPCheckAccountOperation.cc
  async/pop/MCPOPDeleteMessagesOperation.cc
  async/pop/MCPOPFetchHeaderOperation.cc
  async/pop/MCPOPFetchMessageOperation.cc
  async/pop/MCPOPFetchMessagesOperation.cc
  async/pop/MCPOPOperation.cc
)

set(async_smtp_files
  async/smtp/MCSMTPAsyncSession.cc
  async/smtp/MCSMTPCheckAccountOperation.cc
  async/smtp/MCSMTPOperation.cc
  async/smtp/MCSMTPSendWithDataOperation.cc
)

set(async_files
  ${async_imap_files}
  ${async_pop_files}
  ${async_smtp_files}
)

# Includes for build

set(async_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/async
  ${CMAKE_CURRENT_SOURCE_DIR}/async/imap
  ${CMAKE_CURRENT_SOURCE_DIR}/async/pop
  ${CMAKE_CURRENT_SOURCE_DIR}/async/smtp
)
