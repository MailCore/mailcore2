# ActiveSync Wrapper Plan

## Goals

- Build a simple C++ ActiveSync wrapper around libetpan's low-level ActiveSync API.
- Model the wrapper after the existing MailCore IMAP core and ObjC wrapper style.
- Keep ActiveSync-specific protocol fields where needed, but map mail-shaped data into existing abstract MailCore structures when possible.
- Expose an ObjC wrapper with matching class boundaries and typed enum surfaces.

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
- Current enum-backed fields include folder type, attachment method, body type, native body type, filter type, and result statuses.

## Completed Cleanup

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
