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
    core/basetypes/MCDataMac.mm
    core/rfc822/MCMessageParserMac.mm
  )
ENDIF()

set(basetypes_files
  core/basetypes/MCArray.cc
  core/basetypes/MCAssert.c
  core/basetypes/MCAutoreleasePool.cc
  core/basetypes/MCBase64.c
  core/basetypes/MCConnectionLoggerUtils.cc
  core/basetypes/MCData.cc
  core/basetypes/MCHash.cc
  core/basetypes/MCHashMap.cc
  core/basetypes/MCHTMLCleaner.cc
  core/basetypes/MCIndexSet.cc
  core/basetypes/MCJSON.cc
  core/basetypes/MCJSONParser.cc
  core/basetypes/MCLibetpan.cc
  core/basetypes/MCLog.c
  core/basetypes/MCMD5.cc
  core/basetypes/MCNull.cc
  core/basetypes/MCObject.cc
  core/basetypes/MCOperation.cc
  core/basetypes/MCOperationQueue.cc
  core/basetypes/MCRange.cc
  core/basetypes/MCSet.cc
  core/basetypes/MCString.cc
  core/basetypes/MCValue.cc
  core/basetypes/ConvertUTF.c
  ${basetypes_files_apple}
)

set(imap_files
  core/imap/MCIMAPFolder.cc
  core/imap/MCIMAPFolderStatus.cc
  core/imap/MCIMAPIdentity.cc
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

set(nntp_files
  core/nntp/MCNNTPGroupInfo.cc
  core/nntp/MCNNTPSession.cc
)

set(provider_files
  core/provider/MCMailProvider.cc
  core/provider/MCMailProvidersManager.cc
  core/provider/MCNetService.cc
)

set(renderer_files
  core/renderer/MCAddressDisplay.cc
  core/renderer/MCDateFormatter.cc
  core/renderer/MCHTMLBodyRendererTemplateCallback.cc
  core/renderer/MCHTMLRenderer.cc
  core/renderer/MCHTMLRendererCallback.cc
  core/renderer/MCHTMLRendererIMAPDataCallback.cc
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

set(zip_files
  core/zip/MCZip.cc
  core/zip/MiniZip/ioapi.c
  core/zip/MiniZip/mztools.c
  core/zip/MiniZip/unzip.c
  core/zip/MiniZip/zip.c
)

set(security_files
  core/security/MCCertificateUtils.cc
)

set(core_files
  ${basetypes_files}
  ${abstract_files}
  ${imap_files}
  ${pop_files}
  ${nntp_files}
  ${renderer_files}
  ${rfc822_files}
  ${security_files}
  ${smtp_files}
  ${zip_files}
)

# Includes for build

set(core_includes
  "${CMAKE_CURRENT_SOURCE_DIR}/core"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/abstract"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/basetypes"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/imap"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/pop"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/nntp"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/provider"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/renderer"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/rfc822"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/security"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/smtp"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/zip"
  "${CMAKE_CURRENT_SOURCE_DIR}/core/zip/MiniZip"
)
