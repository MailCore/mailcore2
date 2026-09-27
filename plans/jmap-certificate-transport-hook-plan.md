# JMAP Certificate Transport Hook Plan

## Goal

Add an explicit certificate-validation hook to libetpan's HTTP/JMAP transport layer and use it from MailCore's JMAP session so `JMAPSession::checkCertificateEnabled` behaves like IMAP/POP/SMTP/NNTP.

This is a two-repository change:

- `~/Sources/libetpan`: introduce the transport-level certificate policy and apply it in the HTTP backends used by JMAP.
- `/mnt/ai-data/Sources/mailcore2`: set that policy from `src/core/jmap/JMAPSession`, propagate it through async and Objective-C APIs, and map TLS failures to `ErrorCertificate`.

## Current State

MailCore already exposes the public JMAP certificate toggle:

- C++ core: `JMAPSession::setCheckCertificateEnabled(bool)` and `isCheckCertificateEnabled()`.
- C++ async: `JMAPAsyncSession::setCheckCertificateEnabled(bool)` forwards to the core session.
- Objective-C: `MCOJMAPSession.checkCertificateEnabled`.

The missing piece is below MailCore:

- `mailjmap_http_transport` exposes only `context`, `perform`, and `free`.
- `mailjmap_http_request` has URL/method/headers/body/content type/accept/timeout, but no TLS verification policy.
- The JMAP HTTP wrapper converts `mailjmap_http_request` into `mailhttp_request`.
- `mailhttp_request` also has no certificate-validation field.
- The curl backend validates certificates by default, but MailCore cannot explicitly enable/disable verification per JMAP session.
- The NSURLSession backend uses system default validation and maps certificate failures to `MAILHTTP_ERROR_TLS`, but it has no delegate/challenge path for a per-request or per-transport policy.
- `MAILHTTP_ERROR_TLS` is currently collapsed to `MAILJMAP_ERROR_HTTP`, so MailCore cannot distinguish TLS/certificate failures from generic HTTP failures.

## Design Choice

Use a simple boolean verification policy first, matching MailCore's existing `checkCertificateEnabled` semantics:

- `true`: verify peer certificate and hostname.
- `false`: skip peer certificate and hostname verification.

Keep this policy at the lower shared HTTP layer so it applies to all JMAP HTTP requests: session discovery, API calls, blob upload, and blob download.

Avoid exposing certificate objects or callbacks in the first version. MailCore's existing public API is boolean, and introducing a richer callback would create a larger cross-platform design problem than JMAP needs right now.

## libetpan API Changes

Add TLS verification state and setters to `src/data-types/mailhttp`.

### Public Header

In `src/data-types/mailhttp.h`:

- Add an `int check_certificate` field to `struct mailhttp_transport`, defaulting to enabled.
- Add public setter/getter functions:
  - `int mailhttp_transport_set_check_certificate(struct mailhttp_transport * transport, int enabled);`
  - `int mailhttp_transport_get_check_certificate(struct mailhttp_transport * transport);`

Optional but useful:

- Add equivalent JMAP wrapper functions in `src/low-level/jmap/mailjmap_http.h`:
  - `int mailjmap_http_transport_set_check_certificate(struct mailjmap_http_transport * transport, int enabled);`
  - `int mailjmap_http_transport_get_check_certificate(struct mailjmap_http_transport * transport);`

The JMAP wrapper functions avoid forcing MailCore to include `mailhttp.h` or know whether the current `mailjmap_http_transport` wraps a `mailhttp_transport`.

### Transport Construction

In `src/data-types/mailhttp.c`:

- Initialize `transport->check_certificate = 1` in `mailhttp_transport_new()`.
- Implement the setter/getter defensively:
  - return `MAILHTTP_ERROR_BAD_STATE` for a null transport in the setter.
  - return enabled by default for a null transport in the getter, so callers fail closed.

In `src/low-level/jmap/mailjmap_http.c`:

- When wrapping `mailhttp_transport`, preserve the common transport in `jmap_mailhttp_context`.
- Implement the JMAP setter/getter by reaching the wrapped `mailhttp_transport`.
- If a custom JMAP transport does not wrap `mailhttp_transport`, return `MAILJMAP_ERROR_BAD_STATE` from the setter and enabled from the getter.

### Error Mapping

In `src/low-level/jmap/mailjmap_types.h`:

- Add `MAILJMAP_ERROR_TLS` or `MAILJMAP_ERROR_CERTIFICATE`.
- Prefer `MAILJMAP_ERROR_TLS` if keeping libetpan protocol-neutral.

In `src/low-level/jmap/mailjmap_http.c`:

- Map `MAILHTTP_ERROR_TLS` to `MAILJMAP_ERROR_TLS`.
- Keep other HTTP failures mapped to `MAILJMAP_ERROR_HTTP`.

## Backend Behavior

### Curl

In `src/data-types/mailhttp_curl.c`, apply the policy before `curl_easy_perform()`:

- If enabled:
  - `CURLOPT_SSL_VERIFYPEER = 1L`
  - `CURLOPT_SSL_VERIFYHOST = 2L`
- If disabled:
  - `CURLOPT_SSL_VERIFYPEER = 0L`
  - `CURLOPT_SSL_VERIFYHOST = 0L`

Also keep mapping these curl errors to `MAILHTTP_ERROR_TLS`:

- `CURLE_SSL_CONNECT_ERROR`
- `CURLE_PEER_FAILED_VERIFICATION`
- any available host-verification or certificate-related curl constants supported by the repo's minimum curl version.

### NSURLSession

There are two acceptable implementation steps.

Step 1, minimal:

- Keep default NSURLSession validation when `check_certificate` is enabled.
- If disabled, use a session delegate that accepts server trust challenges for that transport/session.
- Continue mapping certificate/trust failures to `MAILHTTP_ERROR_TLS`.

Step 2, if Step 1 is too invasive for the first patch:

- Implement the curl backend fully.
- Make the NSURLSession setter return `MAILHTTP_ERROR_UNAVAILABLE` when disabling validation is requested.
- Keep default validation enabled.
- Document that disabling certificate checks is initially unsupported by the NSURLSession backend.

Since MailCore's Apple path is important, Step 1 is preferred before calling the feature complete.

## MailCore Changes

### Core JMAP Session

In `src/core/jmap/MCJMAPSession.cpp`:

- Include the new libetpan JMAP HTTP transport setter header.
- After `mailjmap_new()` creates the default transport, call the new libetpan session/transport hook before any network operation.
- The cleanest libetpan API would be a session-level function:
  - `int mailjmap_set_check_certificate(mailjmap * session, int enabled);`
- If libetpan only exposes transport-level functions, add or use a public way to reach/set the current JMAP HTTP transport.

Preferred MailCore behavior:

- `mCheckCertificateEnabled = true` should configure the transport to verify peer and hostname.
- `mCheckCertificateEnabled = false` should configure the transport to skip verification.
- If setting the policy fails while enabling validation, fail closed with `ErrorCertificate` or `ErrorConnection`.
- If setting the policy fails while disabling validation, return `ErrorConnection` rather than silently ignoring the caller's explicit setting.

Replace the current placeholder `JMAPSession::checkCertificate()` with a real transport-policy application point or remove it if validation is handled entirely by libetpan.

### Error Mapping

In `src/core/jmap/MCJMAPSession.cpp`:

- Map `MAILJMAP_ERROR_TLS`/`MAILJMAP_ERROR_CERTIFICATE` to `ErrorCertificate`.
- Preserve existing mappings for authentication, parse/protocol, capability, method, and generic connection errors.

### Async And Objective-C

No public API additions should be needed because the toggle is already exposed.

Verify that:

- `JMAPAsyncSession::setCheckCertificateEnabled()` still updates the underlying core session before operations run.
- `MCOJMAPSession.checkCertificateEnabled` still bridges through to `JMAPAsyncSession`.
- Operation callbacks return `MCOErrorCertificate` for certificate failures.

## Implementation Order

1. libetpan shared HTTP transport
   - Add `check_certificate` to `mailhttp_transport`.
   - Add setter/getter declarations and definitions.
   - Initialize the default to enabled.
2. libetpan curl backend
   - Apply `CURLOPT_SSL_VERIFYPEER` and `CURLOPT_SSL_VERIFYHOST` from the transport policy.
   - Broaden curl TLS error mapping where available.
3. libetpan JMAP wrapper
   - Add JMAP-facing setter/getter or session-level setter.
   - Propagate the policy from JMAP transport to wrapped `mailhttp_transport`.
   - Map `MAILHTTP_ERROR_TLS` to a dedicated JMAP TLS/certificate error.
4. libetpan NSURLSession backend
   - Implement disabled-validation behavior with a delegate, or explicitly return unsupported for disabling.
   - Keep enabled validation as the default system behavior.
5. MailCore core integration
   - Call the libetpan hook from `JMAPSession::setup()` and whenever the toggle changes after setup.
   - Map the new libetpan TLS error to `ErrorCertificate`.
6. MailCore async/ObjC verification
   - Confirm existing property forwarding is enough.
   - Add smoke tests for the public toggle.
7. Documentation and plan cleanup
   - Update `plans/jmap-api-plan.md` after implementation to remove the placeholder limitation.

## Tests

### libetpan

Add non-network tests if the test harness supports injecting a fake transport:

- New `mailhttp_transport` defaults to checking certificates.
- Setter/getter round-trip enabled and disabled.
- JMAP wrapper setter/getter round-trip when wrapping a `mailhttp_transport`.
- `MAILHTTP_ERROR_TLS` maps to the new JMAP TLS/certificate error.

Add live/manual tests where practical:

- Curl backend, enabled validation, known-good HTTPS endpoint succeeds.
- Curl backend, enabled validation, bad/self-signed certificate endpoint returns `MAILHTTP_ERROR_TLS` or JMAP TLS error.
- Curl backend, disabled validation, same bad/self-signed endpoint succeeds far enough to prove TLS was not rejected.
- NSURLSession backend equivalent on macOS.

### MailCore

Add or extend the non-network JMAP smoke test:

- Set `JMAPSession.checkCertificateEnabled` true/false before setup.
- Set it after setup and verify no crash and no ownership problems.
- Verify async and Objective-C property forwarding where the platform can compile it.

Add optional live tests behind environment variables:

- `JMAP_BAD_CERT_SESSION_URL` with validation enabled should return `ErrorCertificate`.
- Same URL with validation disabled should not return `ErrorCertificate`; it may still fail later for HTTP/auth reasons.
- Normal Fastmail or other JMAP endpoint with validation enabled should continue to pass connect/discover/login.

## Compatibility

- Default behavior remains certificate validation enabled, matching current curl and NSURLSession defaults.
- Existing libetpan callers are source-compatible if the new struct field is appended and new functions are additive.
- Binary compatibility depends on whether `struct mailhttp_transport` is considered public ABI. If it is, prefer storing policy in backend-private data or add only public functions without requiring external struct allocation.
- MailCore should use libetpan feature detection if this lands behind a version macro:
  - compile with the new hook when available.
  - keep the current default-validating fallback otherwise, while documenting that the disable toggle cannot be honored on older libetpan.

## Open Questions

- Is `struct mailhttp_transport` public ABI-stable, or can a field be safely appended?
- Should libetpan name the error `MAILJMAP_ERROR_TLS` or `MAILJMAP_ERROR_CERTIFICATE`?
- Should the JMAP public API expose a session-level setter instead of transport-level setters?
- Do we need per-request certificate policy, or is per-transport/per-session policy enough for all current JMAP usage?
- For NSURLSession, is disabling verification acceptable in libetpan, or should Apple platforms keep validation always enabled and report unsupported when asked to disable it?

## Acceptance Criteria

- MailCore JMAP with `checkCertificateEnabled = true` validates peer certificate and hostname for connect, discover, API, upload, and download calls.
- MailCore JMAP with `checkCertificateEnabled = false` applies that policy intentionally, or returns an explicit error on a backend that cannot support it.
- Certificate failures are surfaced as `ErrorCertificate`/`MCOErrorCertificate`, not generic connection/HTTP errors.
- The default remains secure without requiring callers to set anything.
- Existing JMAP connect/query/blob/draft/send tests still pass.
