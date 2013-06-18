# public headers

set(public_headers_abstract
  core/abstract/MCAbstract.h
  core/abstract/MCAbstractMessage.h
  core/abstract/MCAbstractMessagePart.h
  core/abstract/MCAbstractMultipart.h
  core/abstract/MCAbstractPart.h
  core/abstract/MCAddress.h
  core/abstract/MCMessageConstants.h
  core/abstract/MCMessageHeader.h
)

set(public_headers_basetypes
  core/basetypes/MCArray.h
  core/basetypes/MCAssert.h
  core/basetypes/MCAutoreleasePool.h
  core/basetypes/MCBaseTypes.h
  core/basetypes/MCData.h
  core/basetypes/MCHash.h
  core/basetypes/MCHashMap.h
  core/basetypes/MCHTMLCleaner.h
  core/basetypes/MCIndexSet.h
  core/basetypes/MCIterator.h
  core/basetypes/MCICUTypes.h
  core/basetypes/MCJSON.h
  core/basetypes/MCLibetpanTypes.h
  core/basetypes/MCLog.h
  core/basetypes/MCMainThread.h
  core/basetypes/MCMD5.h
  core/basetypes/MCNull.h
  core/basetypes/MCObject.h
  core/basetypes/MCOperation.h
  core/basetypes/MCOperationCallback.h
  core/basetypes/MCOperationQueue.h
  core/basetypes/MCRange.h
  core/basetypes/MCSet.h
  core/basetypes/MCString.h
  core/basetypes/MCUtils.h
  core/basetypes/MCValue.h
)

set(public_headers_imap
  core/imap/MCIMAP.h
  core/imap/MCIMAPFolder.h
  core/imap/MCIMAPFolderStatus.h
  core/imap/MCIMAPMessage.h
  core/imap/MCIMAPMessagePart.h
  core/imap/MCIMAPMultipart.h
  core/imap/MCIMAPNamespace.h
  core/imap/MCIMAPNamespaceItem.h
  core/imap/MCIMAPPart.h
  core/imap/MCIMAPProgressCallback.h
  core/imap/MCIMAPSearchExpression.h
  core/imap/MCIMAPSession.h
  core/imap/MCIMAPSyncResult.h
)

set(public_headers_pop
  core/pop/MCPOP.h
  core/pop/MCPOPMessageInfo.h
  core/pop/MCPOPProgressCallback.h
  core/pop/MCPOPSession.h
)

set(public_headers_provider
  core/provider/MCMailProvider.h
  core/provider/MCMailProvidersManager.h
  core/provider/MCNetService.h
  core/provider/MCProvider.h
)

set(public_headers_renderer
  core/renderer/MCAddressDisplay.h
  core/renderer/MCDateFormatter.h
  core/renderer/MCHTMLRendererCallback.h
  core/renderer/MCRenderer.h
  core/renderer/MCSizeFormatter.h
)

set(public_headers_rfc822
  core/rfc822/MCAttachment.h
  core/rfc822/MCMessageBuilder.h
  core/rfc822/MCMessageParser.h
  core/rfc822/MCMessagePart.h
  core/rfc822/MCMultipart.h
  core/rfc822/MCRFC822.h
)

set(public_headers_smtp
  core/smtp/MCSMTP.h
  core/smtp/MCSMTPProgressCallback.h
  core/smtp/MCSMTPSession.h
)

set(public_headers_core
  core/MCCore.h
  ${public_headers_basetypes}
  ${public_headers_renderer}
  ${public_headers_imap}
  ${public_headers_pop}
  ${public_headers_smtp}
  ${public_headers_abstract}
  ${public_headers_rfc822}
  ${public_headers_provider}
)

# Files to build

set(abstract_files
  core/abstract/MCAbstractMessage.cc
  core/abstract/MCAbstractMessagePart.cc
  core/abstract/MCAbstractMultipart.cc
  core/abstract/MCAbstractPart.cc
  core/abstract/MCAddress.cc
  core/abstract/MCMessageHeader.cc
)

IF(APPLE)
  set(basetypes_files_apple
    core/basetypes/MCAutoreleasePoolMac.mm
    core/basetypes/MCMainThread.mm
    core/basetypes/MCObjectMac.mm
  )
ENDIF()

set(basetypes_files
  core/basetypes/MCArray.cc
  core/basetypes/MCAssert.cc
  core/basetypes/MCAutoreleasePool.cc
  core/basetypes/MCData.cc
  core/basetypes/MCHash.cc
  core/basetypes/MCHashMap.cc
  core/basetypes/MCHTMLCleaner.cc
  core/basetypes/MCIndexSet.cc
  core/basetypes/MCJSON.cc
  core/basetypes/MCJSONParser.cc
  core/basetypes/MCLog.cc
  core/basetypes/MCMD5.cc
  core/basetypes/MCNull.cc
  core/basetypes/MCObject.cc
  core/basetypes/MCOperation.cc
  core/basetypes/MCOperationQueue.cc
  core/basetypes/MCRange.cc
  core/basetypes/MCSet.cc
  core/basetypes/MCString.cc
  core/basetypes/MCValue.cc
  ${basetypes_files_apple}
)

set(imap_files
  core/imap/MCIMAPFolder.cc
  core/imap/MCIMAPFolderStatus.cc
  core/imap/MCIMAPMessage.cc
  core/imap/MCIMAPMessagePart.cc
  core/imap/MCIMAPMultipart.cc
  core/imap/MCIMAPNamespace.cc
  core/imap/MCIMAPNamespaceItem.cc
  core/imap/MCIMAPPart.cc
  core/imap/MCIMAPSearchExpression.cc
  core/imap/MCIMAPSession.cc
  core/imap/MCIMAPSyncResult.cc
)

set(pop_files
  core/pop/MCPOPMessageInfo.cc
  core/pop/MCPOPSession.cc
)

set(provider_files
  core/provider/MCMailProvider.cc
  core/provider/MCMailProvidersManager.cc
  core/provider/MCNetService.cc
)

set(renderer_files
  core/renderer/MCAddressDisplay.cc
  core/renderer/MCDateFormatter.cc
  core/renderer/MCHTMLRenderer.cc
  core/renderer/MCHTMLRendererCallback.cc
  core/renderer/MCSizeFormatter.cc
)

set(rfc822_files
  core/rfc822/MCAttachment.cc
  core/rfc822/MCMessageBuilder.cc
  core/rfc822/MCMessageParser.cc
  core/rfc822/MCMessagePart.cc
  core/rfc822/MCMultipart.cc
)

set(smtp_files
  core/smtp/MCSMTPSession.cc
)

set(core_files
  ${basetypes_files}
  ${abstract_files}
  ${imap_files}
  ${pop_files}
  ${renderer_files}
  ${rfc822_files}
  ${smtp_files}
)

# Includes for build

set(core_includes
  ${CMAKE_CURRENT_SOURCE_DIR}/core
  ${CMAKE_CURRENT_SOURCE_DIR}/core/abstract
  ${CMAKE_CURRENT_SOURCE_DIR}/core/basetypes
  ${CMAKE_CURRENT_SOURCE_DIR}/core/imap
  ${CMAKE_CURRENT_SOURCE_DIR}/core/pop
  ${CMAKE_CURRENT_SOURCE_DIR}/core/provider
  ${CMAKE_CURRENT_SOURCE_DIR}/core/renderer
  ${CMAKE_CURRENT_SOURCE_DIR}/core/rfc822
  ${CMAKE_CURRENT_SOURCE_DIR}/core/smtp
)
