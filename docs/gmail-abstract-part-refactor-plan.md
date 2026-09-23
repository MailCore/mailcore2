# Gmail Abstract Part Refactor Plan

Date: 2026-09-23

## Goal

Refactor the first-pass Gmail core API to follow MailCore's IMAP/RFC822 part
model more closely.

The key design correction is to avoid separate public wrapper objects for
"part body" and "fetched attachment data" when the existing MailCore pattern is:

- Part objects describe message structure.
- Session fetch methods return `Data *` for bytes.

## IMAP Pattern To Copy

`src/core/imap` does not expose a separate body-data wrapper equivalent to
`GmailMessagePartBody`, nor a fetched attachment wrapper equivalent to the
current `GmailAttachment`.

Instead:

- `IMAPPart` subclasses `AbstractPart` and describes a leaf/body part:
  - `partID`
  - inherited `filename`
  - inherited `mimeType`
  - inherited content metadata
  - size
  - encoding
- `IMAPMessagePart` subclasses `AbstractMessagePart` for nested
  `message/rfc822` parts.
- `IMAPMultipart` subclasses `AbstractMultipart` for containers.
- `IMAPSession` fetch methods return `Data *` for actual content bytes:
  - `fetchMessageByUID(...)`
  - `fetchMessageAttachmentByUID(...)`
  - `fetchMessageAttachmentToFileByUID(...)`

The structure tree and fetched bytes are deliberately not modeled as two
parallel public object hierarchies.

## Current Gmail Problem

The first-pass Gmail API currently has:

- `GmailMessagePart`
- `GmailMessagePartBody`
- `GmailAttachment`

This mirrors Gmail REST JSON too literally:

- `MessagePart.body` can contain inline `data`, or an `attachmentId` pointer.
- `users.messages.attachments.get` returns an attachment resource containing
  the fetched data for that `attachmentId`.

That JSON distinction is useful internally, but it creates public API
duplication in MailCore:

- `GmailMessagePartBody` is not really a MailCore part.
- `GmailAttachment` is not really structure; it is fetched bytes plus a Gmail
  ID.
- Callers have to understand two Gmail REST concepts instead of one MailCore
  part model plus `Data *` fetch methods.

## Target Public Model

### GmailMessage

Make `GmailMessage` subclass `AbstractMessage`.

Keep Gmail-specific fields:

- `identifier`
- `threadID`
- `labelIDs`
- `snippet`
- `historyID`
- `internalDate`
- `sizeEstimate`
- `RFC822Data`
- `payload`

Add or keep:

- `payload()` / `setPayload(GmailMessagePart *)`
- `partForContentID(String *)`
- `partForUniqueID(String *)`
- `partForPartID(String *)`
- `parsedMessage(ErrorCode *)`

Use inherited `header()` from `AbstractMessage` for parsed/converted message
headers when practical.

### GmailMessagePart

Make `GmailMessagePart` subclass `AbstractMessagePart`.

This is a compromise with the user's requested inheritance. It gives the part
access to `header()` and `mainPart()` behavior, while retaining Gmail-specific
payload metadata. During implementation, verify whether Gmail's ordinary
non-`message/rfc822` body parts would be better modeled as `AbstractPart`
instead; if so, consider a later split into:

- `GmailPart : AbstractPart`
- `GmailMessagePart : AbstractMessagePart`
- `GmailMultipart : AbstractMultipart`

For the immediate refactor, keep one `GmailMessagePart` public type to avoid
expanding the API.

Move common fields into inherited `AbstractPart` accessors:

- Gmail `mimeType` -> inherited `mimeType`
- Gmail `filename` -> inherited `filename`

Keep Gmail-specific fields on `GmailMessagePart`:

- `partID`
- `attachmentID`
- `size`
- decoded inline `data`
- ordered Gmail header list, if still needed for exact Gmail payload shape
- child `parts`

Do not keep a public `GmailMessagePartBody` object. Fold its fields into
`GmailMessagePart`:

- `body.attachment_id` -> `GmailMessagePart::attachmentID`
- `body.size` -> `GmailMessagePart::size`
- decoded `body.data` -> `GmailMessagePart::data`

### GmailAttachment

Remove `GmailAttachment` from the public first-pass API.

Replace:

```cpp
GmailAttachment * attachment(String * messageID,
                             String * attachmentID,
                             ErrorCode * pError);
```

with:

```cpp
Data * attachmentData(String * messageID,
                      String * attachmentID,
                      ErrorCode * pError);
```

Add a convenience that accepts a part:

```cpp
Data * dataForMessagePart(String * messageID,
                          GmailMessagePart * part,
                          ErrorCode * pError);
```

Behavior:

- If `part->data()` is already present, return a retained/autoreleased copy or
  retained/autoreleased reference to that decoded data.
- Else if `part->attachmentID()` is present, call
  `attachmentData(messageID, part->attachmentID(), pError)`.
- Else return `NULL` with an appropriate fetch-style error.

## GmailMessagePartBody vs GmailAttachment

`GmailMessagePartBody` represents the `body` object embedded in a Gmail message
payload part.

It may contain:

- `data`: inline body bytes included directly in the message response.
- `attachmentID`: a token for fetching bytes separately.
- `size`: body size metadata.

`GmailAttachment` currently represents the result of fetching that separate
attachment resource.

The IMAP-style MailCore API should not expose both. Publicly:

- The message part should hold structure and any inline bytes.
- The session should return `Data *` for separately fetched bytes.

## Refactor Steps

1. Change `GmailMessage` inheritance from `Object` to `AbstractMessage`.
2. Change `GmailMessagePart` inheritance from `Object` to
   `AbstractMessagePart`.
3. Change common `GmailMessagePart` fields to use inherited setters/getters:
   `mimeType()` and `filename()`.
4. Move `GmailMessagePartBody` fields onto `GmailMessagePart`:
   `attachmentID`, `size`, and `data`.
5. Remove `GmailMessagePartBody` from public headers and `MCGmail.h`.
6. Remove `GmailAttachment` from public headers and `MCGmail.h`.
7. Replace `GmailSession::attachment(...)` with
   `GmailSession::attachmentData(...)`.
8. Add `GmailSession::dataForMessagePart(...)`.
9. Update conversion helpers:
   - Convert Gmail `MessagePart.body` directly into `GmailMessagePart` fields.
   - Decode Gmail base64url `body.data` into `GmailMessagePart::data`.
   - Decode fetched Gmail attachment resource data into returned `Data *`.
10. Add `GmailMessage` part traversal:
    - `partForPartID`
    - `partForContentID`
    - `partForUniqueID`
11. Update CMake source lists and public headers.
12. Rebuild `MailCore`.
13. Update `docs/gmail-core-api-plan.md` to reflect the deduplicated public
    API.

## Verification

- Build `MailCore`:

```sh
cmake --build build --target MailCore -j2
```

- Add or run focused tests for:
  - inline body data stored on `GmailMessagePart`
  - attachment ID stored on `GmailMessagePart`
  - `attachmentData()` returning decoded bytes
  - `dataForMessagePart()` returning inline data when available
  - `dataForMessagePart()` fetching by `attachmentID` when inline data is not
    available
  - `GmailMessage` part traversal by part ID, content ID, and unique ID

## Open Question

The user requested `GmailMessagePart : AbstractMessagePart`. That can work for
the immediate refactor, but it may overstate what ordinary Gmail parts are:
most Gmail payload parts are body parts, not nested full message parts.

After the deduplication pass, review whether a cleaner long-term model should
mirror IMAP exactly:

- `GmailPart : AbstractPart`
- `GmailMessagePart : AbstractMessagePart`
- `GmailMultipart : AbstractMultipart`

For now, prefer the smaller requested refactor unless implementation friction
shows the split is necessary.
