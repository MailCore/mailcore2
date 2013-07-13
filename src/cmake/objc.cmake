# Files to build

set(async_objc_abstract
  objc/abstract/MCOAbstractMessage.mm
  objc/abstract/MCOAbstractMessagePart.mm
  objc/abstract/MCOAbstractMessageRendererCallback.mm
  objc/abstract/MCOAbstractMultipart.mm
  objc/abstract/MCOAbstractPart.mm
  objc/abstract/MCOAddress.mm
  objc/abstract/MCOMessageHeader.mm
)

set(async_objc_imap
  objc/imap/MCOIMAPAppendMessageOperation.mm
  objc/imap/MCOIMAPBaseOperation.mm
  objc/imap/MCOIMAPCapabilityOperation.mm
  objc/imap/MCOIMAPCopyMessagesOperation.mm
  objc/imap/MCOIMAPFetchContentOperation.mm
  objc/imap/MCOIMAPFetchFoldersOperation.mm
  objc/imap/MCOIMAPFetchMessagesOperation.mm
  objc/imap/MCOIMAPFetchNamespaceOperation.mm
  objc/imap/MCOIMAPFolder.mm
  objc/imap/MCOIMAPFolderInfo.m
  objc/imap/MCOIMAPFolderInfoOperation.mm
  objc/imap/MCOIMAPFolderStatus.m
  objc/imap/MCOIMAPFolderStatusOperation.mm
  objc/imap/MCOIMAPIdentityOperation.mm
  objc/imap/MCOIMAPIdleOperation.mm
  objc/imap/MCOIMAPMessage.mm
  objc/imap/MCOIMAPMessagePart.mm
  objc/imap/MCOIMAPMessageRenderingOperation.mm
  objc/imap/MCOIMAPMultipart.mm
  objc/imap/MCOIMAPNamespace.mm
  objc/imap/MCOIMAPNamespaceItem.mm
  objc/imap/MCOIMAPOperation.mm
  objc/imap/MCOIMAPPart.mm
  objc/imap/MCOIMAPSearchExpression.mm
  objc/imap/MCOIMAPSearchOperation.mm
  objc/imap/MCOIMAPSession.mm
)

set(async_objc_pop
  objc/pop/MCOPOPFetchHeaderOperation.mm
  objc/pop/MCOPOPFetchMessageOperation.mm
  objc/pop/MCOPOPFetchMessagesOperation.mm
  objc/pop/MCOPOPMessageInfo.mm
  objc/pop/MCOPOPOperation.mm
  objc/pop/MCOPOPSession.mm
)

set(async_objc_provider
  objc/provider/MCOMailProvider.mm
  objc/provider/MCOMailProvidersManager.mm
  objc/provider/MCONetService.mm
)

set(async_objc_rfc822
  objc/rfc822/MCOAttachment.mm
  objc/rfc822/MCOMessageBuilder.mm
  objc/rfc822/MCOMessageParser.mm
  objc/rfc822/MCOMessagePart.mm
  objc/rfc822/MCOMultipart.mm
)

set(async_objc_smtp
  objc/smtp/MCOSMTPOperation.mm
  objc/smtp/MCOSMTPSendOperation.mm
  objc/smtp/MCOSMTPSession.mm
)

set(async_objc_utils
  objc/utils/MCOIndexSet.mm
  objc/utils/MCOObjectWrapper.mm
  objc/utils/MCOOperation.mm
  objc/utils/MCORange.mm
  objc/utils/NSArray+MCO.mm
  objc/utils/NSData+MCO.mm
  objc/utils/NSDictionary+MCO.mm
  objc/utils/NSError+MCO.mm
  objc/utils/NSObject+MCO.mm
  objc/utils/NSString+MCO.mm
  objc/utils/NSValue+MCO.mm
)

IF(APPLE)
set(async_objc
  ${async_objc_abstract}
  ${async_objc_imap}
  ${async_objc_pop}
  ${async_objc_rfc822}
  ${async_objc_smtp}
  ${async_objc_utils}
)
ENDIF()

# Includes for build

set(objc_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/objc
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/abstract
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/imap
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/pop
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/rfc822
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/smtp
  ${CMAKE_CURRENT_SOURCE_DIR}/objc/utils
)
