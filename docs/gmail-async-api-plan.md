# Gmail Async API Plan

Date: 2026-09-23

Status: implemented in `src/async/gmail`.

Update 2026-09-26: the implemented API now keeps operation queue wiring and
operation configuration internal. Callers should obtain configured operations
from `GmailAsyncSession` factory methods, start them, and inspect result/error
accessors. `GmailAsyncSession::runOperation()`, `GmailAsyncSession::session()`,
operation input setters, and operation kind enums are not public caller API.

## Goal

Add an async C++ Gmail API on top of the synchronous Gmail API in
`src/core/gmail`.

The async API should feel familiar to existing MailCore users, but it should not
copy IMAP's connection-pool complexity. Gmail's low-level API is HTTP/OAuth and
stateless per request, so the async layer should be closer to POP/ActiveSync:

- one async session
- one owned synchronous `GmailSession`
- one `OperationQueue`
- one operation subclass per synchronous API call

## Existing Async Patterns

### IMAP

IMAP async has the same broad operation pattern, but it is much more complex:

- `IMAPAsyncSession`
- `IMAPOperation`
- many operation subclasses
- folder-bound `IMAPAsyncConnection` instances
- multiple connections
- urgent operations
- IDLE support
- progress callbacks
- server capability/configuration state

Gmail should not copy those IMAP-specific pieces.

### POP / ActiveSync

POP and ActiveSync are better templates:

- async session owns one synchronous session
- async session owns an `OperationQueue`
- operation `start()` enqueues through internal async-session wiring
- operation `main()` calls a sync method through a protected operation helper
- operation stores an `ErrorCode` and typed result

Use this pattern for Gmail.

## Directory Layout

Add a new async Gmail module:

```text
src/async/gmail/
  MCAsyncGmail.h
  MCGmailAsyncSession.h
  MCGmailAsyncSession.cpp
  MCGmailOperation.h
  MCGmailOperation.cpp
  MCGmailOperations.h
  MCGmailProfileOperation.h
  MCGmailProfileOperation.cpp
  MCGmailLabelsOperation.h
  MCGmailLabelsOperation.cpp
  MCGmailLabelOperation.h
  MCGmailLabelOperation.cpp
  MCGmailMessagesOperation.h
  MCGmailMessagesOperation.cpp
  MCGmailMessageOperation.h
  MCGmailMessageOperation.cpp
  MCGmailMessageDataOperation.h
  MCGmailMessageDataOperation.cpp
  MCGmailAttachmentDataOperation.h
  MCGmailAttachmentDataOperation.cpp
  MCGmailMessagePartDataOperation.h
  MCGmailMessagePartDataOperation.cpp
```

Optional later:

```text
src/async/gmail/MCGmailOperationCallback.h
```

Do not add a callback class in the first pass unless Gmail gains progress
callbacks. The current read-only HTTP API can use `Operation` completion/result
inspection without progress hooks.

## Public Umbrella

Add `src/async/gmail/MCAsyncGmail.h`:

```cpp
#ifndef MAILCORE_MCASYNCGMAIL_H
#define MAILCORE_MCASYNCGMAIL_H

#include <MailCore/MCGmailAsyncSession.h>
#include <MailCore/MCGmailOperation.h>
#include <MailCore/MCGmailOperations.h>

#endif
```

`MCGmailOperations.h` is a compatibility umbrella for the concrete operation
headers. Each operation class is declared and implemented in its own `.h` and
`.cpp`, matching the async IMAP file layout.

Include it from the existing async umbrella if the project expects protocol
async umbrellas there.

## GmailAsyncSession

Add `src/async/gmail/MCGmailAsyncSession.h`.

```cpp
namespace mailcore {

class GmailSession;
class GmailOperation;
class GmailProfileOperation;
class GmailLabelsOperation;
class GmailLabelOperation;
class GmailMessagesOperation;
class GmailMessageOperation;
class GmailMessageDataOperation;
class GmailAttachmentDataOperation;
class GmailMessagePartDataOperation;

class MAILCORE_EXPORT GmailAsyncSession : public Object {
public:
    GmailAsyncSession();
    virtual ~GmailAsyncSession();

    virtual void setUserID(String * userID);
    virtual String * userID();

    virtual void setOAuth2Token(String * token);
    virtual String * OAuth2Token();

    virtual void setUserAgent(String * userAgent);
    virtual String * userAgent();

    virtual void setTimeout(time_t timeout);
    virtual time_t timeout();

    virtual int lastHTTPStatus();
    virtual String * lastErrorMessage();

#ifdef __APPLE__
    virtual void setDispatchQueue(dispatch_queue_t dispatchQueue);
    virtual dispatch_queue_t dispatchQueue();
#endif

    virtual void setOperationQueueCallback(OperationQueueCallback * callback);
    virtual OperationQueueCallback * operationQueueCallback();
    virtual bool isOperationQueueRunning();
    virtual void cancelAllOperations();

    virtual GmailProfileOperation * profileOperation();
    virtual GmailLabelsOperation * labelsOperation();
    virtual GmailLabelOperation * labelOperation(String * labelID);

    virtual GmailMessagesOperation * messagesOperation();
    virtual GmailMessagesOperation * messagesWithQueryOperation(String * query);
    virtual GmailMessagesOperation * messagesWithLabelOperation(String * labelID);

    virtual GmailMessageOperation * messageOperation(String * messageID);
    virtual GmailMessageOperation * messageWithFormatOperation(String * messageID,
                                                               GmailMessageFormat format);
    virtual GmailMessageOperation * messageWithMetadataHeadersOperation(String * messageID,
                                                                        Array * headers);

    virtual GmailMessageDataOperation * messageDataOperation(String * messageID);
    virtual GmailAttachmentDataOperation * attachmentDataOperation(String * messageID,
                                                                   String * attachmentID);
    virtual GmailMessagePartDataOperation * dataForMessagePartOperation(String * messageID,
                                                                        GmailMessagePart * part);

private:
    virtual void runOperation(GmailOperation * operation);
    virtual GmailSession * session();

    GmailSession * mSession;
    OperationQueue * mQueue;
    OperationQueueCallback * mOperationQueueCallback;
};

}
```

Implementation notes:

- Constructor creates and retains one `GmailSession`.
- Constructor creates and retains one `OperationQueue`.
- Configuration methods delegate to `mSession`.
- `lastHTTPStatus()` and `lastErrorMessage()` delegate to `mSession`.
- Operation factory methods allocate the operation, set its async session, set
  inputs, autorelease, and return it.
- `runOperation()` adds the operation to the queue and is internal-only.
- `cancelAllOperations()` cancels the queue.
- `isOperationQueueRunning()` mirrors POP/ActiveSync behavior.
- On Apple, operation callback dispatch queue should be passed through in
  `GmailOperation::setSession()`.

## GmailOperation

Add `src/async/gmail/MCGmailOperation.h`.

```cpp
namespace mailcore {

class GmailAsyncSession;
class GmailSession;

class MAILCORE_EXPORT GmailOperation : public Operation {
    friend class GmailAsyncSession;

public:
    GmailOperation();
    virtual ~GmailOperation();

    virtual ErrorCode error();

    virtual void start();

protected:
    virtual GmailAsyncSession * session();
    virtual GmailSession * syncSession();
    virtual void setError(ErrorCode error);

private:
    virtual void setSession(GmailAsyncSession * session);

    GmailAsyncSession * mSession;
    ErrorCode mError;
};

}
```

Implementation should mirror `ActiveSyncOperation`:

- Retain/release async session.
- Store `ErrorCode`.
- `start()` calls the internal async-session queue hook.
- Operation subclasses call synchronous Gmail methods through `syncSession()`,
  not through a public `GmailAsyncSession::session()` accessor.
- On Apple, set callback dispatch queue from `GmailAsyncSession`.

No progress callback is needed in the first pass.

## Operation Subclasses

Add `src/async/gmail/MCGmailOperations.h` as an umbrella and one `.h`/`.cpp`
pair per concrete operation.

### GmailProfileOperation

Sync call:

```cpp
GmailProfile * GmailSession::profile(ErrorCode * pError);
```

Result:

```cpp
GmailProfile * profile();
```

### GmailLabelsOperation

Sync call:

```cpp
Array * GmailSession::labels(ErrorCode * pError);
```

Result:

```cpp
Array * /* GmailLabel */ labels();
```

### GmailLabelOperation

Input:

```cpp
String * labelID;
```

Sync call:

```cpp
GmailLabel * GmailSession::label(String * labelID, ErrorCode * pError);
```

Result:

```cpp
GmailLabel * label();
```

### GmailMessagesOperation

Use one operation class for all list-message variants.

Internal inputs:

```cpp
enum GmailMessagesOperationKind {
    GmailMessagesOperationKindDefault,
    GmailMessagesOperationKindQuery,
    GmailMessagesOperationKindLabel,
};

String * query;
String * labelID;
```

Sync calls:

```cpp
GmailMessageList * messages(ErrorCode * pError);
GmailMessageList * messagesWithQuery(String * query, ErrorCode * pError);
GmailMessageList * messagesWithLabel(String * labelID, ErrorCode * pError);
```

Result:

```cpp
GmailMessageList * messages();
```

The kind enum and input setters are internal. Public callers use
`GmailAsyncSession::messagesOperation()`,
`GmailAsyncSession::messagesWithQueryOperation()`, and
`GmailAsyncSession::messagesWithLabelOperation()`.

### GmailMessageOperation

Use one operation class for all message-get variants.

Internal inputs:

```cpp
enum GmailMessageOperationKind {
    GmailMessageOperationKindDefault,
    GmailMessageOperationKindFormat,
    GmailMessageOperationKindMetadataHeaders,
};

String * messageID;
GmailMessageFormat format;
Array * /* String */ metadataHeaders;
```

The kind enum and input setters are internal. Public callers use
`GmailAsyncSession::messageOperation()`,
`GmailAsyncSession::messageWithFormatOperation()`, and
`GmailAsyncSession::messageWithMetadataHeadersOperation()`.

Sync calls:

```cpp
GmailMessage * message(String * messageID, ErrorCode * pError);
GmailMessage * messageWithFormat(String * messageID,
                                 GmailMessageFormat format,
                                 ErrorCode * pError);
GmailMessage * messageWithMetadataHeaders(String * messageID,
                                          Array * headers,
                                          ErrorCode * pError);
```

Result:

```cpp
GmailMessage * message();
```

### GmailMessageDataOperation

Input:

```cpp
String * messageID;
```

Sync call:

```cpp
Data * GmailSession::messageData(String * messageID, ErrorCode * pError);
```

Result:

```cpp
Data * data();
```

### GmailAttachmentDataOperation

Inputs:

```cpp
String * messageID;
String * attachmentID;
```

Sync call:

```cpp
Data * GmailSession::attachmentData(String * messageID,
                                    String * attachmentID,
                                    ErrorCode * pError);
```

Result:

```cpp
Data * data();
```

### GmailMessagePartDataOperation

Inputs:

```cpp
String * messageID;
GmailMessagePart * part;
```

Sync call:

```cpp
Data * GmailSession::dataForMessagePart(String * messageID,
                                        GmailMessagePart * part,
                                        ErrorCode * pError);
```

Result:

```cpp
Data * data();
```

## Operation Implementation Rules

For every operation:

- Retain/copy inputs in setters.
- Release inputs and result in destructor.
- `main()` should call exactly one synchronous `GmailSession` method.
- Store returned result with `MC_SAFE_REPLACE_RETAIN`.
- Store `ErrorCode` with `setError(error)`.
- Return retained/autoreleased results from sync methods safely by retaining in
  the operation result field.
- Do not perform network work in constructors or setters.

## Build Wiring

Implemented in `src/cmake/async.cmake`:

```cmake
set(async_gmail_files
  async/gmail/MCGmailAsyncSession.cpp
  async/gmail/MCGmailOperation.cpp
  async/gmail/MCGmailProfileOperation.cpp
  async/gmail/MCGmailLabelsOperation.cpp
  async/gmail/MCGmailLabelOperation.cpp
  async/gmail/MCGmailMessagesOperation.cpp
  async/gmail/MCGmailMessageOperation.cpp
  async/gmail/MCGmailMessageDataOperation.cpp
  async/gmail/MCGmailAttachmentDataOperation.cpp
  async/gmail/MCGmailMessagePartDataOperation.cpp
)
```

Add `${async_gmail_files}` to `async_files`.

Add async include directory:

```cmake
"${CMAKE_CURRENT_SOURCE_DIR}/async/gmail"
```

Implemented in `src/cmake/public-headers.cmake`:

```cmake
async/gmail/MCAsyncGmail.h
async/gmail/MCGmailAsyncSession.h
async/gmail/MCGmailOperation.h
async/gmail/MCGmailOperations.h
async/gmail/MCGmailProfileOperation.h
async/gmail/MCGmailLabelsOperation.h
async/gmail/MCGmailLabelOperation.h
async/gmail/MCGmailMessagesOperation.h
async/gmail/MCGmailMessageOperation.h
async/gmail/MCGmailMessageDataOperation.h
async/gmail/MCGmailAttachmentDataOperation.h
async/gmail/MCGmailMessagePartDataOperation.h
```

Updated `src/async/MCAsync.h` to include `MCAsyncGmail.h`.

## Tests / Verification

Minimum verification:

```sh
cmake --build build --target MailCore -j2
```

Recommended focused tests:

- Create `GmailAsyncSession`, set token/user fields, create each operation, and
  verify operation inputs/results compile and retain safely.
- Mock/fake sync Gmail session only if the current test architecture makes that
  practical.
- Live Gmail OAuth tests should stay opt-in and out of the first async pass
  unless credentials are explicitly provided.

## Future Follow-Ups

- Objective-C async Gmail wrappers.
- Progress callbacks only if libetpan Gmail adds streaming/progress hooks.
- Public list options operation if the synchronous API later exposes public list
  options.
- Mutation operations if/when the core Gmail API grows write/mutate methods.
