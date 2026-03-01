#!/usr/bin/env bash
set -euo pipefail

# time_left.sh
#
# Reads EID, TOKEN, and GITHUB (or USER) from .env, calls the request-access
# endpoint, and prints remaining time:
#   (requested_at + 30 minutes) - now
# Ensure permissions are correct: chmod u+x ./time_left.sh
# Usage:
#   ./time_left.sh
#   ./time_left.sh --url https://utcs429.com/request-access
#   ./time_left.sh --minutes 30

URL="https://utcs429.com/request-access"
MINUTES=30

usage() {
  echo "Usage: $0 [--url <endpoint>] [--minutes <N>]"
  exit 1
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --url) URL="$2"; shift 2;;
    --minutes) MINUTES="$2"; shift 2;;
    -h|--help) usage;;
    *) usage;;
  esac
done

if [[ ! -f .env ]]; then
  echo "Missing .env in current directory" >&2
  exit 1
fi

# Load .env (expects simple KEY=VALUE lines)
set -a
# shellcheck disable=SC1091
source .env
set +a

EID="${EID:-}"
TOKEN="${TOKEN:-}"
GITHUB="${GITHUB:-${USER:-}}"

if [[ -z "$EID" || -z "$TOKEN" || -z "$GITHUB" ]]; then
  echo "Missing EID, TOKEN, or GITHUB/USER in .env" >&2
  exit 1
fi

JSON="$(
  curl -sS "$URL" \
    -H 'content-type: application/json' \
    -d "{\"eid\":\"${EID}\",\"token\":\"${TOKEN}\",\"github_username\":\"${GITHUB}\"}"
)"

echo "$JSON"

# Feed JSON to python via stdin; keep python code in -c
printf '%s' "$JSON" | python3 -c '
import json, sys, time

minutes = int(sys.argv[1])
raw = sys.stdin.read().strip()
data = json.loads(raw)

requested_at = data["requested_at"]

now_ms = int(time.time() * 1000)
deadline_ms = requested_at + minutes * 60 * 1000
remaining_ms = deadline_ms - now_ms

def fmt(ms):
    if ms < 0:
        ms = -ms
        sign = "-"
    else:
        sign = ""
    s = ms // 1000
    m, s = divmod(s, 60)
    h, m = divmod(m, 60)
    if h:
        return f"{sign}{h}:{m:02d}:{s:02d}"
    return f"{sign}{m}:{s:02d}"

if remaining_ms <= 0:
    print(f"Time left: 0:00 (expired {fmt(-remaining_ms)} ago)")
else:
    print(f"Time left: {fmt(remaining_ms)}")
' "$MINUTES"
