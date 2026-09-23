# ActiveSync Gap Plan

Date: 2026-08-02

## Current Smaller-Commit Plan

Date: 2026-09-22

This phase plan is based on the current low-level ActiveSync implementation in
`/home/dvh/Sources/libetpan/src/low-level/activesync` and the MailCore wrapper
surface in `src/core/activesync` and `src/objc/activesync`.

The main approach is to keep ActiveSync as its own protocol-shaped API instead
of forcing it into the IMAP UID/mailbox abstraction. We should still mirror the
IMAP wrapper discipline: thin C++ session methods over libetpan primitives,
typed result/request objects, then Objective-C bridges.

### Phase 1: Current Parity Audit

Goal: make the implementation target explicit before adding API surface.

- Confirm which libetpan ActiveSync primitives are currently implemented and
  exported.
- Compare them to `ActiveSyncSession` and `MCOActiveSyncSession`.
- Record missing MailCore wrappers separately from missing libetpan primitives.
- Keep this phase documentation-only unless build metadata needs updating.

### Phase 2: Folder Primitives

Goal: expose folder lifecycle operations in a compact, self-contained change.

- Add C++ session methods for `folderResync`, `folderCreate`, `folderUpdate`,
  and `folderDelete`.
- Add an `ActiveSyncFolderMutationResult` wrapper for returned sync key,
  server ID, and status.
- Add Objective-C result and bridge methods.
- Add focused conversion tests where practical.

### Phase 3: Message Mutation Primitives

Goal: support common mailbox actions as batch operations without changing sync
modeling yet.

- Add C++ session methods for marking messages read/unread, flagging/unflagging
  messages, and deleting messages using arrays of message IDs.
- Treat libetpan's batch primitives as the canonical wrapper target:
  `mailactivesync_mark_messages_read`,
  `mailactivesync_set_messages_flagged`, and
  `mailactivesync_delete_messages`.
- Keep any single-message MailCore methods as convenience wrappers over the
  batch APIs, not as the primary implementation path.
- Return the `ActiveSyncSyncResult` from these operations so callers can capture
  the updated sync key and server status.
- Add Objective-C bridge methods.
- Keep `deletesAsMoves` as sync configuration, not the public delete API.

### Phase 4: Move Items

Goal: support server-side message moves as a dedicated unit of work.

- Wire `mailactivesync_move_items` through `ActiveSyncSession`.
- Use the existing `MCActiveSyncMove` model where it fits.
- Add result/response wrappers for source IDs, destination IDs, new message ID,
  and per-item status.
- Add Objective-C bridge methods.

### Phase 5: Fetch Primitives

Goal: complete retrieval support before expanding sync complexity.

- Add body-part fetch support around `mailactivesync_item_operations_fetch_body_part`.
- Add attachment fetch support around
  `mailactivesync_item_operations_fetch_attachment`.
- Add multi-fetch support around `mailactivesync_item_operations_fetch_multi`
  if the result shape remains small enough for this phase.
- Add wrappers for attachment data and any body-part fields not represented by
  `ActiveSyncBody`.

### Phase 6: Sync Request and Result Completeness

Goal: expose low-level sync features that callers need for robust clients.

- Extend `ActiveSyncSyncRequest` for wait, heartbeat interval, conversation
  mode, rights management support, multiple body preferences, all-or-none body
  preference, supported properties, and client commands if we choose to expose
  the command mechanism directly.
- Extend `ActiveSyncSyncResult` for collection ID, limit, command responses,
  nested collection results, and collection lookup helpers.
- Keep this as its own commit because it changes core data shape.

### Phase 7: Multi-Collection Operations

Goal: support efficient multi-folder clients once result nesting is modeled.

- Add `syncMulti`.
- Add `itemEstimateMulti`.
- Add collection-specific result lookup helpers where useful.
- Bridge the same surface to Objective-C.

### Phase 8: Search and Find

Goal: expose server-side discovery without mixing it into sync commits.

- Add request/result models for `mailSearch`.
- Add request/result models for `mailFind`.
- Include result item metadata such as collection ID, server ID, long ID,
  previews, and total/range fields.
- Bridge to Objective-C after the C++ shape is stable.

### Phase 9: Settings, Recipients, and Certificate Utilities

Goal: expose account/support commands independently from mail sync.

- Add wrappers for getting user information and setting device information.
- Add wrappers for resolve recipients.
- Add wrappers for validate cert.
- Preserve mail-focused naming in public APIs where possible, but keep protocol
  details available in typed request/result objects.

### Phase 10: Draft and Extended Compose

Goal: complete write-side mail support after mutation and result plumbing exists.

- Add wrappers for add draft and update draft.
- Add extended compose request wrappers for send, smart reply, and smart
  forward.
- Keep the simple existing send/reply/forward methods as conveniences over the
  extended request form.
- Add tests carefully, with live send tests gated behind explicit environment
  variables.

### Phase 11: Tests and Samples

Goal: verify each wrapper layer without making earlier commits too large.

- Add or update unit/conversion tests with each phase when the result modeling
  is deterministic.
- Add live smoke coverage after API shape settles.
- Prefer small protocol-focused tests over broad end-to-end tests until the
  wrapper surface is complete.

Scope:

- libetpan ActiveSync low-level implementation in `/home/dvh/Sources/libetpan/src/low-level/activesync`
- MailCore ActiveSync C++ and Objective-C APIs in `src/core/activesync` and `src/objc/activesync`
- Microsoft Exchange ActiveSync command and email-class specs, current published docs checked on 2026-08-02

## Spec Baseline

The ActiveSync command set is broader than the current libetpan/MailCore surface. The relevant Microsoft docs describe these major client capabilities:

- Bootstrap and state: `OPTIONS`, `Provision`, `Settings`, `FolderSync`, `Sync`, `GetItemEstimate`
- Item retrieval: `ItemOperations`, legacy `GetAttachment`
- Item mutations through `Sync` `Commands`: `Add`, `Change`, `Delete`, `Fetch`
- Message state updates through `Sync` `Change`: `email:Read`, `email:Flag`, `email:Categories`
- Message deletion through `Sync` `Delete`
- Server-side message moves through `MoveItems`
- Outbound mail: `SendMail`, `SmartReply`, `SmartForward`
- Long polling / change notification: `Ping`
- Folder mutations: `FolderCreate`, `FolderUpdate`, `FolderDelete`
- Search and discovery: `Search`, `Find`, `ResolveRecipients`
- Calendar workflow: `MeetingResponse`
- S/MIME support: `ResolveRecipients`, `ValidateCert`

Important email add caveat:

- ActiveSync is not a general IMAP-style append API for arbitrary RFC822 mail into any folder.
- `Sync` `Add` can create objects in a collection, but the spec rejects client-created non-draft email items. Draft email synchronization is supported only in protocol versions `16.0` and `16.1`.
- `email:MIMEData` is documented as server-to-client response data, not as a generic upload field.

References:

- MS-ASCMD overview: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/feee082b-b66e-46d3-9883-f8390692d893
- MS-ASCMD command reference: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/1a3490f1-afe1-418a-aa92-6f630036d65a
- MS-ASCMD Sync `Commands`: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/5a54d46c-823d-44ff-8a86-91ba4d4af77f
- MS-ASCMD `Add` in `Sync`: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/22628ffe-b14a-4300-aec7-187b0c37a1dc
- MS-ASEMAIL `Sync` request: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-asemail/a8602ea5-a3f3-4426-83b5-a8d5315a953d
- MS-ASCMD `SendMail`: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/172db9ec-350f-4bb1-94b1-0526f976ed5e
- MS-ASCMD `MoveItems`: https://learn.microsoft.com/en-us/openspecs/exchange_server_protocols/ms-ascmd/92958692-b116-462f-871c-eaab66076da1

## Current libetpan State

Implemented enough to sync and fetch mail:

- Session object, URL normalization, basic/OAuth login state, device metadata, protocol version, policy key, and user agent.
- HTTP transport with curl.
- `OPTIONS`, including redirect/authenticate metadata handling.
- `FolderSync`.
- `Sync` for server-to-client changes.
- Sync options: collection class, get changes, deletes-as-moves, filter type, conflict, window size, body preferences, MIME body preference.
- `Provision`.
- `Settings` device information set.
- `GetItemEstimate`.
- `ItemOperations` fetch.
- Batch message mutation helpers for read/unread, flag/unflag, and delete:
  `mailactivesync_mark_messages_read`,
  `mailactivesync_set_messages_flagged`, and
  `mailactivesync_delete_messages`.
- WBXML encode/decode infrastructure and partial code page coverage.
- Message parsing for core email fields, read/flagged state, body, attachments, and MIME returned by server.
- Live test infrastructure in `tests/activesync-live-sync-test.sh`.

Stubbed public APIs:

- `mailactivesync_send_mail()` validates session/input and returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- `mailactivesync_smart_reply()` validates session/input and returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- `mailactivesync_smart_forward()` validates session/input and returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- `mailactivesync_move_items()` validates session/input and returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- `mailactivesync_ping()` validates session/input and returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.

Missing or under-modeled APIs:

- No public builders for `Sync` client commands.
- `mailactivesync_sync_request` has `client_commands`, but there is no exposed type/API for `Add`, `Change`, `Delete`, or `Fetch` commands.
- No single-message read/flag/delete helpers by design; callers should use the
  batch helpers, and higher-level wrappers may provide singular convenience
  methods over one-message arrays.
- No implemented path to add draft emails.
- No folder create/update/delete API.
- No `Search` or `Find` API.
- No `ResolveRecipients` or `ValidateCert` API.
- No `MeetingResponse` API.
- No direct attachment fetch API other than data surfaced through supported item/body fetch paths.
- No high-level response modeling for per-command mutation responses such as client ID to server ID mapping, per-item status, and new server ID after move.

Poorly implemented / risky areas:

- Outbound method signatures accept MIME data but do not expose all protocol fields needed for modern protocol versions, such as `ClientId`, `AccountId`, `SaveInSentItems`, `Mime`, `ReplaceMime`, and source metadata for smart reply/forward.
- `MoveItems` has data structures but no request encoder, POST call, response parser, or status conversion.
- `Ping` has request/result structures but no request encoder, POST call, response parser, heartbeat bounds handling, or folder class metadata.
- `Sync` request exposes `deletes_as_moves`, which can be mistaken for a delete operation. It should be documented and paired with real delete APIs.
- Status coverage is partial. Several command-specific statuses are not mapped to typed result/error surfaces.
- Test coverage is weighted toward successful live sync/fetch. Mutation commands need mock transport tests before live tests.

## Current MailCore State

Implemented wrapper APIs:

- C++ and Objective-C session setup: server URL, username, password, OAuth2 token, device ID.
- `connect`, `login`, `loginOAuth2`, `setOAuth2TokenOnConnection`.
- `options`.
- `folderSync`.
- `sync` and `syncMessages`.
- `provision`.
- `itemEstimate`.
- `fetchMessage`.
- `sendMessage`, `smartReply`, `smartForward`, and `ping` are exposed but depend on libetpan stubs.
- Message models expose received read/flagged state and message/body/attachment data.
- Live C++ sync test exists in `tests/test-activesync-live.cpp` and `tests/mailcore-activesync-live-test.sh`.

Missing or poorly exposed APIs:

- No C++ or Objective-C API to mark a message read/unread.
- No C++ or Objective-C API to flag/unflag a message or update flag metadata.
- No C++ or Objective-C API to delete a message.
- No C++ or Objective-C API to move messages, even though `MCActiveSyncMove` exists.
- No C++ or Objective-C API to add or update a draft email through `Sync` `Add`/`Change`.
- No C++ or Objective-C folder mutation APIs.
- No C++ or Objective-C search/find APIs.
- No C++ or Objective-C resolve-recipient, certificate validation, or meeting response APIs.
- `ActiveSyncSyncRequest` does not expose client commands, so callers cannot use the general `Sync` mutation mechanism.
- `sendMessage`, `smartReply`, `smartForward`, and `ping` look available but currently return not-implemented through libetpan. This is a usability problem because the public API appears more complete than it is.
- Objective-C headers mirror the limited C++ surface and inherit the same gaps.

## Implementation Plan

### Phase 1: Make Existing API Truthful

Goal: avoid misleading users and make unsupported operations obvious.

libetpan:

- Keep existing stubs until implemented, but document that send/reply/forward/move/ping currently return `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- Add command-level unit tests asserting each stub returns `MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED`.
- Add README status matrix for implemented/stubbed/missing commands.

MailCore:

- Document that `sendMessage`, `smartReply`, `smartForward`, and `ping` are currently wired to libetpan stubs.
- Consider guarding those APIs in higher-level docs or marking them experimental until implemented.
- Add tests confirming MailCore maps libetpan not-implemented into the expected MailCore error.

### Phase 2: Implement Mail Mutations Through `Sync`

Goal: support core mailbox operations used by real clients.

libetpan:

- Add public client command types:
  - `mailactivesync_sync_command_add`
  - `mailactivesync_sync_command_change`
  - `mailactivesync_sync_command_delete`
  - optional `mailactivesync_sync_command_fetch`
- Add constructors/free functions and request attachment helpers.
- Implement WBXML encoding under `Sync/Collections/Collection/Commands`.
- Implement response parsing under `Sync/Collections/Collection/Responses`.
- Implement typed batch helpers:
  - `mailactivesync_mark_messages_read(session, collection_id, sync_key, server_ids, read, result)`
  - `mailactivesync_set_messages_flagged(...)`
  - `mailactivesync_delete_messages(...)`
- Do not reintroduce single-message libetpan helpers; single-message behavior
  belongs in higher-level wrappers as convenience calls over the batch helpers.
- Preserve the general lower-level command mechanism so future content classes can use it.

MailCore:

- Add C++ batch APIs:
  - `markMessagesRead(folderID, syncKey, messageIDs, read, error)`
  - `setMessagesFlagged(folderID, syncKey, messageIDs, flagged, error)`
  - `deleteMessages(folderID, syncKey, messageIDs, deletesAsMoves, error)`
- Optionally keep singular MailCore convenience methods that wrap one-message
  arrays, but do not make them the only public mutation surface.
- Add Objective-C equivalents.
- Return updated sync key and per-item status where the protocol provides it.
- Avoid exposing raw `deletesAsMoves` as the delete API.

Validation:

- Mock transport tests for exact WBXML structure and response parsing.
- Live Outlook test with a test message:
  - fetch current read state
  - mark unread
  - sync and verify changed item
  - mark read
  - delete or move to Deleted Items
  - sync source and destination folders

### Phase 3: Implement `MoveItems`

Goal: support moving messages between server folders.

libetpan:

- Implement `mailactivesync_move_items()` request encoding using the Move namespace.
- Parse per-move status, source message ID, destination message ID, source folder ID, and destination folder ID.
- Map command-specific statuses to libetpan errors without losing per-item results.

MailCore:

- Wire `MCActiveSyncMove` to `ActiveSyncSession`.
- Add C++ `moveMessages(Array * moves, ErrorCode * error)` or simpler single-message and batch APIs.
- Add Objective-C equivalents.

Validation:

- Mock WBXML tests.
- Live test moving one test message from Inbox to a temporary folder and back, verifying the server may assign a new server ID.

### Phase 4: Implement Outbound Mail

Goal: make the currently exposed send APIs real.

libetpan:

- Implement `SendMail`.
- Implement `SmartReply`.
- Implement `SmartForward`.
- For protocol 14.0+, encode ComposeMail WBXML with `ClientId`, `SaveInSentItems`, and `Mime`.
- Decide whether to support protocol 2.5/12.x raw `message/rfc822` body mode or explicitly require 14.0+.
- Expose `ClientId` control or generate deterministic unique IDs.
- Parse failure response statuses.

MailCore:

- Keep the existing `sendMessage`, `smartReply`, and `smartForward` APIs, but ensure they work.
- Consider adding overloads/options for client ID, account ID, and replace-MIME behavior.

Validation:

- Mock protocol-version tests for body format.
- Live test sending to self with a unique subject and verifying the message arrives through `Sync`.
- Live smart reply/forward tests guarded behind explicit environment flags to avoid accidental sends.

### Phase 5: Implement `Ping`

Goal: provide efficient change notification.

libetpan:

- Encode Ping request with heartbeat interval and watched folders.
- Parse status and changed folder IDs.
- Handle server-requested heartbeat corrections and folder limit statuses.

MailCore:

- Existing C++ and Objective-C `ping` APIs can remain, but should expose status details clearly.
- Add documentation for heartbeat range and server throttling behavior.

Validation:

- Mock status tests.
- Live smoke test with short heartbeat, then mutate mailbox through another path and verify changed collection IDs.

### Phase 6: Folder Mutations

Goal: support creating, renaming/moving, and deleting folders.

libetpan:

- Add `FolderCreate`, `FolderUpdate`, and `FolderDelete`.
- Model returned sync key, status, and server ID.

MailCore:

- Add C++ and Objective-C folder create/update/delete methods.
- Make folder sync key lifecycle clear in API docs.

Validation:

- Live test creates a temporary mail folder, renames it, deletes it, and verifies `FolderSync`.

### Phase 7: Search, Recipients, Calendar, and S/MIME

Goal: complete important secondary ActiveSync workflows.

Prioritize based on product need:

- `Search` / `Find` for server-side lookup.
- `ResolveRecipients` for recipient validation and S/MIME certificate discovery.
- `MeetingResponse` for accepting/declining meeting requests.
- `ValidateCert` if S/MIME support is in scope.

Validation:

- Start with mock transport tests.
- Add live tests only for low-risk read-only operations first.

### Phase 8: Draft Email Support

Goal: support the only spec-valid form of client-created email item through folder sync.

libetpan:

- Implement `Sync` `Add` / `Change` for draft email only on protocol `16.0`/`16.1`.
- Model allowed draft fields and body forms.
- Explicitly reject arbitrary non-draft RFC822 append at API boundaries.

MailCore:

- Add draft-specific APIs rather than a generic append API:
  - `createDraft(folderID, syncKey, draft, error)`
  - `updateDraft(folderID, syncKey, messageID, draft, error)`
  - optional `sendDraft(...)` if supported through `email2:Send`

Validation:

- Live test creates a draft in Drafts, verifies it appears, updates it, then deletes it.
- Do not implement or advertise generic message import unless backed by another protocol.

## Recommended Priority Order

1. Truthful API/docs and not-implemented tests.
2. `Sync` client `Change` and `Delete` for read/unread, flag, and delete.
3. `MoveItems`.
4. `Ping`.
5. `SendMail`.
6. `SmartReply` and `SmartForward`.
7. Folder create/update/delete.
8. Draft add/update.
9. Search/find, meeting response, resolve recipients, validate cert.

Reasoning:

- Read/unread, delete, move, and flag are core mailbox operations and use existing sync state.
- `MoveItems`, `Ping`, and outbound mail already have public API placeholders, so implementing them closes misleading surfaces.
- Folder and draft support require more API design because sync keys and protocol restrictions matter.
- Search/calendar/S/MIME are useful but less central to validating the current mail sync path.

## Test Strategy

Unit/mock tests:

- Use the libetpan HTTP transport injection point to test exact command URLs, headers, request WBXML, response parsing, and error mapping without network.
- Add WBXML round-trip tests for each new command body.
- Add MailCore wrapper tests that verify arguments are passed to libetpan and errors/results map correctly.

Live tests:

- Keep live tests opt-in and config-driven.
- Reuse existing OAuth/config conventions from the libetpan and MailCore live sync tests.
- Use a dedicated test folder and subject prefix.
- Prefer reversible operations:
  - create test message
  - mark unread/read
  - flag/unflag
  - move to test folder and back
  - delete only generated test messages
- Persist sync keys in a local state file, but support `--reset-state` or a fresh state path for from-scratch validation.

Safety:

- Never run destructive live tests against arbitrary messages.
- Require explicit flags for send/reply/forward tests.
- Log message IDs and folder IDs used by live tests.
- Keep OAuth tokens and local configs ignored by git.

## Completion Criteria

The ActiveSync stack should be considered production-usable for mail when:

- Initial `FolderSync` from sync key `0` works.
- Initial and incremental `Sync` work and drain `MoreAvailable`.
- Full message fetch works.
- Read/unread, flag/unflag, delete, and move work and update sync keys correctly.
- `Ping` detects changes.
- Send mail works with modern protocol versions.
- Public MailCore C++ and Objective-C APIs expose only working operations or clearly mark unsupported ones.
- Every mutation has mock protocol tests and at least one guarded live Outlook test.
