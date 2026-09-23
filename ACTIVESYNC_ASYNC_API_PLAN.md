# ActiveSync Async API Plan

Date: 2026-09-23

## Goal

Add an asynchronous ActiveSync API that follows MailCore's existing async
session and operation model while reusing the synchronous ActiveSync
implementation as the protocol owner.

The async layer should not invent a separate ActiveSync transport. It should
wrap `ActiveSyncSession` methods in queued operations, just as IMAP, SMTP, POP,
and NNTP async sessions wrap their synchronous session implementations.

## Design Principles

- Keep `ActiveSyncSession` as the synchronous implementation and state owner.
- Add a protocol-specific `ActiveSyncAsyncSession` under `src/async/activesync`.
- Return operation objects from the async session rather than accepting raw
  callback blocks in the C++ API.
- Use `OperationQueue`, `OperationQueueCallback`, callback dispatch queues on
  Apple platforms, and cancellation conventions already used by the other async
  protocols.
- Keep result types identical to the synchronous ActiveSync API:
  `ActiveSyncOptions`, `ActiveSyncFolderSyncResult`,
  `ActiveSyncSyncResult`, `ActiveSyncMessage`, and related classes.
- Preserve the mail-focused public naming chosen for the ActiveSync wrapper:
  `folderID`, `messageID`, and `messageData` instead of protocol-only names
  where the API is mail-shaped.
- Treat long-running `ping` carefully because it may need transport-level
  cancellation support before it is comfortable as a normal queued operation.

## Phase 1: Pattern Audit

Confirm the details of the existing async pattern before adding files.

- Review `SMTPAsyncSession`, `POPAsyncSession`, `NNTPAsyncSession`, and
  `IMAPAsyncSession`.
- Review operation base classes such as `SMTPOperation`, `POPOperation`,
  `NNTPOperation`, and `IMAPOperation`.
- Confirm how callbacks, dispatch queues, operation queue callbacks, and
  cancellation are exposed in C++ and Objective-C.
- Record any ActiveSync-specific differences, especially around persistent
  session state, policy keys, redirects, authentication headers, and ping.

## Phase 2: C++ Async Session Skeleton

Add `ActiveSyncAsyncSession`.

Expected files:

- `src/async/activesync/MCActiveSyncAsyncSession.h`
- `src/async/activesync/MCActiveSyncAsyncSession.cpp`
- `src/async/activesync/MCAsyncActiveSync.h`

Core members:

- `ActiveSyncSession * mSession`
- `OperationQueue * mQueue`
- `ActiveSyncOperationQueueCallback * mQueueCallback`
- `OperationQueueCallback * mOperationQueueCallback`

Session configuration proxies:

- `setServerURL` / `serverURL`
- `setUsername` / `username`
- `setPassword` / `password`
- `setOAuth2Token` / `OAuth2Token`
- `setDeviceID` / `deviceID`
- `lastRedirectURL`
- `lastAuthenticateHeader`

Queue methods:

- `setOperationQueueCallback`
- `operationQueueCallback`
- `isOperationQueueRunning`
- `cancelAllOperations`
- `runOperation`
- `session`

The queue callback should retain the async session while operations are
running and forward `queueStartRunning` / `queueStoppedRunning` to the external
callback, matching the SMTP/POP/NNTP pattern.

## Phase 3: Base ActiveSync Operation

Add a base operation class for all ActiveSync async operations.

Expected files:

- `src/async/activesync/MCActiveSyncOperation.h`
- `src/async/activesync/MCActiveSyncOperation.cpp`

Base responsibilities:

- Store an `ActiveSyncAsyncSession *`.
- Store the final `ErrorCode`.
- Expose `error()`.
- Expose `setActiveSyncCallback` / `activeSyncCallback` if the C++ API needs a
  protocol-specific callback in addition to the generic operation callback.
- In `start()`, enqueue itself through the owning async session.
- In `main()`, allow subclasses to call the synchronous `ActiveSyncSession`.
- Notify callbacks through the existing `Operation` callback mechanism.

This base class should stay small. Result ownership should live in typed
subclasses.

## Phase 4: First Operation Set

Implement the low-risk operations first.

Session/auth operations:

- `connectOperation`
- `loginOperation`
- `loginOAuth2Operation`
- `setOAuth2TokenOnConnectionOperation`

Discovery/state operations:

- `optionsOperation`
- `provisionOperation`
- `folderSyncOperation`
- `folderResyncOperation`
- `itemEstimateOperation`

Each result-bearing operation should expose a typed getter:

- `OptionsOperation::options()`
- `FolderSyncOperation::result()`
- `ProvisionOperation::result()`
- `ItemEstimateOperation::result()`

This phase is the best first commit because it proves the async scaffolding
without touching message mutation or compose side effects.

## Phase 5: Sync and Fetch Operations

Add read-side mail operations.

- `syncOperation(ActiveSyncSyncRequest * request)`
- `syncMessagesOperation(String * folderID, String * syncKey)`
- `fetchMessageOperation(String * folderID, String * messageID)`
- `fetchMessageBodyPartOperation(String * folderID, String * messageID,
  ActiveSyncBodyType bodyType, uint32_t truncationSize)`
- `fetchAttachmentOperation(String * fileReference, String * range)`

Result getters:

- `syncResult()`
- `message()`
- `attachmentData()`

Use the synchronous API's exact result classes and conversion behavior.

## Phase 6: Mutation Operations

Add state-changing mailbox operations.

- `folderCreateOperation`
- `folderUpdateOperation`
- `folderDeleteOperation`
- `markMessagesReadOperation`
- `setMessagesFlaggedOperation`
- `deleteMessagesOperation`
- single-message convenience operations for read, flag, and delete
- `moveMessagesOperation`

Batch operations should remain the canonical implementation path. Single-item
operations should build one-element arrays and delegate to the batch operation
logic, matching the synchronous wrapper design.

## Phase 7: Compose and Ping Operations

Add operations for commands with higher side-effect or lifecycle risk.

- `sendMessageOperation`
- `smartReplyOperation`
- `smartForwardOperation`
- `pingOperation`

Important caveats:

- Keep send/reply/forward tests opt-in because they modify real mailboxes.
- Verify whether the backing libetpan implementations are complete before
  exposing these as production-ready.
- Treat `pingOperation` as cancellable only if the transport layer can actually
  interrupt the in-flight request. Otherwise, document that cancellation only
  affects queued pings.

## Phase 8: Objective-C Async API

Expose the async API to Objective-C after the C++ shape is stable.

Two viable shapes:

- Add async operation factories directly to `MCOActiveSyncSession`.
- Add a separate `MCOActiveSyncAsyncSession` to keep the synchronous Objective-C
  wrapper clean.

Preferred starting point: separate `MCOActiveSyncAsyncSession`, because the
current `MCOActiveSyncSession` is purely synchronous and adding operation
factories would make its role less clear.

Expected Objective-C classes:

- `MCOActiveSyncOperation`
- `MCOActiveSyncOptionsOperation`
- `MCOActiveSyncFolderSyncOperation`
- `MCOActiveSyncFolderMutationOperation`
- `MCOActiveSyncSyncOperation`
- `MCOActiveSyncProvisionOperation`
- `MCOActiveSyncItemEstimateOperation`
- `MCOActiveSyncFetchMessageOperation`
- `MCOActiveSyncFetchAttachmentOperation`
- `MCOActiveSyncMoveOperation`
- `MCOActiveSyncSendOperation`
- `MCOActiveSyncPingOperation`

Completion blocks should follow existing MailCore Objective-C operation style:
`NSError *` first, then the typed result when applicable.

## Phase 9: Build Integration

Wire the new files into all relevant build surfaces.

- Update `src/cmake/async.cmake`.
- Update umbrella headers for async ActiveSync.
- Update Xcode/project metadata if this repository still requires checked-in
  project file membership.
- Update package/podspec include lists if async headers are explicitly listed.
- Confirm generated install headers include the new async ActiveSync headers.

## Phase 10: Tests

Add focused tests in layers.

Unit or local tests:

- Operation factory methods return the expected subclass.
- Operation parameters are retained/copied correctly.
- Errors propagate from synchronous calls to async operation results.
- Queue callbacks fire start/stop around ActiveSync operations.
- Cancellation prevents queued operations from running.

Live smoke tests:

- OAuth login through async session.
- Options through async session.
- Folder sync through async session.
- Initial message sync through async session.
- Fetch one message through async session when a message is available.

Gated live tests:

- Message read/flag/delete mutations.
- Move messages.
- Send, smart reply, and smart forward.
- Ping with timeout/cancellation behavior.

## Suggested Commit Order

1. Add C++ async session, base operation, build wiring, and auth/options/folder
   sync operations.
2. Add sync, fetch, provision, and item estimate operations.
3. Add folder and message mutation operations.
4. Add compose, move, and ping operations once backing support is confirmed.
5. Add Objective-C async wrappers.
6. Add tests and documentation updates.

## Open Questions

- Should Objective-C reuse `MCOActiveSyncSession` with operation factories, or
  introduce `MCOActiveSyncAsyncSession`?
- Does libetpan ActiveSync expose a reliable in-flight cancellation primitive
  for long-running HTTP requests?
- Should async `ping` live in the normal operation queue, or should it use a
  dedicated queue/connection to avoid blocking unrelated operations?
- Which ActiveSync operations should be considered stable if the underlying
  libetpan function currently returns `NOT_IMPLEMENTED`?
- Should the async API expose lower-level `ActiveSyncSyncRequest` everywhere,
  or bias toward mail-shaped operation factories with the request object reserved
  for advanced callers?
