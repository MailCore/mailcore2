# IMAP IDLE Stress Test Plan

## Goal

Add a Linux/macOS-only regression stress test for IMAP IDLE cancellation and teardown races. The test should hold MailCore in a controlled `IDLE` state using a fake local IMAP server, then repeatedly race cancel/disconnect/session teardown against that state.

Implementation status: initial Linux/macOS C++ stress harness added in `tests/test-imap-idle.cpp` and wired into the `tests` executable behind `MAILCORE_RUN_IMAP_IDLE_STRESS`.

The test should verify that:

- `cancelAllOperations()` wakes an active `IMAPIdleOperation` quickly.
- Disconnect/teardown does not free the libetpan `mailimap` or stream while IDLE is still using it.
- The process does not crash or hang under repetition, especially under ASan/TSan.

## Scope

Implement the fake server in C++ inside the native test target.

Supported initially:

- Linux
- macOS

Out of scope for the first pass:

- Windows/Winsock support
- TLS
- Real IMAP server integration
- Full IMAP protocol compliance

## Test Location

Prefer a dedicated test file:

```text
tests/test-imap-idle.cpp
```

Then update:

```text
tests/CMakeLists.txt
```

to include the new file in the existing `tests` executable.

If wiring a new file is inconvenient, place the first version in:

```text
tests/test-all.cpp
```

but keep the fake server helper isolated so it can be moved later.

## Fake IMAP Server

Use a small blocking TCP server implemented with POSIX sockets:

- `socket`
- `bind`
- `listen`
- `accept`
- `recv`
- `send`
- `close`

Use `pthread_t`, `pthread_mutex_t`, and `pthread_cond_t` for synchronization with the test thread.

Bind to `127.0.0.1` on port `0`, then read the assigned port with `getsockname()`.

## Minimal Protocol

The server only needs enough protocol to drive MailCore into `IDLE`.

Expected conversation:

```text
S: * OK fake imap ready
C: <tag> CAPABILITY
S: * CAPABILITY IMAP4rev1 IDLE
S: <tag> OK CAPABILITY done
C: <tag> LOGIN ...
S: <tag> OK LOGIN done
C: <tag> SELECT INBOX
S: * 0 EXISTS
S: * 0 RECENT
S: * OK [UIDVALIDITY 1]
S: * OK [UIDNEXT 1]
S: <tag> OK SELECT done
C: <tag> IDLE
S: + idling
```

After `+ idling`, the server should stay quiet until either:

```text
C: DONE
S: <idle-tag> OK IDLE done
```

or the client closes the socket.

The server should also tolerate:

- `NOOP`
- `LOGOUT`
- socket close during teardown

## Server Synchronization State

Expose these thread-safe signals:

- `listening`: server has bound and is ready; includes assigned port.
- `clientAccepted`: client socket accepted.
- `idleEntered`: server received `IDLE` and sent `+ idling`.
- `doneReceived`: server received `DONE`.
- `connectionClosed`: client closed the socket.
- `serverError`: protocol or socket error.

Each wait should have a short timeout so failures do not hang forever.

## Test A: Cancel Wakes IDLE

Purpose: verify `IMAPIdleOperation::cancel()` wakes IDLE through `cancelAllOperations()`.

Steps:

1. Start fake IMAP server.
2. Create `IMAPAsyncSession`.
3. Configure:
   - hostname: `127.0.0.1`
   - port: fake server port
   - connection type: cleartext
   - username/password: any non-empty values
   - maximum connections: `1`
4. Start `idleOperation("INBOX", 0)`.
5. Wait for server `idleEntered`.
6. Call `session->cancelAllOperations()`.
7. Assert the operation queue stops within 2 seconds.
8. Assert the server saw `DONE` or socket close.
9. Assert no crash.

Run this for a default of 25-50 iterations.

## Test B: Disconnect During IDLE

Purpose: verify `IMAPSession::unsetup()` interrupts/cancels and waits before freeing `mImap`.

Steps:

1. Start fake IMAP server.
2. Start `idleOperation("INBOX", 0)`.
3. Wait for server `idleEntered`.
4. Start disconnect through the normal async path, for example `disconnectOperation()->start()`, or use the closest existing teardown path used by clients.
5. Assert disconnect completes within 2 seconds.
6. Assert the server saw `DONE` or socket close.
7. Assert no crash.

Run this for a default of 25-50 iterations.

## Test C: Destroy Session During IDLE

Purpose: approximate client lifecycle crashes where the async session is released while IDLE is active.

Steps:

1. Start fake IMAP server.
2. Start `idleOperation("INBOX", 0)`.
3. Wait for server `idleEntered`.
4. Call `cancelAllOperations()`.
5. Release/destroy the async session using the normal ownership pattern for the test target.
6. Assert no crash/hang.

This test can be added after Test A and Test B are stable.

## Stress Controls

Use a default iteration count suitable for CI:

```text
50
```

Allow override with an environment variable:

```text
MAILCORE_IDLE_STRESS_ITERS=1000
```

Use short per-iteration timeouts:

```text
2 seconds
```

## Sanitizer Verification

The highest-value verification is running this test target under ASan and TSan.

Expected behavior before the fix:

- Cancel path may hang until the IDLE timeout.
- Teardown path may occasionally crash or report use-after-free/data race under sanitizers.

Expected behavior after the fix:

- All iterations complete quickly.
- No sanitizer crash or race report in the IDLE lifecycle.

## Implementation Notes

- Keep the fake server deliberately small and deterministic.
- Parse commands line by line.
- Preserve the IMAP tag for tagged responses.
- For `IDLE`, remember the IDLE command tag so `DONE` can respond with `<tag> OK IDLE done`.
- Treat socket close during teardown as acceptable once `idleEntered` has happened.
- Avoid sleeping as synchronization; use condition variables.
- Gate the test with `#if !defined(_MSC_VER)` or a Linux/macOS-specific CMake condition.
