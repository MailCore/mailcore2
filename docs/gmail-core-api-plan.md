# Gmail Core API Plan

Date: 2026-09-23

Update 2026-09-26: the implemented API now keeps the lazy session lifecycle
hooks internal. `setup()`, `unsetup()`, and `isSetup()` are implementation
details, not public caller API.

## Goal

Create a simple C++ Gmail API in `src/core/gmail` on top of libetpan's
low-level Gmail API in `/home/dvh/Sources/libetpan/src/low-level/gmail`.

The API should follow the style of the existing synchronous C++ wrappers in
`src/core/imap`, especially `IMAPSession`: a MailCore `Object`-based session,
typed MailCore result objects, `ErrorCode * pError` out parameters, and thin
conversion layers over libetpan-owned C structs.

This is a Gmail HTTP API wrapper, not an IMAP extension wrapper. It should be a
sibling module to `src/core/imap`, not new methods on `IMAPSession`.

## Scope

Initial API surface:

- Configure Gmail API access with user ID/email, OAuth2 token, user agent, and
  timeout.
- Fetch the authenticated Gmail profile.
- List labels.
- Get one label by ID.
- List message summaries with query, labels, pagination, and spam/trash option.
- Get one message in Gmail API formats: full, metadata, minimal, raw.
- Fetch an attachment by Gmail message ID and attachment ID.
- Return decoded message, body, and attachment bytes from Gmail base64url fields,
  similar to the way IMAP fetch APIs return usable decoded data.
- Provide MailCore-shaped conveniences for RFC 822 data and parsed
  `AbstractMessage` results.
- Expose last HTTP status and last Gmail/libetpan error message for diagnostics.

Out of scope for the first pass:

- Send, insert, import, trash, untrash, modify labels, batch modify, watch,
  history, threads, drafts, and settings.
- Objective-C wrapper surface.
- Async operation wrappers.
- Sharing implementation with `IMAPSession` Gmail label/ID extensions.
- Token refresh; callers provide a valid access token.
- Public request-builder classes for message list/get. The wrapper should build
  libetpan request structs internally from convenience method arguments.

## libetpan Baseline

Public low-level functions from `mailgmail.h`:

```c
mailgmail * mailgmail_new(void);
void mailgmail_free(mailgmail * session);

int mailgmail_set_http_transport(mailgmail * session,
    struct mailhttp_transport * transport);
int mailgmail_set_user(mailgmail * session, const char * user_id);
int mailgmail_set_oauth2_token(mailgmail * session,
    const char * access_token);
int mailgmail_set_user_agent(mailgmail * session,
    const char * user_agent);
int mailgmail_set_timeout(mailgmail * session, time_t timeout);

int mailgmail_get_last_http_status(mailgmail * session);
const char * mailgmail_get_last_error_message(mailgmail * session);

int mailgmail_get_profile(mailgmail * session,
    struct mailgmail_profile ** result);
int mailgmail_list_labels(mailgmail * session,
    struct mailgmail_label_list ** result);
int mailgmail_get_label(mailgmail * session,
    const char * label_id,
    struct mailgmail_label ** result);
int mailgmail_list_messages(mailgmail * session,
    struct mailgmail_message_list_request * request,
    struct mailgmail_message_list ** result);
int mailgmail_get_message(mailgmail * session,
    const char * message_id,
    struct mailgmail_message_get_request * request,
    struct mailgmail_message ** result);
int mailgmail_get_attachment(mailgmail * session,
    const char * message_id,
    const char * attachment_id,
    struct mailgmail_attachment ** result);
```

Important libetpan ownership rules:

- `mailgmail_new()` returns a session owned by the caller.
- `mailgmail_free()` releases the session.
- `mailgmail_*` result pointers are owned by the caller on success and must be
  freed with the matching `mailgmail_*_free()` function after conversion.
- Request structs created by `mailgmail_message_list_request_new()` and
  `mailgmail_message_get_request_new()` are owned by the wrapper until the call
  completes, then freed.
- `mailgmail_set_http_transport()` takes ownership of the provided
  `mailhttp_transport`, whether wrapping succeeds or fails.

## Directory Layout

Add a new core module:

```text
src/core/gmail/
  MCGmail.h
  MCGmailTypes.h
  MCGmailSession.h
  MCGmailSession.cpp
  MCGmailProfile.h
  MCGmailProfile.cpp
  MCGmailLabel.h
  MCGmailLabel.cpp
  MCGmailMessageListRequestPrivate.h
  MCGmailMessageListRequestPrivate.cpp
  MCGmailMessageGetRequestPrivate.h
  MCGmailMessageGetRequestPrivate.cpp
  MCGmailMessageList.h
  MCGmailMessageList.cpp
  MCGmailMessageSummary.h
  MCGmailMessageSummary.cpp
  MCGmailMessage.h
  MCGmailMessage.cpp
  MCGmailMessagePart.h
  MCGmailMessagePart.cpp
  MCGmailMessageHeader.h
  MCGmailMessageHeader.cpp
```

`MCGmail.h` is the C++ umbrella header:

```cpp
#ifndef MAILCORE_MCGMAIL_H
#define MAILCORE_MCGMAIL_H

#include <MailCore/MCGmailTypes.h>
#include <MailCore/MCGmailSession.h>
#include <MailCore/MCGmailProfile.h>
#include <MailCore/MCGmailLabel.h>
#include <MailCore/MCGmailMessageList.h>
#include <MailCore/MCGmailMessageSummary.h>
#include <MailCore/MCGmailMessage.h>
#include <MailCore/MCGmailMessagePart.h>
#include <MailCore/MCGmailMessageHeader.h>

#endif
```

## Types

Add `src/core/gmail/MCGmailTypes.h`.

```cpp
namespace mailcore {

enum GmailMessageFormat {
    GmailMessageFormatMinimal,
    GmailMessageFormatFull,
    GmailMessageFormatRaw,
    GmailMessageFormatMetadata,
};

}
```

Mapping:

- `GmailMessageFormatMinimal` -> `MAILGMAIL_MESSAGE_FORMAT_MINIMAL`
- `GmailMessageFormatFull` -> `MAILGMAIL_MESSAGE_FORMAT_FULL`
- `GmailMessageFormatRaw` -> `MAILGMAIL_MESSAGE_FORMAT_RAW`
- `GmailMessageFormatMetadata` -> `MAILGMAIL_MESSAGE_FORMAT_METADATA`

Use an explicit conversion helper rather than relying on enum ordinal equality,
because libetpan currently stores the same concepts in a different order.

Do not expose libetpan error enum values directly. Convert them to MailCore
`ErrorCode` values inside `GmailSession`.

## Session API

Add `src/core/gmail/MCGmailSession.h`.

```cpp
namespace mailcore {

class GmailProfile;
class GmailLabel;
class GmailMessageList;
class GmailMessage;
class GmailMessagePart;

class MAILCORE_EXPORT GmailSession : public Object {
public:
    GmailSession();
    virtual ~GmailSession();

    virtual void setUserID(String * userID);
    virtual String * userID();

    virtual void setOAuth2Token(String * token);
    virtual String * OAuth2Token();

    virtual void setUserAgent(String * userAgent);
    virtual String * userAgent();

    virtual void setTimeout(time_t timeout);
    virtual time_t timeout();

    virtual GmailProfile * profile(ErrorCode * pError);
    virtual Array * /* GmailLabel */ labels(ErrorCode * pError);
    virtual GmailLabel * label(String * labelID, ErrorCode * pError);

    virtual GmailMessageList * messages(ErrorCode * pError);
    virtual GmailMessageList * messagesWithQuery(String * query,
                                                 ErrorCode * pError);
    virtual GmailMessageList * messagesWithLabel(String * labelID,
                                                 ErrorCode * pError);

    virtual GmailMessage * message(String * messageID, ErrorCode * pError);
    virtual GmailMessage * messageWithFormat(String * messageID,
                                             GmailMessageFormat format,
                                             ErrorCode * pError);
    virtual GmailMessage * messageWithMetadataHeaders(String * messageID,
                                                      Array * /* String */ headers,
                                                      ErrorCode * pError);
    virtual Data * messageData(String * messageID, ErrorCode * pError);

    virtual Data * attachmentData(String * messageID,
                                  String * attachmentID,
                                  ErrorCode * pError);
    virtual Data * dataForMessagePart(String * messageID,
                                      GmailMessagePart * part,
                                      ErrorCode * pError);

    virtual int lastHTTPStatus();
    virtual String * lastErrorMessage();

private:
    virtual void setup(ErrorCode * pError);
    virtual void unsetup();
    virtual bool isSetup();

    String * mUserID;
    String * mOAuth2Token;
    String * mUserAgent;
    time_t mTimeout;
    mailgmail * mGmail;

    void init();
    void applyConfiguration(ErrorCode * pError);
};

}
```

Session behavior:

- Construct with no network work.
- Allocate `mailgmail` lazily in `setup()` before the first API call.
- `setup()` should create a `mailgmail` session and apply current configuration.
- `unsetup()` should free `mailgmail` and set `mGmail` to `NULL`.
- Reconfiguration after setup should call the corresponding libetpan setter
  immediately when practical. If a setter fails, keep the MailCore property
  value but report the error through the operation that observes it.
- The common path is:
  - Caller sets `userID`.
  - Caller sets `OAuth2Token`.
  - Caller optionally sets `userAgent`.
  - Caller optionally sets `timeout`.
  - Caller calls Gmail API methods.
- `userID` should default to `"me"` if unset or empty, matching Gmail API
  convention, unless libetpan requires a non-null value.
- `OAuth2Token` is required before network calls.
- `timeout` should default to the same broad style used by other MailCore
  sessions if there is a project default; otherwise use `0` to let libetpan use
  its default.
- `lastHTTPStatus()` returns `0` if the session has not been created.
- `lastErrorMessage()` returns `NULL` if no session exists or libetpan returns
  no message.

HTTP transport:

- Prefer libetpan's default HTTP backend through `mailgmail` internals if
  available.
- If MailCore must explicitly provide a transport, create a
  `mailhttp_transport` using `mailhttp_transport_new_default()` and pass it to
  `mailgmail_set_http_transport()`.
- Because `mailgmail_set_http_transport()` takes ownership, never free that
  transport after passing it to libetpan.

Request-object policy:

- Do not expose `GmailMessageGetRequest` in phase one. The public surface should
  use `message()`, `messageWithFormat()`, and
  `messageWithMetadataHeaders()`.
- Do not expose `GmailMessageListRequest` in phase one. The public surface
  should use small list conveniences only.
- Keep `GmailMessageGetRequest` and `GmailMessageListRequest` as private
  implementation data structures, either in private headers or in
  `MCGmailSession.cpp`. They should not be listed in
  `public-headers.cmake` or included by `MCGmail.h`.
- `GmailSession` should use those private request structures internally, then
  build `mailgmail_message_list_request` and
  `mailgmail_message_get_request` structs from them before calling libetpan.
- `messageData()` is a convenience around `messageWithFormat(...,
  GmailMessageFormatRaw, ...)` that returns decoded RFC 822 bytes.

Convenience method behavior:

- `messages()` creates an internal `GmailMessageListRequest` with default
  values.
- `messagesWithQuery(query)` creates an internal `GmailMessageListRequest` with
  only `query` set.
- `messagesWithLabel(labelID)` creates an internal `GmailMessageListRequest`
  with a one-item label array.
- Do not add public `includeSpamTrash` or `inSpamAndTrash` methods in phase
  one. The former is an advanced widening option; the latter sounds like a
  filter and is ambiguous.
- `message(messageID)` calls
  `messageWithFormat(messageID, GmailMessageFormatFull, pError)`.
- `messageWithFormat()` passes no metadata headers.
- `messageWithMetadataHeaders()` uses `GmailMessageFormatMetadata` and passes
  the provided header names.
- `messageData()` requests `GmailMessageFormatRaw` and returns the decoded
  `RFC822Data()` from the returned `GmailMessage`.

Internal request fields:

- `GmailMessageListRequest` should contain `query`, `labelIDs`, `maxResults`,
  `pageToken`, and `includeSpamTrash`, matching libetpan's list request shape.
  Phase one public methods should only set the default, query-only, or
  single-label forms.
- Keep both `query` and `labelIDs` internally. Gmail search query can express
  some label filters, but `labelIDs` is the API-native stable-ID filter, has
  clear all-labels matching semantics, and works independently from query-string
  parsing.
- Keep `includeSpamTrash` internally. Gmail query syntax can sometimes express
  spam/trash searches, but `includeSpamTrash` is the API-native widening flag,
  composes with `query`, and avoids rewriting caller-provided search strings.
- `includeSpamTrash` means "include SPAM and TRASH in the broader result set,"
  not "return only spam and trash." Avoid public names like `inSpamAndTrash`
  because they sound like filter/AND semantics.
- `GmailMessageGetRequest` should contain `format` and `metadataHeaders`.
  Only one field combination is semantically special: when `format` is
  `GmailMessageFormatMetadata`, `metadataHeaders` may restrict returned
  headers. For `full`, `minimal`, and `raw`, metadata headers should be ignored
  and should not be sent to libetpan.

## Profile Model

Add `src/core/gmail/MCGmailProfile.h`.

```cpp
namespace mailcore {

class MAILCORE_EXPORT GmailProfile : public Object {
public:
    GmailProfile();
    virtual ~GmailProfile();

    virtual String * emailAddress();
    virtual void setEmailAddress(String * emailAddress);

    virtual uint32_t messagesTotal();
    virtual void setMessagesTotal(uint32_t messagesTotal);

    virtual uint32_t threadsTotal();
    virtual void setThreadsTotal(uint32_t threadsTotal);

    virtual String * historyID();
    virtual void setHistoryID(String * historyID);

    virtual String * description();

private:
    String * mEmailAddress;
    uint32_t mMessagesTotal;
    uint32_t mThreadsTotal;
    String * mHistoryID;
};

}
```

Mapping from `struct mailgmail_profile`:

- `email_address` -> `emailAddress`
- `messages_total` -> `messagesTotal`
- `threads_total` -> `threadsTotal`
- `history_id` -> `historyID`

## Label Model

Add `src/core/gmail/MCGmailLabel.h`.

```cpp
namespace mailcore {

class MAILCORE_EXPORT GmailLabel : public Object {
public:
    GmailLabel();
    virtual ~GmailLabel();

    virtual String * identifier();
    virtual void setIdentifier(String * identifier);

    virtual String * name();
    virtual void setName(String * name);

    virtual String * type();
    virtual void setType(String * type);

    virtual String * messageListVisibility();
    virtual void setMessageListVisibility(String * visibility);

    virtual String * labelListVisibility();
    virtual void setLabelListVisibility(String * visibility);

    virtual uint32_t messagesTotal();
    virtual void setMessagesTotal(uint32_t messagesTotal);

    virtual uint32_t messagesUnread();
    virtual void setMessagesUnread(uint32_t messagesUnread);

    virtual uint32_t threadsTotal();
    virtual void setThreadsTotal(uint32_t threadsTotal);

    virtual uint32_t threadsUnread();
    virtual void setThreadsUnread(uint32_t threadsUnread);

    virtual String * description();

private:
    String * mIdentifier;
    String * mName;
    String * mType;
    String * mMessageListVisibility;
    String * mLabelListVisibility;
    uint32_t mMessagesTotal;
    uint32_t mMessagesUnread;
    uint32_t mThreadsTotal;
    uint32_t mThreadsUnread;
};

}
```

Mapping from `struct mailgmail_label`:

- `id` -> `identifier`
- `name` -> `name`
- `type` -> `type`
- `message_list_visibility` -> `messageListVisibility`
- `label_list_visibility` -> `labelListVisibility`
- `messages_total` -> `messagesTotal`
- `messages_unread` -> `messagesUnread`
- `threads_total` -> `threadsTotal`
- `threads_unread` -> `threadsUnread`

## Message List Result

Add `src/core/gmail/MCGmailMessageList.h`.

```cpp
namespace mailcore {

class MAILCORE_EXPORT GmailMessageList : public Object {
public:
    GmailMessageList();
    virtual ~GmailMessageList();

    virtual Array * /* GmailMessageSummary */ messages();
    virtual void setMessages(Array * messages);

    virtual String * nextPageToken();
    virtual void setNextPageToken(String * nextPageToken);

    virtual uint32_t resultSizeEstimate();
    virtual void setResultSizeEstimate(uint32_t resultSizeEstimate);

private:
    Array * mMessages;
    String * mNextPageToken;
    uint32_t mResultSizeEstimate;
};

}
```

Mapping from `struct mailgmail_message_list`:

- `messages` -> `Array` of `GmailMessageSummary`
- `next_page_token` -> `nextPageToken`
- `result_size_estimate` -> `resultSizeEstimate`

## Message Summary Model

Add `src/core/gmail/MCGmailMessageSummary.h`.

```cpp
namespace mailcore {

class MAILCORE_EXPORT GmailMessageSummary : public Object {
public:
    GmailMessageSummary();
    virtual ~GmailMessageSummary();

    virtual String * identifier();
    virtual void setIdentifier(String * identifier);

    virtual String * threadID();
    virtual void setThreadID(String * threadID);

private:
    String * mIdentifier;
    String * mThreadID;
};

}
```

Mapping from `struct mailgmail_message_summary`:

- `id` -> `identifier`
- `thread_id` -> `threadID`

## Message Model

Add `src/core/gmail/MCGmailMessage.h`.

```cpp
namespace mailcore {

class AbstractMessage;
class GmailMessagePart;

class MAILCORE_EXPORT GmailMessage : public Object {
public:
    GmailMessage();
    virtual ~GmailMessage();

    virtual String * identifier();
    virtual void setIdentifier(String * identifier);

    virtual String * threadID();
    virtual void setThreadID(String * threadID);

    virtual Array * /* String */ labelIDs();
    virtual void setLabelIDs(Array * labelIDs);

    virtual String * snippet();
    virtual void setSnippet(String * snippet);

    virtual String * historyID();
    virtual void setHistoryID(String * historyID);

    virtual String * internalDate();
    virtual void setInternalDate(String * internalDate);

    virtual uint32_t sizeEstimate();
    virtual void setSizeEstimate(uint32_t sizeEstimate);

    virtual Data * RFC822Data();
    virtual void setRFC822Data(Data * RFC822Data);

    virtual GmailMessagePart * payload();
    virtual void setPayload(GmailMessagePart * payload);

    virtual AbstractMessage * parsedMessage(ErrorCode * pError);

private:
    String * mIdentifier;
    String * mThreadID;
    Array * mLabelIDs;
    String * mSnippet;
    String * mHistoryID;
    String * mInternalDate;
    uint32_t mSizeEstimate;
    Data * mRFC822Data;
    GmailMessagePart * mPayload;
};

}
```

Mapping from `struct mailgmail_message`:

- `id` -> `identifier`
- `thread_id` -> `threadID`
- `label_ids` -> `labelIDs`
- `snippet` -> `snippet`
- `history_id` -> `historyID`
- `internal_date` -> `internalDate`
- `size_estimate` -> `sizeEstimate`
- `raw` -> `RFC822Data`
- `payload` -> `payload`

`raw` / RFC 822 handling:

- Gmail API raw message data is URL-safe base64 text.
- The wrapper should decode URL-safe base64 before storing it in
  `RFC822Data()`.
- `RFC822Data()` should return usable RFC 822 bytes, similar to
  `IMAPSession::fetchMessageByUID()`.
- Keep the encoded Gmail API `raw` string out of the public model for phase one.
  If callers need exact API JSON fields later, add an explicitly named
  `encodedRaw()` accessor.
- `parsedMessage()` should parse `RFC822Data()` through MailCore's existing
  RFC 822 parser when raw data is present.
- If only Gmail `payload` data is present, `parsedMessage()` may build a best
  effort `AbstractMessage` from payload headers and parts, but it should prefer
  RFC 822 parsing when available.

`internalDate` handling:

- Keep it as `String` in the first pass because libetpan exposes it as text.
- A later convenience accessor can parse milliseconds since epoch into `time_t`
  if the project wants that behavior.

## Message Header Model

Add `src/core/gmail/MCGmailMessageHeader.h`.

```cpp
namespace mailcore {

class MAILCORE_EXPORT GmailMessageHeader : public Object {
public:
    GmailMessageHeader();
    virtual ~GmailMessageHeader();

    virtual String * name();
    virtual void setName(String * name);

    virtual String * value();
    virtual void setValue(String * value);

private:
    String * mName;
    String * mValue;
};

}
```

Mapping from `struct mailgmail_message_header`:

- `name` -> `name`
- `value` -> `value`

Do not replace Gmail's ordered header list with `MessageHeader` in the primary
Gmail model. Gmail API metadata headers are returned as a simple ordered
name/value list, and preserving that shape avoids lossy parsing. The
`parsedMessage()` convenience may separately construct MailCore `MessageHeader`
data for the returned `AbstractMessage`.

## Message Part Model

Add `src/core/gmail/MCGmailMessagePart.h`.

```cpp
namespace mailcore {

class GmailMessageHeader;

class MAILCORE_EXPORT GmailMessagePart : public AbstractMessagePart {
public:
    GmailMessagePart();
    virtual ~GmailMessagePart();

    virtual String * partID();
    virtual void setPartID(String * partID);

    virtual Array * /* GmailMessageHeader */ headers();
    virtual void setHeaders(Array * headers);

    virtual String * attachmentID();
    virtual void setAttachmentID(String * attachmentID);

    virtual uint32_t size();
    virtual void setSize(uint32_t size);

    virtual Data * data();
    virtual void setData(Data * data);

    virtual Array * /* GmailMessagePart */ parts();
    virtual void setParts(Array * parts);

private:
    String * mPartID;
    Array * mHeaders;
    String * mAttachmentID;
    uint32_t mSize;
    Data * mData;
    Array * mParts;
};

}
```

Mapping from `struct mailgmail_message_part`:

- `part_id` -> `partID`
- `mime_type` -> inherited `mimeType`
- `filename` -> inherited `filename`
- `headers` -> `Array` of `GmailMessageHeader`
- `body.attachment_id` -> `attachmentID`
- `body.size` -> `size`
- decoded `body.data` -> `data`
- `parts` -> `Array` of child `GmailMessagePart`

`data` handling:

- Gmail API body and attachment data are URL-safe base64 text.
- Inline `body.data` should be decoded into `GmailMessagePart::data()`.
- If only `attachmentID()` is present, callers should use
  `GmailSession::attachmentData()` or `GmailSession::dataForMessagePart()`.
- `dataForMessagePart()` should return inline data when available and otherwise
  fetch decoded attachment bytes by `attachmentID()`.

## Error Mapping

Add a private helper in `MCGmailSession.cpp`:

```cpp
static ErrorCode errorCodeFromGmailError(int r)
```

Suggested mapping:

- `MAILGMAIL_NO_ERROR` -> `ErrorNone`
- `MAILGMAIL_ERROR_UNAUTHORIZED` -> `ErrorAuthentication`
- `MAILGMAIL_ERROR_FORBIDDEN` -> `ErrorAuthentication`
- `MAILGMAIL_ERROR_NOT_FOUND` -> `ErrorNonExistantFolder` only if a better
  generic not-found error does not exist; otherwise prefer a generic fetch error
  for message/attachment calls.
- `MAILGMAIL_ERROR_RATE_LIMITED` -> `ErrorConnection`
- `MAILGMAIL_ERROR_HTTP_UNAVAILABLE` -> `ErrorConnection`
- `MAILGMAIL_ERROR_SSL` -> `ErrorCertificate`
- `MAILGMAIL_ERROR_MEMORY` -> `ErrorMemory`
- `MAILGMAIL_ERROR_PARSE` -> `ErrorParse`
- `MAILGMAIL_ERROR_PROTOCOL` -> `ErrorParse`
- `MAILGMAIL_ERROR_BAD_STATE` -> `ErrorConnection`
- `MAILGMAIL_ERROR_HTTP` -> `ErrorConnection`
- `MAILGMAIL_ERROR_CONFLICT` -> `ErrorConnection`
- `MAILGMAIL_ERROR_SERVER` -> `ErrorConnection`
- `MAILGMAIL_ERROR_NOT_IMPLEMENTED` -> `ErrorConnection`
- unknown values -> `ErrorConnection`

Before implementation, confirm the exact `ErrorCode` enum names currently
available in `MCMessageConstants.h`. If no appropriate `ErrorMemory` or
`ErrorParse` exists, use the closest existing project error and document it in
the helper.

Per-method failures:

- `profile()` should use `ErrorAuthentication` for auth failures and
  `ErrorConnection` for transport/server failures.
- `labels()` and `label()` should use fetch/list-shaped errors where MailCore
  has them; otherwise use the generic mapping.
- `messages()`, `message()`, `attachmentData()`, and `dataForMessagePart()`
  should avoid inventing new `ErrorCode` enum values in the first pass.

## Conversion Helpers

Implement private helpers in `MCGmailSession.cpp` or a private implementation
section:

```cpp
static String * stringFromNullableCString(const char * value);
static Data * decodedBase64URLDataFromNullableCString(const char * value,
                                                      ErrorCode * pError);
static Array * stringArrayFromCList(clist * list);
static GmailProfile * profileFromLibetpan(struct mailgmail_profile * profile);
static GmailLabel * labelFromLibetpan(struct mailgmail_label * label);
static GmailMessageSummary * summaryFromLibetpan(
    struct mailgmail_message_summary * summary);
static GmailMessageList * messageListFromLibetpan(
    struct mailgmail_message_list * list);
static GmailMessageHeader * headerFromLibetpan(
    struct mailgmail_message_header * header);
static GmailMessagePart * partFromLibetpan(
    struct mailgmail_message_part * part);
static GmailMessage * messageFromLibetpan(
    struct mailgmail_message * message);
```

Conversion rules:

- `NULL` C strings become `NULL` MailCore `String *`, not empty strings.
- `NULL` lists become empty `Array` instances for list-valued properties.
- Numeric fields copy directly.
- Gmail API base64url fields must be decoded before storing them in public
  `Data *` fields:
  - `mailgmail_message.raw` -> `GmailMessage::RFC822Data()`
  - `mailgmail_message_part_body.data` -> `GmailMessagePart::data()`
  - `mailgmail_attachment.data` -> `GmailSession::attachmentData()`
- Nested objects should be retained through setters following the existing
  MailCore ownership idioms.
- Free every libetpan object after conversion, even on partial conversion
  failures.

Private request conversion helpers:

```cpp
static struct mailgmail_message_list_request *
createLibetpanMessageListRequest(GmailMessageListRequest * request,
                                 ErrorCode * pError);

static struct mailgmail_message_get_request *
createLibetpanMessageGetRequest(GmailMessageGetRequest * request,
                                ErrorCode * pError);
```

Request conversion failure should set `*pError` and return `NULL`.

Internal message-list mapping to `struct mailgmail_message_list_request`:

- `maxResults` -> `max_results`
- `pageToken` -> `mailgmail_message_list_request_set_page_token()`
- `query` -> `mailgmail_message_list_request_set_query()`
- `labelIDs` -> repeated `mailgmail_message_list_request_add_label_id()`
- `includeSpamTrash` -> `include_spam_trash`

Internal message-get mapping to `struct mailgmail_message_get_request`:

- `format` -> constructor argument to `mailgmail_message_get_request_new()`
- `metadataHeaders` -> repeated
  `mailgmail_message_get_request_add_metadata_header()`

## Build Wiring

Update `src/cmake/core.cmake`:

- Add `gmail_files`:

```cmake
set(gmail_files
  core/gmail/MCGmailSession.cpp
  core/gmail/MCGmailProfile.cpp
  core/gmail/MCGmailLabel.cpp
  core/gmail/MCGmailMessageListRequestPrivate.cpp
  core/gmail/MCGmailMessageGetRequestPrivate.cpp
  core/gmail/MCGmailMessageList.cpp
  core/gmail/MCGmailMessageSummary.cpp
  core/gmail/MCGmailMessage.cpp
  core/gmail/MCGmailMessagePart.cpp
  core/gmail/MCGmailMessageHeader.cpp
)
```

- Add `${gmail_files}` to `core_files`.
- Add `"${CMAKE_CURRENT_SOURCE_DIR}/core/gmail"` to `core_includes`.
- Private request headers such as `MCGmailMessageListRequestPrivate.h` and
  `MCGmailMessageGetRequestPrivate.h` should not be public headers. If their
  constructors/destructors live in `.cpp` files, include those `.cpp` files in
  `gmail_files`.

Update `src/cmake/public-headers.cmake`:

```cmake
core/gmail/MCGmail.h
core/gmail/MCGmailTypes.h
core/gmail/MCGmailSession.h
core/gmail/MCGmailProfile.h
core/gmail/MCGmailLabel.h
core/gmail/MCGmailMessageList.h
core/gmail/MCGmailMessageSummary.h
core/gmail/MCGmailMessage.h
core/gmail/MCGmailMessagePart.h
core/gmail/MCGmailMessageHeader.h
```

Optionally update `src/core/MCCore.h` or `MailCore.h` only if the project
expects every core module to be pulled through the top-level umbrella. Keep this
consistent with existing `MCIMAP.h`, `MCPOP.h`, and `MCSMTP.h`.

libetpan dependency checks:

- Confirm the configured libetpan install exposes `<libetpan/mailgmail.h>`.
- Confirm `<libetpan/mailgmail_types.h>` is installed.
- Confirm the libetpan library linked by MailCore includes the Gmail objects.
- Confirm a usable `mailhttp` backend is built: curl on non-Apple platforms and
  NSURLSession where available on Apple platforms.

## Implementation Order

1. Add headers and empty model implementations.
2. Wire CMake and public headers.
3. Implement model constructors, destructors, getters, setters, and
   `description()` where useful.
4. Implement `GmailSession` configuration and lifecycle.
5. Implement conversion helpers and request builders.
6. Implement API methods one by one:
   - `profile()`
   - `labels()`
   - `label()`
   - `messages()`
   - `messagesWithQuery()`
   - `messagesWithLabel()`
   - `message()`
   - `messageWithFormat()`
   - `messageWithMetadataHeaders()`
   - `messageData()`
   - `attachmentData()`
   - `dataForMessagePart()`
7. Add tests.
8. Build on the primary local platform.

## Testing Plan

Unit or compile-time tests:

- Model setters/getters retain and return expected values.
- Private request builders create libetpan requests with:
  - empty/default request
  - query
  - one label
  - internal `includeSpamTrash` set to true
  - metadata format with metadata headers
  - full/minimal/raw formats without metadata headers
- Conversion helpers handle:
  - `NULL` strings
  - empty lists
  - nested multipart payloads
  - decoded raw message data as RFC 822 bytes
  - decoded inline part body data on `GmailMessagePart`
  - decoded attachment data returned by `attachmentData()`
  - `dataForMessagePart()` returning inline data or fetching by attachment ID
- `GmailMessage::parsedMessage()` parses raw RFC 822 data when available.
- `GmailSession::messageData()` fetches RAW format and returns decoded RFC 822
  bytes.
- Error mapping covers every `MAILGMAIL_ERROR_*` value.

Mock HTTP tests:

- Prefer a fake `mailhttp_transport` if test infrastructure can link against
  libetpan's `mailhttp` API.
- Feed deterministic Gmail JSON responses through the Gmail session:
  - profile
  - label list
  - single label
  - message list
  - full message with nested parts
  - metadata message
  - raw message
  - attachment data
- Verify `lastHTTPStatus()` and `lastErrorMessage()` after errors.

Live smoke test:

- Keep this opt-in behind environment variables.
- Proposed command shape:

```sh
GMAIL_USER_ID=me \
GMAIL_OAUTH2_TOKEN=... \
./tests/gmail-live-smoke-test.sh
```

Live smoke should:

- Fetch profile.
- List labels.
- List one page of inbox messages with `maxResults = 1`.
- Fetch the returned message in metadata format.

Do not include live send or mutation tests in this phase.

## Future Follow-Ups

- Add Objective-C wrappers under `src/objc/gmail`.
- Add async wrappers under `src/async/gmail`.
- Add explicitly named encoded-data accessors only if callers later need exact
  Gmail API base64url strings.
- Add spam/trash list conveniences only if a real caller needs full mailbox
  sweep behavior. Prefer explicit widening names such as
  `messagesIncludingSpamAndTrash()` and
  `messagesWithQueryIncludingSpamAndTrash()`. Avoid `inSpamAndTrash`.
- Add a small public list options object only if callers need combinations such
  as pagination, multiple labels, custom `maxResults`, and spam/trash inclusion
  at the public API layer.
- Add Gmail message mutation APIs after the read-only surface is stable:
  modify labels, trash/untrash, delete, batch modify.
- Add thread, history, draft, watch, and settings APIs if libetpan exposes them.
- Broaden `parsedMessage()` if needed so payload-only messages can be converted
  into richer `AbstractMessage` objects without RAW data.
