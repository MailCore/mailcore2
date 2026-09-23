#!/bin/sh
#
# Live MailCore C++ ActiveSync smoke test.

set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
top_dir=$(CDPATH= cd -- "$script_dir/.." && pwd)

libetpan_dir=${LIBETPAN_SOURCE_DIR:-"/home/dvh/Sources/libetpan"}
config_file=${LIBETPAN_ACTIVESYNC_OAUTH_CONFIG:-"$libetpan_dir/tests/activesync-ms-oauth.local.json"}
token_json=${LIBETPAN_ACTIVESYNC_TOKEN_JSON:-}
state_file=${MAILCORE_ACTIVESYNC_STATE_FILE:-}
server=${LIBETPAN_ACTIVESYNC_SERVER:-}
login=${LIBETPAN_ACTIVESYNC_LOGIN:-}
build_dir=${MAILCORE2_BUILD_DIR:-"$top_dir/build"}

usage() {
  cat <<EOF
usage: $(basename "$0") [--config PATH] [--token-json PATH] [--state-file PATH]
       [--server URL] [--login USER] [--build-dir PATH]

Environment overrides:
  LIBETPAN_SOURCE_DIR
  LIBETPAN_ACTIVESYNC_OAUTH_CONFIG
  LIBETPAN_ACTIVESYNC_TOKEN_JSON
  LIBETPAN_ACTIVESYNC_SERVER
  LIBETPAN_ACTIVESYNC_LOGIN
  MAILCORE_ACTIVESYNC_STATE_FILE
  MAILCORE2_BUILD_DIR
EOF
}

while [ $# -gt 0 ]; do
  case "$1" in
    --config)
      config_file=$2
      shift 2
      ;;
    --token-json)
      token_json=$2
      shift 2
      ;;
    --state-file)
      state_file=$2
      shift 2
      ;;
    --server)
      server=$2
      shift 2
      ;;
    --login)
      login=$2
      shift 2
      ;;
    --build-dir)
      build_dir=$2
      shift 2
      ;;
    --help)
      usage
      exit 0
      ;;
    *)
      usage >&2
      exit 2
      ;;
  esac
done

json_value() {
  key=$1
  file=$2
  python3 - "$key" "$file" <<'PY'
import json
import sys

key = sys.argv[1]
path = sys.argv[2]
try:
    with open(path, "r", encoding="utf-8") as f:
        value = json.load(f).get(key)
except FileNotFoundError:
    value = None
if value is not None:
    print(value)
PY
}

if [ -f "$config_file" ]; then
  if [ -z "$token_json" ]; then
    token_json=$(json_value token_json "$config_file")
  fi
  if [ -z "$state_file" ]; then
    state_file=$(json_value mailcore_state_file "$config_file")
  fi
  if [ -z "$server" ]; then
    server=$(json_value activesync_url "$config_file")
  fi
  if [ -z "$login" ]; then
    login=$(json_value login "$config_file")
  fi
fi

if [ -z "$token_json" ]; then
  token_json="/tmp/libetpan-eas-token.json"
fi
if [ -z "$state_file" ]; then
  state_file="/tmp/mailcore2-activesync-state.txt"
fi
if [ -z "$server" ]; then
  server="https://eas.outlook.com/Microsoft-Server-ActiveSync"
fi
if [ -z "$login" ]; then
  login=$(json_value login "$token_json")
fi
if [ -z "$login" ]; then
  printf '%s\n' "error: set LIBETPAN_ACTIVESYNC_LOGIN, pass --login, or set login in $config_file" >&2
  exit 2
fi

client_id=$(json_value client_id "$token_json")
tenant=$(json_value tenant "$token_json")
oauth_version=$(json_value oauth_version "$token_json")
servertype=$(json_value servertype "$token_json")
scope=$(json_value scope "$token_json")
resource=$(json_value resource "$token_json")

if [ -n "$client_id" ]; then
  set -- "$libetpan_dir/tests/activesync-ms-device-oauth.py" \
    --config "$config_file" \
    --client-id "$client_id" \
    --token-json "$token_json" \
    --refresh \
    --probe-activesync \
    --login "$login" \
    --activesync-url "$server"
  if [ -n "$tenant" ]; then
    set -- "$@" --tenant "$tenant"
  fi
  if [ -n "$oauth_version" ]; then
    set -- "$@" --oauth-version "$oauth_version"
  fi
  if [ -n "$servertype" ]; then
    set -- "$@" --servertype "$servertype"
  fi
  if [ -n "$scope" ]; then
    set -- "$@" --scope "$scope"
  fi
  if [ -n "$resource" ]; then
    set -- "$@" --resource "$resource"
  fi
else
  set -- "$libetpan_dir/tests/activesync-ms-device-oauth.py" \
    --config "$config_file" \
    --token-json "$token_json" \
    --refresh \
    --probe-activesync \
    --login "$login" \
    --activesync-url "$server"
fi

"$@" >/tmp/mailcore2-activesync-oauth-refresh.log

access_token=$(json_value access_token "$token_json")
if [ -z "$access_token" ]; then
  printf '%s\n' "error: OAuth helper did not write an access_token to $token_json" >&2
  exit 1
fi

cmake --build "$build_dir" --target tests-activesync-live

test_binary="$build_dir/tests/tests-activesync-live"
if [ ! -x "$test_binary" ]; then
  printf '%s\n' "error: expected test binary at $test_binary" >&2
  exit 1
fi

printf 'MailCore ActiveSync server=%s\n' "$server" >&2
printf 'MailCore ActiveSync login=%s\n' "$login" >&2
printf 'MailCore ActiveSync token_json=%s\n' "$token_json" >&2
printf 'MailCore ActiveSync state_file=%s\n' "$state_file" >&2

"$test_binary" \
  --server "$server" \
  --login "$login" \
  --oauth-token "$access_token" \
  --state-file "$state_file"
