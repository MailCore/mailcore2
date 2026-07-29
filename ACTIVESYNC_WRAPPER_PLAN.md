# ActiveSync Wrapper Plan

## Goals

- Build a simple C++ ActiveSync wrapper around libetpan's low-level ActiveSync API.
- Model the wrapper after the existing MailCore IMAP core and ObjC wrapper style.
- Keep ActiveSync-specific protocol fields where needed, but map mail-shaped data into existing abstract MailCore structures when possible.
- Expose an ObjC wrapper with matching class boundaries and typed enum surfaces.
- Keep the public ActiveSync surface focused on email. Protocol details that are needed only for parsing or libetpan interop should move behind private headers or implementation helpers.

## Current Structure

- C++ ActiveSync public umbrella: `src/core/activesync/MCActiveSync.h`
- C++ ActiveSync enum definitions: `src/core/activesync/MCActiveSyncTypes.h`
- C++ declarations: one public header per class in `src/core/activesync`
- C++ implementations: one `.cpp` per class in `src/core/activesync`
- ObjC ActiveSync public umbrella: `src/objc/activesync/MCOActiveSync.h`
- ObjC enum definitions: `src/objc/activesync/MCOActiveSyncTypes.h`
- ObjC declarations: one public header per class in `src/objc/activesync`
- ObjC implementations: one `.mm` per class in `src/objc/activesync`

## Abstract Data Mapping

- `ActiveSyncMessage` inherits from `AbstractMessage`.
- Message header fields from ActiveSync (`Subject`, `From`, `To`, `Cc`, `Reply-To`, `Date`) are parsed into `MessageHeader`.
- Duplicate message header fields should stay removed from `ActiveSyncMessage` and `MCOActiveSyncMessage`.
- ItemOperations Fetch returns `ActiveSyncMessage` / `MCOActiveSyncMessage`, not a separate item wrapper.
- Fetched MIME data is imported into the inherited `MessageHeader` when available.
- `ActiveSyncAttachment` inherits from `AbstractPart`.
- Attachment `display_name` should map to inherited `filename`.
- Attachment `file_reference` should map to inherited `uniqueID`.
- Attachment `content_type` should map to inherited `mimeType`.
- Attachment `content_id`, `content_location`, `is_inline`, and attachment state should use inherited `AbstractPart` fields.
- ActiveSync attachment-specific fields are limited to `method` and `estimatedDataSize`.
- `ActiveSyncBody.content_type` is exposed as `mimeType` for naming consistency.

## Enum Policy

- Keep protocol integer values intact.
- Expose typed C++ enums in `MCActiveSyncTypes.h`.
- Expose matching ObjC `NS_ENUM` types in `MCOActiveSyncTypes.h`.
- Keep public enum-backed fields for attachment method, body type, native body type, filter type, and result statuses.
- Move `ActiveSyncFolderType` out of `MCActiveSyncTypes.h` into `MCActiveSyncTypesPrivate.h`.
- Move `MCOActiveSyncFolderType` out of `MCOActiveSyncTypes.h` into `MCOActiveSyncTypesPrivate.h` if ObjC still needs to bridge the native folder type internally.
- Do not expose calendar/contact/task/note/journal folder categories in the public mail API. Use private folder-type helpers to classify ActiveSync folders as mail folders before returning them.

## Mail-Focused Public API

- Public APIs should prefer mail terms over ActiveSync protocol terms:
  - `folderID` instead of `collectionID`.
  - `messageID` instead of `serverID` where the ID identifies an email message.
  - `messageData` instead of `MIMEData`.
- Keep protocol-level names in private helpers when they map directly to libetpan calls.
- Replace or supplement `sync(ActiveSyncSyncRequest *)` / `syncWithRequest:` with a mail-shaped sync entry point that builds the underlying ActiveSync request internally:
  - C++: `syncMessages(String * folderID, String * syncKey, ActiveSyncMessageSyncOptions * options, ErrorCode * pError)`.
  - ObjC: `syncMessagesInFolderID:syncKey:options:error:`.
- The mail-shaped sync method should set `collectionClass` to `Email` internally.
- Keep advanced `ActiveSyncSyncRequest` available only if a lower-level protocol API is still desired.
- Rename the fetch/send/reply/forward surface to align with MailCore mail APIs:
  - C++ `fetchMessage(String * folderID, String * messageID, ErrorCode * pError)` wrapping ItemOperations Fetch.
  - ObjC `fetchMessageInFolderID:messageID:error:`.
  - C++ `sendMessage(Data * messageData, bool saveInSent, ErrorCode * pError)`.
  - ObjC `sendMessageWithData:saveInSent:error:`.
  - C++ `smartReply(String * folderID, String * messageID, Data * messageData, bool saveInSent, ErrorCode * pError)`.
  - ObjC `smartReplyInFolderID:messageID:messageData:saveInSent:error:`.
  - C++ `smartForward(String * folderID, String * messageID, Data * messageData, bool saveInSent, ErrorCode * pError)`.
  - ObjC `smartForwardInFolderID:messageID:messageData:saveInSent:error:`.
- `messageData` is RFC 822 formatted message data, matching the SMTP/IMAP/POP/NNTP naming used elsewhere in MailCore.

## Device Identity Policy

- Callers should not have to set `deviceID` explicitly for the common mail-only API.
- `ActiveSyncSession` / `MCOActiveSyncSession` should generate or provide a stable default `deviceID` when none is set before connecting.
- The generated `deviceID` must be stable for the account/device pair so server policy state and sync state do not churn between app launches.
- Keep `deviceID` as the only public device identity override for callers that need deterministic migration, testing, or product-specific identifiers.
- Do not expose `deviceType`; pass a conservative internal device type of `MailCore`.
- The implementation should still pass the resolved device identity through `mailactivesync_set_device()` before `connect`.

## Completed Cleanup

- Moved `ActiveSyncFolderType` and `MCOActiveSyncFolderType` into private ActiveSync type headers.
- Removed the public folder `type` property/accessors and filter FolderSync added/updated folders to mail folder types during native conversion.
- Renamed ActiveSync RFC 822 payload APIs from `MIMEData` to `messageData`.
- Renamed fetch/send/reply/forward session APIs from item/collection/MIME terms to message/folder/messageData terms.
- Added `syncMessages` / `syncMessagesInFolderID:syncKey:error:` as the mail-shaped sync entry point that sets the ActiveSync collection class to `Email`.
- Added default device identity resolution before connect: generated stable `deviceID` when unset and default `deviceType` of `MailCore`.
- Removed `deviceType` from the public C++ and ObjC APIs; the session now always passes `MailCore` internally.
- Removed wrapper-level ActiveSync cache toggles and cache directory configuration; updated libetpan no longer requires them in `mailactivesync_new()`.
- Made `protocolVersion`, `policyKey`, and `userAgent` internal-only session configuration instead of public ActiveSync session API.
- Removed public device-information settings from the mail-focused session API.
- Removed duplicate `ActiveSyncAttachment` and `MCOActiveSyncAttachment` accessors for values already represented by `AbstractPart` / `MCOAbstractPart`:
  - `displayName`
  - `fileReference`
  - `contentType`
- Kept `ActiveSyncAttachment::method` and `estimatedDataSize`, because `AbstractPart` has no equivalent fields.
- Added `ActiveSyncMessage` overrides for abstract part lookup:
  - `attachments()`
  - `htmlInlineAttachments()`
  - `partForContentID()`
  - `partForUniqueID()`
- Removed the separate `ActiveSyncItem` / `MCOActiveSyncItem` wrapper and made fetch return message objects.

## Verification

- Run focused C++ syntax checks over every `src/core/activesync/*.cpp`.
- Run whitespace checks over `src/core/activesync`, `src/objc/activesync`, and touched CMake files.
- Run ObjC++ syntax/build checks when a toolchain with `clang++` is available.
