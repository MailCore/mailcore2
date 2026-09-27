# JMAP API Plan

## Goal

Add a simple JMAP API to MailCore on top of libetpan's low-level JMAP implementation:

- `src/core/jmap`: synchronous C++ MailCore objects and session methods over libetpan.
- `src/async/jmap`: asynchronous operation/session layer for network calls.
- `src/objc/jmap`: Objective-C public API mirroring the async layer.

Use the existing IMAP stack as the structural model, but keep the first JMAP surface intentionally small.

## Existing Pattern To Follow

The current IMAP stack is layered as:

- `src/core/imap`
  - `IMAPSession` owns the libetpan `mailimap *` session.
  - Synchronous methods connect, authenticate, run one network command, translate libetpan errors to `ErrorCode`, and convert libetpan structs into MailCore C++ objects.
  - Public umbrella header: `MCIMAP.h`.
- `src/async/imap`
  - `IMAPAsyncSession` owns connection/session configuration and creates operation objects.
  - `IMAPAsyncConnection` serializes network work against an underlying core `IMAPSession`.
  - `IMAPOperation` is the base operation type; concrete operations call one synchronous core method in `main()`, store results, and dispatch callbacks.
  - Public umbrella header: `MCAsyncIMAP.h`.
- `src/objc/imap`
  - `MCOIMAPSession` wraps `mailcore::IMAPAsyncSession`.
  - Each Objective-C operation wraps a matching C++ async operation and exposes a block-based `start:` method.
  - Public umbrella header: `MCOIMAP.h`.

JMAP should reuse this shape so users see the same MailCore API style across protocols.

## Gmail Pattern To Follow

`src/core/gmail` is the closest existing precedent for taking a provider-specific HTTP API and turning it into a smaller MailCore API:

- `GmailSession` hides libetpan request structs behind intent-level methods such as `profile()`, `labels()`, `messagesWithQuery()`, `messageWithFormat()`, `messageData()`, `attachmentData()`, and `dataForPart()`.
- Request-building details live in private helpers and private request types instead of becoming the main public surface.
- libetpan structs are copied into MailCore objects immediately, and the rest of the API speaks MailCore collections, strings, data, and message objects.
- Gmail keeps useful transport details (`lastHTTPStatus()`, `lastErrorMessage()`) without forcing callers to parse libetpan error state.
- Gmail message payloads are simplified into MailCore's explicit message/part hierarchy:
  - `GmailMessage : AbstractMessage`
  - `GmailPart : AbstractPart`
  - `GmailMultipart : AbstractMultipart`
  - `GmailMessagePart : AbstractMessagePart`

JMAP should follow that same simplification strategy. The default API should expose common mail operations and MailCore-shaped objects, not libetpan's raw JMAP request/response grammar. A custom/raw call can exist as an escape hatch, but it should not be the primary user experience.

## libetpan JMAP Surface

The relevant libetpan API lives in `~/Sources/libetpan/src/low-level/jmap`:

- `mailjmap.h`
  - `mailjmap_new()`, `mailjmap_free()`
  - `mailjmap_connect(session, session_url)`
  - `mailjmap_discover(session, domain_or_email, &session_result)`
  - `mailjmap_login_oauth2(session, user, access_token)`
  - `mailjmap_set_oauth2_token(session, access_token)`
  - `mailjmap_set_timeout(session, timeout)`
  - `mailjmap_get_session(session, &session_result)`
  - `mailjmap_call(session, request, &response)`
  - last-error accessors for HTTP status, problem type, method name/call id, method error type/description.
- `mailjmap_types.h`
  - `mailjmap_session`, account, capability, primary-account, blob-upload types.
  - Error constants: `MAILJMAP_NO_ERROR`, `MAILJMAP_ERROR_AUTHENTICATION`, `MAILJMAP_ERROR_DISCOVERY`, `MAILJMAP_ERROR_HTTP`, etc.
- `mailjmap_mail.h`
  - Typed JMAP Mail objects: mailbox, thread, email, body part/value, identity, query/filter/sort/result, set/import/copy/submission result types.
  - Helper functions for `Mailbox/get`, `Email/get`, `Email/query`, changes, set, import, parse, snippet, and submission flows.
- `mailjmap_blob.h`
  - `mailjmap_upload()`
  - `mailjmap_download()`

The first MailCore layer should use libetpan's typed helpers where available and reserve generic `mailjmap_call()` for custom or not-yet-wrapped methods.

When libetpan exposes many knobs for a method, wrap the common case in a small MailCore method first and keep advanced request construction private until a caller-facing need is clear.

## Initial API Slice

Keep v1 small enough to land safely:

1. Session setup
   - session URL
   - discovery by domain/email
   - username
   - OAuth2 token
   - timeout
   - certificate validation toggle equivalent to IMAP's `checkCertificateEnabled`
   - connection logger only if libetpan JMAP HTTP transport exposes logging; otherwise defer
2. Session/account metadata
   - connect to known session URL
   - discover session
   - fetch session object
   - choose primary mail account for JMAP Mail capability
3. Mailboxes
   - get all mailboxes
   - get mailboxes by id
   - create/update/destroy mailbox if libetpan helper coverage is straightforward
4. Message read/query
   - `Email/query`
   - `Email/get`
   - optional combined query-then-get convenience exposed as a message operation
5. Blob
   - upload data
   - download blob data
6. Drafts and send
   - create draft from RFC822 data or `MessageBuilder` output
   - update draft mailbox/keywords where supported
   - delete draft
   - submit draft/message through JMAP `EmailSubmission/set`
   - expose sent/submission ids and delivery/undo status through operation results without making the set result wrappers public by default
7. Escape hatch
   - custom JMAP method call using libetpan request/response objects or JSON values, after the typed API is in place.

Defer push/event source, long-lived state sync helpers, and complex JMAP batching until the basic session/read/blob/draft/send path is proven.

## Core C++ Design

Create `src/core/jmap` with:

- `MCJMAP.h`
  - Umbrella header.
- `MCJMAPSession.h/.cpp`
  - Owns `mailjmap * mJMAP`.
  - Stores `sessionURL`, `domainOrEmail`, `username`, `OAuth2Token`, `timeout`.
  - Stores `checkCertificateEnabled`, defaulting to the same behavior as IMAP.
  - Methods:
    - `connect(ErrorCode * pError)`
    - `discover(ErrorCode * pError)`
    - `login(ErrorCode * pError)`
    - `disconnect()`
    - `sessionInfo(ErrorCode * pError)`
    - `primaryMailAccountID(ErrorCode * pError)`
    - mailbox/message/blob methods from the initial slice.
    - draft/send methods from the initial slice.
  - Uses `setup()`/`unsetup()` pattern from `IMAPSession`.
  - Maps libetpan JMAP errors to MailCore `ErrorCode`.
  - Captures detailed JMAP failure fields on the session for debugging.
  - Validates the HTTPS/TLS peer certificate after connect/discovery HTTP transport setup, using the same policy as `IMAPSession::checkCertificate()`:
    - if disabled, skip validation
    - if enabled and validation fails, set `ErrorCertificate`
    - validate against the effective JMAP host, not merely the discovery input
- Minimal public v1 data objects:
  - `MCJMAPMailbox`
  - `MCJMAPMessage`
  - `MCJMAPPart`
  - `MCJMAPMultipart`
  - `MCJMAPMessagePart`
  - `MCJMAPBlobUpload`
  - `MCJMAPSubmission`
- Public only if a later feature proves it needs a stable object:
  - session/account/capability metadata objects
  - query filter/sort/result objects
  - thread and identity objects
  - changes/query-changes/set result objects
- Internal/private data objects:
  - libetpan request builders, result envelopes, account/capability structs, address/header/body-value wrappers, set items, copy/import/parse/submission mutation items, and method result wrappers that can be flattened into operation return values.
- Conversion helpers:
  - Keep libetpan-to-MailCore conversion local to `MCJMAPSession.cpp` at first.
  - Move into private helpers only after duplication appears.
  - Copy all strings/data into MailCore-owned objects before freeing libetpan results.
  - Convert JMAP email body structures into the explicit MailCore part hierarchy, following MIME, IMAP, and Gmail:
    - single leaf body part -> `JMAPPart : AbstractPart`
    - nested/multipart body -> `JMAPMultipart : AbstractMultipart`
    - embedded `message/rfc822` body -> `JMAPMessagePart : AbstractMessagePart`
    - top-level email object -> `JMAPMessage : AbstractMessage`
  - Reuse existing RFC822/IMAP message behavior wherever the model matches:
    - use `AbstractMessage` behavior for headers, attachments, inline attachments, rendering-required parts, and part lookup
    - use `MessageHeader` for parsed RFC822-style headers instead of a JMAP-specific public header object
    - use `AbstractPart`, `AbstractMultipart`, and `AbstractMessagePart` behavior for MIME metadata, nested parts, content-id lookup, unique part ids, and embedded messages
    - keep JMAP-specific fields such as email id, blob id, thread id, mailbox ids, keywords, preview, and JMAP body value state as additive fields on `JMAPMessage`/`JMAPPart`
    - avoid duplicating IMAP-only concepts such as sequence number, UID, modseq, and IMAP flags unless there is a clear JMAP equivalent
  - Preserve JMAP-specific identifiers on those objects:
    - email id
    - blob id
    - thread id
    - part id
    - mailbox ids
    - keywords
    - body values
    - preview, received/sent timestamps, and size
  - Use existing `MessageHeader`, `AbstractPart` MIME fields, and content metadata wherever they fit before adding JMAP-only properties.
- Draft/send helpers:
  - Reuse existing RFC822 `MessageBuilder` output when creating draft/send messages from MailCore objects.
  - Prefer a simple public operation over public request-builder classes:
    - import/create draft from RFC822 `Data`
    - send existing message id with identity id
    - upload attachment data, then reference returned blob ids internally when constructing the message
  - Keep JMAP `Email/set`, `Email/import`, and `EmailSubmission/set` request item classes private.
  - Return a minimal `JMAPSubmission` result object for send operations instead of exposing the full `JMAPSetResult` graph.
- Error handling:
  - Add JMAP-specific mappings for authentication, discovery, HTTP, JSON parse, protocol, capability, method, limit, stream, and memory errors.
  - Preserve existing MailCore error semantics for network/auth failures where possible.
  - Map certificate validation failures to `ErrorCertificate`, matching IMAP/POP/SMTP/NNTP behavior.
  - Add optional detail getters such as `lastHTTPStatus`, `lastErrorMessage`, `lastProblemType`, `lastMethodErrorType`, `lastMethodErrorDescription`.

## Certificate Validation

JMAP should expose certificate validation in the same style as IMAP:

- Core C++:
  - `JMAPSession::setCheckCertificateEnabled(bool enabled)`
  - `JMAPSession::isCheckCertificateEnabled()`
  - private `JMAPSession::checkCertificate()` helper
- Async C++:
  - `JMAPAsyncSession::setCheckCertificateEnabled(bool enabled)`
  - `JMAPAsyncSession::isCheckCertificateEnabled()`
  - propagate the value into the underlying `JMAPSession` before network work starts
- Objective-C:
  - `@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;` on `MCOJMAPSession`

Implementation should mirror IMAP's flow:

- IMAP stores the toggle on `IMAPSession`.
- After SSL or STARTTLS setup, `IMAPSession::checkCertificate()` calls `mailcore::checkCertificate(mImap->imap_stream, hostname())`.
- If validation fails, IMAP returns `ErrorCertificate`.

For JMAP, the transport is HTTP/TLS rather than a `mailstream` IMAP socket, so the implementation needs one of these approaches:

- Prefer a libetpan JMAP HTTP transport hook that exposes the TLS stream/certificate validation callback.
- If libetpan's curl transport is used, configure curl's certificate verification and hostname verification from `checkCertificateEnabled`.
- If MailCore must perform validation itself, expose enough transport state from libetpan to validate the peer with `MCCertificateUtils` or an equivalent shared helper.

The current MailCore JMAP API exposes the same public toggle at the core, async, and Objective-C layers. The remaining work is in the HTTP transport boundary: libetpan's public `mailjmap_http_transport` has `perform`/`free` callbacks only, and the underlying `mailhttp_request` has no certificate-validation flag. The curl backend does map TLS failures and curl validates peers by default, so the default path remains certificate-validating, but MailCore cannot yet mirror IMAP's explicit stream-level check or honor `checkCertificateEnabled = false` for JMAP without a libetpan transport hook.

The plan should not ship JMAP as a fully equivalent replacement for IMAP certificate handling until one of these is true:

- libetpan exposes a JMAP/HTTP transport option for peer and hostname verification.
- MailCore provides a custom JMAP HTTP transport that owns TLS verification policy for each supported backend.
- The public JMAP documentation clearly marks `checkCertificateEnabled` as default-validated but not yet a full IMAP-equivalent toggle.

## Minimal Public Class Surface

The v1 public API should expose only the classes callers need to configure a session, start operations, and inspect returned mail data.

- Core public classes:
  - `JMAPSession`
  - `JMAPMailbox`
  - `JMAPMessage`
  - `JMAPPart`
  - `JMAPMultipart`
  - `JMAPMessagePart`
  - `JMAPBlobUpload`
  - `JMAPSubmission`
- Async public classes:
  - `JMAPAsyncSession`
  - `JMAPOperation`
  - result-bearing operation classes only where callbacks need typed access:
    - `JMAPFetchMailboxesOperation`
    - `JMAPFetchMessagesOperation`
    - `JMAPQueryMessagesOperation`
    - `JMAPUploadOperation`
    - `JMAPDownloadOperation`
    - `JMAPCreateDraftOperation`
    - `JMAPSendOperation`
  - use plain `JMAPOperation` for connect, discover, and other void-result commands.
- Objective-C public classes:
  - `MCOJMAPSession`
  - `MCOJMAPOperation`
  - `MCOJMAPMailbox`
  - `MCOJMAPMessage`
  - `MCOJMAPPart`
  - `MCOJMAPMultipart`
  - `MCOJMAPMessagePart`
  - `MCOJMAPBlobUpload`
  - `MCOJMAPSubmission`
  - result-bearing operation classes only where callbacks need typed access:
    - `MCOJMAPFetchMailboxesOperation`
    - `MCOJMAPFetchMessagesOperation`
    - `MCOJMAPQueryMessagesOperation`
    - `MCOJMAPUploadOperation`
    - `MCOJMAPDownloadOperation`
    - `MCOJMAPCreateDraftOperation`
    - `MCOJMAPSendOperation`

Everything else should be private or represented with existing MailCore types (`String`, `Array`, `HashMap`, `Data`, `Address`, `MessageHeader`, `IndexSet`) until a use case forces a named public class.

## Data Object Catalog And Visibility

This catalog intentionally marks many libetpan/JMAP shapes as internal. Names should follow MailCore style in C++ and Objective-C, but field names below are written descriptively so they can map cleanly from libetpan.

Visibility rules:

- Public objects should be the minimum durable MailCore domain concepts callers naturally keep or inspect: mailboxes, messages, MIME-like parts, and blob upload results.
- Reuse existing MailCore public types where possible. Use `MCAddress`/`MCOAddress` for addresses and `MessageHeader`/`MCOMessageHeader` for headers instead of adding JMAP-specific address/header classes.
- Internal/private objects should represent request construction, protocol plumbing, or result envelopes that are only useful while translating libetpan into MailCore.
- A libetpan result wrapper should become public only when callers need its metadata. Otherwise, async operations can expose the useful pieces directly, such as `messages`, `notFound`, and `state`.
- Full protocol result graphs stay private until callers need them. For v1 draft/send, expose `JMAPSubmission` and keep lower-level set/import/submission request items and set-result wrappers private.
- Prefer private `...RequestPrivate` classes for rich request options, following Gmail's `GmailMessageListRequest` and `GmailMessageGetRequest` pattern.

### Minimal Public In V1

- `JMAPMailbox`
  - `identifier`: mailbox id.
  - `name`: mailbox display name.
  - `parentID`: parent mailbox id, nullable.
  - `role`: JMAP role string, nullable.
  - `sortOrder`: server-provided ordering.
  - `subscribed`: subscription state.
  - `totalEmails`: total email count.
  - `unreadEmails`: unread email count.
  - `totalThreads`: total thread count.
  - `unreadThreads`: unread thread count.
  - `rights`: map from right name to boolean.
- `JMAPMessage : AbstractMessage`
  - `identifier`: JMAP email id.
  - `blobID`: blob id for the whole message.
  - `threadID`: thread id.
  - `mailboxIDs`: array or set of mailbox ids.
  - `keywords`: array or set of keyword strings.
  - `size`: message size.
  - `preview`: preview/snippet string.
  - `header`: inherited `MessageHeader`, the canonical public location for subject, dates, message-id, in-reply-to, references, sender/from/to/cc/bcc/reply-to, and extra headers.
  - JMAP `receivedAt` should populate `MessageHeader::receivedDate()` when parseable.
  - JMAP `sentAt` should populate `MessageHeader::date()` when parseable.
  - JMAP-specific raw header/body-value fields stay internal unless `MessageHeader` cannot preserve needed data.
  - `mainPart`: top-level `AbstractPart` converted from `bodyStructure`.
  - `textBody`: array of `JMAPPart` references from JMAP `textBody`.
  - `htmlBody`: array of `JMAPPart` references from JMAP `htmlBody`.
  - `attachments`: array of `JMAPPart` references from JMAP `attachments`.
  - Body values stay internal; expose rendered/fetched data through operations rather than a public body-value class.
- `JMAPPart : AbstractPart`
  - `partID`: JMAP part id.
  - `blobID`: blob id for fetching the part content.
  - `size`: part size when provided.
  - Uses inherited fields for MIME type, filename/name, charset, content id, content location, content disposition/attachment flags, content description, and content type parameters.
  - JMAP-only fields to preserve:
    - `language`: single language string.
    - `languages`: array of language strings.
    - part headers stay internal unless the existing `AbstractPart`/`MessageHeader` model cannot preserve required data.
- `JMAPMultipart : AbstractMultipart`
  - `partID`: JMAP part id.
  - `blobID`: blob id if provided.
  - `size`: part size when provided.
  - `parts`: inherited array of child `AbstractPart` objects.
  - Uses inherited MIME/content metadata plus JMAP language and header fields as above.
- `JMAPMessagePart : AbstractMessagePart`
  - `partID`: JMAP part id.
  - `blobID`: blob id if provided.
  - `size`: part size when provided.
  - `mainPart`: inherited embedded message body part.
  - `header`: inherited `MessageHeader`.
  - Uses inherited MIME/content metadata plus JMAP language and header fields as above.
- `JMAPBlobUpload`
  - `accountID`: account id.
  - `blobID`: uploaded blob id.
  - `type`: content type.
  - `name`: blob name.
  - `size`: uploaded size.
- `JMAPSubmission`
  - Minimal public send result.
  - `identifier`: submission id.
  - `emailID`: submitted email/message id.
  - `threadID`: thread id if returned.
  - `identityID`: identity id used for submission.
  - `sendAt`: scheduled/actual send timestamp if returned.
  - `undoStatus`: JMAP undo status if returned.
  - `deliveryStatus`: optional simple map/array retained only if useful; do not expose the full set-result graph in v1.

### Not Public In V1

These are useful protocol/domain shapes, but they should remain private in v1. Promote one only when a public operation needs callers to keep the object as a stable value.

- `JMAPThread`
  - Public when `Thread/get` or thread-aware message operations are added.
  - `identifier`: thread id.
  - `emailIDs`: array of email/message ids in the thread.
- `JMAPIdentity`
  - Public when identity fetch/submission support is added.
  - `identifier`: identity id.
  - `name`: display name.
  - `email`: email address.
  - `replyTo`: reply-to address string.
  - `bcc`: bcc address string.
  - `textSignature`: plain-text signature.
  - `htmlSignature`: HTML signature.
- `JMAPChangesResult`
  - Public when mailbox/message/thread changes operations are added.
  - `accountID`: account id.
  - `oldState`: previous state.
  - `newState`: new state.
  - `hasMoreChanges`: whether additional changes remain.
  - `created`: array of created ids.
  - `updated`: array of updated ids.
  - `destroyed`: array of destroyed ids.
- `JMAPQueryChange`
  - Public when query changes operations are added.
  - `identifier`: id added to the query.
  - `index`: index where it was added.
- `JMAPQueryChangesResult`
  - Public when query changes operations are added.
  - `accountID`: account id.
  - `oldQueryState`: previous query state.
  - `newQueryState`: new query state.
  - `total`: total count when provided.
  - `hasTotal`: whether `total` is valid.
  - `removed`: array of removed ids.
  - `added`: array of `JMAPQueryChange`.
- `JMAPSetResult`
  - Public when create/update/destroy operations are added.
  - `accountID`: account id.
  - `oldState`: previous state.
  - `newState`: new state.
  - `created`: array of `JMAPSetCreated`.
  - `updated`: array of updated ids.
  - `destroyed`: array of destroyed ids.
  - `notCreated`: array of `JMAPSetError`.
  - `notUpdated`: array of `JMAPSetError`.
  - `notDestroyed`: array of `JMAPSetError`.
- `JMAPSetCreated`
  - Public only with `JMAPSetResult`.
  - `creationID`: client creation id.
  - `identifier`: created object id.
  - `emailID`: created email id when applicable.
  - `identityID`: identity id when applicable.
  - `threadID`: thread id when applicable.
  - `sendAt`: scheduled send timestamp when applicable.
  - `undoStatus`: submission undo status when applicable.
  - `envelopeMailFrom`: `Address`.
  - `envelopeRcptTo`: array of `Address`.
  - `deliveryStatus`: array of `JMAPEmailSubmissionDeliveryStatus`.
  - `dsnBlobIDs`: map of DSN name/address to blob id.
- `JMAPSetError`
  - Public only with `JMAPSetResult`.
  - `identifier`: object id or creation id.
  - `type`: JMAP set error type.
  - `description`: server-provided description.
- `JMAPEmailSubmissionDeliveryStatus`
  - Public only with submission support.
  - `email`: recipient email.
  - `smtpReply`: SMTP reply string.
  - `delivered`: delivery status string.
  - `displayed`: display status string.

### Internal Only Unless Needed

These should be represented privately in `src/core/jmap` or hidden inside operations unless a concrete public use case appears.

- `JMAPMailboxGetResult`
  - Internal by default. Public operations can return `Array /* JMAPMailbox */`, plus optional `state`/`notFound` accessors on the operation if needed.
  - Fields if retained privately: `accountID`, `state`, `mailboxes`, `notFound`.
- `JMAPMessageGetResult`
  - Internal by default. Public operations can return `Array /* JMAPMessage */`, plus optional `state`/`notFound`.
  - Fields if retained privately: `accountID`, `state`, `messages`, `notFound`.
- `JMAPThreadGetResult`
  - Internal until thread operations are public.
  - Fields if retained privately: `accountID`, `state`, `threads`, `notFound`.
- `JMAPIdentityGetResult`
  - Internal until identity operations are public.
  - Fields if retained privately: `accountID`, `state`, `identities`, `notFound`.
- Request builder classes
  - Keep private, like Gmail request private classes.
  - Examples: message query options, message get properties, mailbox set items, email set items, copy items, import items, parse items, submission set items.
- Raw `JMAPCapability.json`
  - Public as a string for debugging/forward compatibility, but no public structured object should be added until MailCore exposes a typed use for that capability.
- Import/parse/search-snippet result item wrappers
  - Keep internal until import, parse, or snippet operations are part of the public API.
- Generic `mailjmap_request`/`mailjmap_response`
  - Keep internal. A custom operation can accept a minimal MailCore-owned method name/arguments abstraction instead of exposing libetpan JSON structs directly.

Objects used only by features outside v1, such as parse, copy, and snippets, can stay private until the corresponding public operations are added. Low-level import/set/submission mutation request items should also stay private even though v1 exposes simple draft/send operations.

### Full Field Reference

This reference includes fields for public-later/internal objects so implementation does not have to rediscover them from libetpan.

- `JMAPMailboxGetResult`
  - `accountID`: account id.
  - `state`: mailbox state string.
  - `mailboxes`: array of `JMAPMailbox`.
  - `notFound`: array of mailbox ids.
- `JMAPMessageGetResult`
  - `accountID`: account id.
  - `state`: email state string.
  - `messages`: array of `JMAPMessage`.
  - `notFound`: array of message ids.
- `JMAPThreadGetResult`
  - `accountID`: account id.
  - `state`: thread state string.
  - `threads`: array of `JMAPThread`.
  - `notFound`: array of thread ids.
- `JMAPIdentityGetResult`
  - `accountID`: account id.
  - `state`: identity state string.
  - `identities`: array of `JMAPIdentity`.
  - `notFound`: array of identity ids.
- `JMAPImportCreated`
  - `creationID`: client creation id.
  - `message`: imported `JMAPMessage`.
- `JMAPImportNotCreated`
  - `creationID`: client creation id.
  - `type`: error type.
  - `description`: server-provided description.
- `JMAPImportResult`
  - `accountID`: account id.
  - `oldState`: previous state.
  - `newState`: new state.
  - `created`: array of `JMAPImportCreated`.
  - `notCreated`: array of `JMAPImportNotCreated`.
- `JMAPEmailParseItem`
  - `blobID`: parsed blob id.
  - `message`: parsed `JMAPMessage`.
- `JMAPEmailParseResult`
  - `accountID`: account id.
  - `parsed`: array of `JMAPEmailParseItem`.
  - `notParsable`: array of blob ids.
- `JMAPSearchSnippet`
  - `emailID`: email/message id.
  - `subject`: subject snippet.
  - `preview`: preview snippet.
- `JMAPSearchSnippetGetResult`
  - `accountID`: account id.
  - `snippets`: array of `JMAPSearchSnippet`.
  - `notFound`: array of message ids.
- `JMAPSetCreated`
  - `creationID`: client creation id.
  - `identifier`: created object id.
  - `emailID`: created email id when applicable.
  - `identityID`: identity id when applicable.
  - `threadID`: thread id when applicable.
  - `sendAt`: scheduled send timestamp when applicable.
  - `undoStatus`: submission undo status when applicable.
  - `envelopeMailFrom`: `Address`.
  - `envelopeRcptTo`: array of `Address`.
  - `deliveryStatus`: array of `JMAPEmailSubmissionDeliveryStatus`.
  - `dsnBlobIDs`: map of DSN name/address to blob id.
- `JMAPSetError`
  - `identifier`: object id or creation id.
  - `type`: JMAP set error type.
  - `description`: server-provided description.
- `JMAPSetResult`
  - `accountID`: account id.
  - `oldState`: previous state.
  - `newState`: new state.
  - `created`: array of `JMAPSetCreated`.
  - `updated`: array of updated ids.
  - `destroyed`: array of destroyed ids.
  - `notCreated`: array of `JMAPSetError`.
  - `notUpdated`: array of `JMAPSetError`.
  - `notDestroyed`: array of `JMAPSetError`.
- `JMAPEmailSubmissionDeliveryStatus`
  - `email`: recipient email.
  - `smtpReply`: SMTP reply string.
  - `delivered`: delivery status string.
  - `displayed`: display status string.

## Async C++ Design

Create `src/async/jmap` with:

- `MCAsyncJMAP.h`
  - Umbrella header.
- `MCJMAPAsyncSession.h/.cpp`
  - Mirrors configuration from `JMAPSession`.
  - Owns one `JMAPAsyncConnection` initially. JMAP is HTTP-based and request batching can be added before multiple connections.
  - Creates operation objects.
  - Supports dispatch queue and operation queue callback like `IMAPAsyncSession`.
- `MCJMAPAsyncConnection.h/.cpp`
  - Owns one core `JMAPSession`.
  - Serializes operations and applies session configuration before first use.
  - Cancels current network work if libetpan exposes cancellation; otherwise cancellation suppresses callbacks only.
- `MCJMAPOperation.h/.cpp`
  - Base operation analogous to `IMAPOperation`.
  - Stores `ErrorCode`.
  - Optionally stores JMAP detail fields from the core session after failure.
- Public concrete operations:
  - `JMAPFetchMailboxesOperation`
  - `JMAPFetchMessagesOperation`
  - `JMAPQueryMessagesOperation`
  - `JMAPUploadOperation`
  - `JMAPDownloadOperation`
  - `JMAPCreateDraftOperation`
  - `JMAPSendOperation`
- Internal or generic operations:
  - connect/discover/custom can use `JMAPOperation` unless they need typed result access.
  - update/delete draft can use `JMAPOperation` unless they need typed result access.
  - session/account/capability fetch should populate `JMAPAsyncSession` properties or private structs instead of exposing `JMAPFetchSessionOperation`.

Each concrete operation should call one synchronous `JMAPSession` method, retain/copy the result, and expose getters for the Objective-C wrapper.

## Objective-C Design

Create `src/objc/jmap` with:

- `MCOJMAP.h`
  - Umbrella header.
- `MCOJMAPSession.h/.mm`
  - Wraps `mailcore::JMAPAsyncSession`.
  - Properties:
    - `sessionURL`
    - `domainOrEmail`
    - `username`
    - `OAuth2Token`
    - `timeout`
    - `checkCertificateEnabled`
    - `dispatchQueue`
    - `operationQueueRunning`
    - `operationQueueRunningChangeBlock`
  - Methods:
    - `connectOperation`
    - `discoverOperation`
    - `fetchMailboxesOperation`
    - `fetchMessagesOperationWithIds:properties:`
    - `queryMessagesOperationWithText:position:limit:`
    - `queryMessagesOperationWithMailboxID:position:limit:`
    - `uploadOperationWithData:contentType:accountID:`
    - `downloadOperationWithBlobID:name:accept:accountID:`
    - `createDraftOperationWithData:mailboxID:keywords:`
    - `updateDraftOperationWithMessageID:mailboxIDs:keywords:`
    - `deleteDraftOperationWithMessageID:`
    - `sendOperationWithMessageID:identityID:`
    - `sendOperationWithData:identityID:`
    - `customOperationWithMethodName:arguments:`
- Minimal ObjC data wrappers:
  - `MCOJMAPMailbox`
  - `MCOJMAPMessage`
  - `MCOJMAPPart`
  - `MCOJMAPMultipart`
  - `MCOJMAPMessagePart`
  - `MCOJMAPBlobUpload`
  - `MCOJMAPSubmission`
- ObjC operation wrappers:
  - Base `MCOJMAPOperation`
  - Result-specific operations with block-based `start:` methods, following `MCOIMAP…Operation`.
  - Query operations should return ids, state, and optional totals through their completion block or operation properties rather than exposing `MCOJMAPQueryResult`.
  - Session discovery/account/capability details should be exposed as session properties or simple collections first, not as public data wrapper classes.

Expose `MCOJMAP.h` from `src/objc/MCObjC.h` once the API compiles.

## Build Integration

Update all build surfaces that enumerate protocol files:

- `CMakeLists.txt`
- `build-mac/mailcore2.xcodeproj/project.pbxproj`
- `build-android/jni/Android.mk`
- umbrella headers:
  - `src/MailCore.h` indirectly through `MCCore.h`, `MCAsync.h`, `MCObjC.h`
  - add `MCJMAP.h`, `MCAsyncJMAP.h`, `MCOJMAP.h` to the appropriate existing umbrella headers.

Also verify whether the vendored/linked libetpan build currently includes the JMAP objects and any HTTP/curl dependency required by `mailjmap_http_curl`.

## Implementation Order

1. Build-system reconnaissance
   - Confirm how libetpan is linked in this repo and whether JMAP is compiled into the dependency.
   - Confirm exported header install/copy rules for new `MailCore/...` includes.
2. Core foundation
   - Add `src/core/jmap` directory, umbrella header, session object, basic error mapping.
   - Implement connect/discover/login/session-info.
   - Add session/account data objects.
3. Core Mail read path
   - Add mailbox, message, part, multipart, message-part, address, and query result objects.
   - Implement mailbox get, message query, message get.
   - Convert JMAP body structures into `AbstractPart` subclasses instead of exposing raw JMAP body-part structs.
4. Core blob and draft/send path
   - Implement upload/download.
   - Implement create/update/delete draft.
   - Implement send existing message id and send RFC822 data.
   - Keep JMAP import/set/submission request item classes private behind simple session methods.
5. Async layer
   - Add base async session/connection/operation.
   - Add operations for read, blob, draft, and send methods above.
6. Objective-C layer
   - Add wrappers for session, data objects, operations.
   - Add umbrella headers and block callbacks.
7. Integration and polish
   - Wire all build targets.
   - Add docs comments matching the style of existing `MCOIMAPSession.h`.
   - Add simple examples if the repo has a suitable examples area.

## Testing Plan

Start with compile and conversion tests, then add live smoke tests only if credentials are available.

- Unit-style tests
  - Error mapping from libetpan JMAP error constants to MailCore `ErrorCode`.
  - Conversion of synthetic libetpan structs to MailCore data objects.
  - ObjC wrapper property bridging.
- Build checks
  - CMake or existing project build for core/async/objc.
  - macOS framework build if available.
  - Android build-file syntax sanity if Android is supported in the current environment.
- Live smoke test, optional
  - Discover JMAP session from an email/domain.
  - Login with OAuth2 token.
  - Fetch session info.
  - Fetch mailboxes.
  - Query a small number of messages and fetch those messages.
  - Download one safe blob or upload a tiny text blob to a test account if the account supports it.
  - Create and delete a draft in a test account.
  - Send a test message only when explicit test-recipient configuration is present.

## Open Questions

- Which libetpan JMAP transport hook should MailCore use to implement IMAP-equivalent certificate validation?
- Does the libetpan JMAP transport expose logging hooks compatible with MailCore's existing connection logger?
- Is JMAP currently built into the libetpan dependency used by this repo on every supported platform?
- Should the first public API support only OAuth2, or also bearer-token-only sessions without a username?
- Which account should be the default when the session exposes multiple JMAP Mail accounts and no primary account is advertised?

## Risks

- JMAP's object model differs enough from IMAP that over-reusing IMAP classes may create misleading APIs.
- libetpan JMAP may depend on curl/HTTP pieces that are not currently linked on every MailCore platform.
- Cancellation semantics may be weaker than IMAP unless libetpan exposes HTTP cancellation.
- Objective-C API naming needs care so the first surface is small but does not paint future JMAP batching/submission support into a corner.

## Execution Status

Implemented in this branch:

- `src/core/jmap`
  - Minimal public objects: `JMAPSession`, `JMAPMailbox`, `JMAPMessage`, `JMAPPart`, `JMAPMultipart`, `JMAPMessagePart`, `JMAPBlobUpload`, `JMAPSubmission`.
  - `JMAPMessage` inherits `AbstractMessage` and uses `MessageHeader` for RFC822-style header fields.
  - Read APIs for mailbox fetch, message query, and message fetch.
  - Blob upload/download APIs.
  - Draft/send APIs for create/update/delete draft, send existing message, and send RFC822 data.
  - Certificate-validation toggle is exposed; actual TLS peer validation still depends on adding/exposing a libetpan JMAP HTTP transport hook.
- `src/async/jmap`
  - `JMAPAsyncSession`, base `JMAPOperation`, and typed result operations for mailbox/message/query/upload/download/draft/send.
  - Operation queue callback, dispatch queue support, cancellation plumbing, and session property propagation.
- `src/objc/jmap`
  - Minimal Objective-C wrappers for the session, data objects, base operation, and typed result operations.
  - Public session includes certificate-validation toggle and send/draft/upload/download operation factories.
- Build wiring
  - CMake source lists, include paths, public headers, and core/async/objc umbrella headers are wired.
  - Android `Android.mk` source/include groups are wired for `core/jmap` and `async/jmap`.
  - `build-mac/mailcore2.xcodeproj/project.pbxproj` includes JMAP core, async, and ObjC groups.
  - The Xcode project includes JMAP source build entries for the static OS X and iOS targets, plus public header copy entries for the JMAP C++ and ObjC headers.
  - Linux CMake now prefers the sibling `../libetpan/src/.libs` library when present, because the system `/usr/local/lib/libetpan.so` used in this workspace does not export JMAP symbols.
  - CMake configure passes.
  - Targeted generated compile commands for all `src/core/jmap/*.cpp` and `src/async/jmap/*.cpp` pass on this Linux host.
  - `libMailCore.a` builds successfully with the new JMAP core/async files.
  - `tests-jmap-link` is a permanent non-network compile/link smoke target that instantiates core and async JMAP objects.
  - `tests-jmap-link` is registered with CTest and runs successfully against the sibling JMAP-capable libetpan.
  - Full CMake build now passes against the sibling libetpan; the earlier ActiveSync live-test link failure was caused by the system `/usr/local/lib/libetpan.so` mismatch.
  - Dorian.home validation:
    - Worktree prepared at `~/Lydian/mailcore2`; build outputs/logs live under `~/Lydian/mailcore2/build-jmap-xcode-plan`.
    - The top-level `README.md` points iOS/OS X builds to `build-mac/README.md`; that documented path uses the Xcode project in `build-mac`.
    - `scripts/get-mac.sh`, the macOS dependency script invoked by CMake on Apple and referenced in the Xcode project run-script phase, has been run in the Dorian worktree and reports `ctemplate-osx` and `libetpan-osx` installed at the required versions.
    - The dependency-script-provisioned `Externals/libetpan-osx` is pre-JMAP: it has no `mailjmap.h` and no `mailjmap_new` symbol, so macOS needs a rebuilt JMAP-capable libetpan dependency before final JMAP link validation.
    - `xcodebuild -list -project build-mac/mailcore2.xcodeproj` passes, so the edited Xcode project parses.
    - Running the documented Xcode project command from `build-mac` fails immediately on Dorian because the project sets `MACOSX_DEPLOYMENT_TARGET=10.8`, while Xcode 27 supports macOS deployment targets from 12.0 to 27.0.x.
    - Re-running the same documented Xcode project build with the minimal command-line override `MACOSX_DEPLOYMENT_TARGET=12.0` reaches compilation.
    - With the repo-provisioned `Externals` dependencies, Xcode copies the new JMAP public headers into the build products, confirming the JMAP header copy phase is active.
    - Full Xcode compile is currently blocked before any JMAP source compile steps by pre-existing Xcode 27 compatibility/dependency issues in `MCString.cpp`, vendored ICU `char16_t`, and libxml2 structured-error callback signatures.

Still pending:

- Full macOS/Objective-C compile validation for `src/objc/jmap` after the existing Xcode 27 compatibility and dependency setup issues are fixed.
- Rebuild/provision `Externals/libetpan-osx` from a JMAP-capable libetpan revision so the macOS Xcode project can compile/link JMAP symbols.
- libetpan JMAP transport work for IMAP-equivalent certificate validation, unless the linked libetpan already exposes a usable TLS/curl verification hook.
