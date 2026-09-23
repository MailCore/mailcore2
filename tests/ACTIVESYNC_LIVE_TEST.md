# MailCore ActiveSync live test

`mailcore-activesync-live-test.sh` refreshes the same Microsoft OAuth token
used by the libetpan ActiveSync tests, then runs the MailCore C++ ActiveSync
wrapper against the live server.

Defaults:

```
libetpan checkout: /home/dvh/Sources/libetpan
OAuth config:      /home/dvh/Sources/libetpan/tests/activesync-ms-oauth.local.json
token JSON:        /tmp/libetpan-eas-token.json
MailCore state:    /tmp/mailcore2-activesync-state.txt
server:            https://eas.outlook.com/Microsoft-Server-ActiveSync
```

Run:

```
./tests/mailcore-activesync-live-test.sh --login dinhvh79@outlook.com
```

The test builds the opt-in `tests-activesync-live` CMake target and checks:

- OAuth token refresh through libetpan's helper.
- ActiveSync OAuth login through `mailcore::ActiveSyncSession`.
- `OPTIONS`, `FolderSync`, `GetItemEstimate`, and bounded Inbox `Sync`.

The test prints only status and count summaries. It does not print OAuth token
contents or message bodies.
