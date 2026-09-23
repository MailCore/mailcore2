# ActiveSync Objective-C Async API Plan

Date: 2026-09-23

## Goal

Expose ActiveSync to Objective-C as an asynchronous API backed by
`mailcore::ActiveSyncAsyncSession`.

The Objective-C public API should not expose synchronous `WithError:` methods.
`MCOActiveSyncSession` should behave like the existing asynchronous POP, SMTP,
IMAP, and NNTP session wrappers: callers configure the session, request an
operation object, then start the operation with a completion block.

## Design Principles

- Use `mailcore::ActiveSyncAsyncSession` as the only native session backing for
  `MCOActiveSyncSession`.
- Do not call `mailcore::ActiveSyncSession` directly from Objective-C public
  methods.
- Do not expose public synchronous Objective-C methods such as
  `optionsWithError:` or `folderSyncWithSyncKey:error:`.
- Keep the existing ActiveSync model/result Objective-C wrappers unchanged.
- Match existing MailCore operation conventions:
  - `MCOOperation` base behavior.
  - Session-retained operations.
  - Completion blocks on the configured dispatch queue.
  - `operationQueueRunningChangeBlock`.
  - `cancelAllOperations`.
- Preserve the mail-focused ActiveSync naming already used by the wrapper:
  `folderID`, `messageID`, and `messageData`.

## Phase 1: Replace Native Session Backing

Change `MCOActiveSyncSession` from a synchronous wrapper to an async wrapper.

Current backing:

- `mailcore::ActiveSyncSession * _session`

Target backing:

- `mailcore::ActiveSyncAsyncSession * _session`
- `MCOActiveSyncCallbackBridge * _callbackBridge`
- `MCOOperationQueueRunningChangeBlock _operationQueueRunningChangeBlock`

Keep configuration properties:

- `serverURL`
- `username`
- `password`
- `OAuth2Token`
- `deviceID`
- `lastRedirectURL`
- `lastAuthenticateHeader`

Add async session properties and methods:

- `dispatchQueue`
- `operationQueueRunning`
- `operationQueueRunningChangeBlock`
- `cancelAllOperations`

The callback bridge should implement `mailcore::OperationQueueCallback` and
call back into `MCOActiveSyncSession` when the native operation queue starts or
stops.

## Phase 2: Remove Public Sync Surface

Remove synchronous `WithError:` methods from `MCOActiveSyncSession.h`.

Methods to remove from the public Objective-C API include:

- `connectWithError:`
- `loginWithError:`
- `loginOAuth2WithError:`
- `setOAuth2TokenOnConnectionWithError:`
- `optionsWithError:`
- `folderSyncWithSyncKey:error:`
- `folderResyncWithError:`
- `folderCreateWithSyncKey:parentID:displayName:error:`
- `folderUpdateWithSyncKey:folderID:parentID:displayName:error:`
- `folderDeleteWithSyncKey:folderID:error:`
- `syncWithRequest:error:`
- `syncMessagesInFolderID:syncKey:error:`
- message mutation `error:` methods
- `moveMessages:error:`
- `provisionWithError:`
- `itemEstimateForCollectionID:syncKey:error:`
- `fetchMessageInFolderID:messageID:error:`
- `fetchMessageBodyPartInFolderID:messageID:bodyType:truncationSize:error:`
- `fetchAttachmentWithFileReference:range:error:`
- `sendMessageWithData:saveInSent:error:`
- `smartReplyInFolderID:messageID:messageData:saveInSent:error:`
- `smartForwardInFolderID:messageID:messageData:saveInSent:error:`
- `pingCollectionIDs:heartbeatInterval:error:`

Preferred behavior: remove these declarations and implementations rather than
keeping deprecated sync methods.

## Phase 3: Add Base Operation Wrapper

Add a generic Objective-C ActiveSync operation wrapper.

Expected files:

- `src/objc/activesync/MCOActiveSyncOperation.h`
- `src/objc/activesync/MCOActiveSyncOperation.mm`
- Optional: `src/objc/activesync/MCOActiveSyncOperation+Private.h`

Native backing:

- `mailcore::ActiveSyncOperation`

Public API:

```objc
- (void)start:(void (^)(NSError *error))completionBlock;
```

Responsibilities:

- Retain the parent `MCOActiveSyncSession`.
- Convert `mailcore::ErrorCode` to `NSError`.
- Release copied completion blocks after completion.
- Reuse `MCOOperation` lifecycle through `initWithMCOperation:`.

## Phase 4: Add Typed Result Operations

Add typed Objective-C operation subclasses where native operations return
result objects.

Expected classes:

- `MCOActiveSyncOptionsOperation`
- `MCOActiveSyncFolderSyncOperation`
- `MCOActiveSyncFolderMutationOperation`
- `MCOActiveSyncSyncOperation`
- `MCOActiveSyncProvisionOperation`
- `MCOActiveSyncItemEstimateOperation`
- `MCOActiveSyncFetchMessageOperation`
- `MCOActiveSyncFetchAttachmentOperation`
- `MCOActiveSyncMoveOperation`
- `MCOActiveSyncPingOperation`

Completion block shapes:

```objc
- (void)start:(void (^)(NSError *error, MCOActiveSyncOptions *options))completionBlock;
- (void)start:(void (^)(NSError *error, MCOActiveSyncFolderSyncResult *result))completionBlock;
- (void)start:(void (^)(NSError *error, MCOActiveSyncSyncResult *result))completionBlock;
- (void)start:(void (^)(NSError *error, MCOActiveSyncMessage *message))completionBlock;
- (void)start:(void (^)(NSError *error, MCOActiveSyncAttachmentData *data))completionBlock;
```

Each subclass should read from the corresponding native async operation getter:

- `ActiveSyncOptionsOperation::options()`
- `ActiveSyncFolderSyncOperation::result()`
- `ActiveSyncFolderResyncOperation::result()`
- folder mutation operation `result()`
- sync operation `result()`
- `ActiveSyncProvisionOperation::result()`
- `ActiveSyncItemEstimateOperation::result()`
- fetch message operation `message()`
- fetch attachment operation `attachmentData()`
- move operation `result()`
- ping operation `result()`

## Phase 5: Add Operation Factories To `MCOActiveSyncSession`

Expose operation factories only.

Void-result operations:

- `connectOperation`
- `loginOperation`
- `loginOAuth2Operation`
- `setOAuth2TokenOnConnectionOperation`
- `sendMessageOperationWithData:saveInSent:`
- `smartReplyOperationInFolderID:messageID:messageData:saveInSent:`
- `smartForwardOperationInFolderID:messageID:messageData:saveInSent:`

Result operations:

- `optionsOperation`
- `folderSyncOperationWithSyncKey:`
- `folderResyncOperation`
- `folderCreateOperationWithSyncKey:parentID:displayName:`
- `folderUpdateOperationWithSyncKey:folderID:parentID:displayName:`
- `folderDeleteOperationWithSyncKey:folderID:`
- `syncOperationWithRequest:`
- `syncMessagesOperationInFolderID:syncKey:`
- `markMessagesReadOperationInFolderID:syncKey:messageIDs:read:`
- `setMessagesFlaggedOperationInFolderID:syncKey:messageIDs:flagged:`
- `deleteMessagesOperationInFolderID:syncKey:messageIDs:deletesAsMoves:`
- `markMessageReadOperationInFolderID:syncKey:messageID:read:`
- `setMessageFlaggedOperationInFolderID:syncKey:messageID:flagged:`
- `deleteMessageOperationInFolderID:syncKey:messageID:deletesAsMoves:`
- `moveMessagesOperation:`
- `provisionOperation`
- `itemEstimateOperationForCollectionID:syncKey:`
- `fetchMessageOperationInFolderID:messageID:`
- `fetchMessageBodyPartOperationInFolderID:messageID:bodyType:truncationSize:`
- `fetchAttachmentOperationWithFileReference:range:`
- `pingOperationWithCollectionIDs:heartbeatInterval:`

## Phase 6: Bridge Operation Objects

Follow the existing POP/SMTP operation wrapping pattern.

`MCOActiveSyncSession` should have helpers like:

```objc
- (id)_objcOperationFromNativeOp:(mailcore::ActiveSyncOperation *)op;
- (id)_objcOpaqueOperationFromNativeOp:(mailcore::ActiveSyncOperation *)op;
```

Typed operations should use `MCO_TO_OBJC(op)` when the native class has a
registered Objective-C wrapper.

Void operations can use an opaque `MCOActiveSyncOperation` wrapper around the
native operation.

Every returned operation must retain the Objective-C session via
`setSession:`.

## Phase 7: Update Umbrella Headers And Build Metadata

Update:

- `src/objc/activesync/MCOActiveSync.h`
- `src/cmake/objc.cmake`
- `src/cmake/public-headers.cmake`

Public headers to add:

- `MCOActiveSyncOperation.h`
- every typed operation header added in phase 4

The existing ActiveSync model/result headers remain public.

## Phase 8: Tests And Verification

Compile verification:

- Build an Apple/Objective-C target.
- Ensure every operation factory compiles.
- Ensure every completion block has the expected ObjC result type.

Behavior verification:

- Use async operations in a small smoke test:
  - `loginOAuth2Operation`
  - `optionsOperation`
  - `folderSyncOperationWithSyncKey:`
  - `syncMessagesOperationInFolderID:syncKey:`

Gated live tests:

- message read/flag/delete mutations
- folder create/update/delete
- move messages
- send, smart reply, smart forward
- ping

## Suggested Commit Order

1. Add base `MCOActiveSyncOperation` and convert `MCOActiveSyncSession` to
   `ActiveSyncAsyncSession` with queue controls.
2. Replace sync public methods with operation factories for auth/options/folder
   sync/provision/item estimate.
3. Add sync/fetch result operations.
4. Add mutation/move/send/ping operations.
5. Update umbrella/build metadata and add compile tests.

## Open Questions

- Should sync Objective-C methods be fully removed, or kept temporarily as
  deprecated compatibility shims in a private/legacy category?
- Should `itemEstimateOperationForCollectionID:syncKey:` be renamed to
  `itemEstimateOperationForFolderID:syncKey:` for mail-focused consistency?
- Should `pingOperationWithCollectionIDs:heartbeatInterval:` use `folderIDs`
  in Objective-C even though the protocol term is collection IDs?
- Should send/reply/forward operation factories be public immediately if the
  backing libetpan functions can still return not implemented?
