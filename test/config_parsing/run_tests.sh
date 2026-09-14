#!/bin/bash
# ---------------------------------------------------------------------------
# Config-block structure tests for webserv.
#
# Verifies the nginx-style rules enforced in srcs/config/Config.cpp:
#   * exactly one `http {}` block is REQUIRED
#   * `server` blocks are valid ONLY directly inside `http`
#   * `events {}` is OPTIONAL and must be a TOP-LEVEL SIBLING of `http`
#     (never nested inside http)
#   * unknown blocks are rejected at top level and directly inside http
#   * both brace styles work:  `http {`  and  `http` \n `{`
#
# How it works:
#   The server starts an event loop on success, so we can't just check the
#   exit code. Instead we run the binary with a 1s timeout and inspect stderr/
#   stdout: a rejected config prints "Config error: ..." and exits; an accepted
#   config prints "Number of ServerBlocks: N" then blocks in the poll loop
#   until the timeout kills it.
#
#   PASS criteria per case:
#     EXPECT_OK    -> output must NOT contain "Config error"
#     EXPECT_FAIL  -> output MUST contain "Config error" (optionally matching
#                     a substring given as the 3rd column)
# ---------------------------------------------------------------------------
set -u
cd "$(dirname "$0")/../.." || exit 1        # repo root
CASES="$(dirname "$0")/cases"

# Always build a FRESH binary from current sources into a temp path, so the
# test can never run against a stale ./bin/webserv. (Direct c++ invocation
# avoids `make clean` failing when objs/ is owned by another user on a mount.)
BIN="$(mktemp -u /tmp/webserv_test.XXXXXX)"
INC="-Iincludes -Iincludes/config -Iincludes/utils -Iincludes/exceptions \
     -Iincludes/http -Iincludes/server -Iincludes/cgi -Iincludes/core -Iincludes/debug"
echo "Building fresh test binary ..."
c++ -Wall -Wextra -Werror -std=c++98 $INC $(find srcs -name '*.cpp') -o "$BIN" \
    || { echo "BUILD FAILED"; exit 1; }
trap 'rm -f "$BIN"' EXIT

pass=0; fail=0
run() {                     # run <name> <EXPECT_OK|EXPECT_FAIL> [expected substring]
    name="$1"; expect="$2"; want="${3:-}"
    out="$(timeout 1 "$BIN" "$CASES/$name.conf" 2>&1)"
    got_err=0
    echo "$out" | grep -q "Config error" && got_err=1

    ok=1
    if [ "$expect" = "EXPECT_OK" ] && [ "$got_err" -eq 1 ]; then ok=0; fi
    if [ "$expect" = "EXPECT_FAIL" ] && [ "$got_err" -eq 0 ]; then ok=0; fi
    if [ -n "$want" ] && ! echo "$out" | grep -q "$want"; then ok=0; fi

    first="$(echo "$out" | grep -iE 'Config error|ServerBlocks' | head -1)"
    if [ "$ok" -eq 1 ]; then
        printf "  \033[0;32mPASS\033[0m  %-24s %s\n" "$name" "$first"; pass=$((pass+1))
    else
        printf "  \033[0;31mFAIL\033[0m  %-24s (expected %s%s) got: %s\n" \
               "$name" "$expect" "${want:+ /$want/}" "$first"; fail=$((fail+1))
    fi
}

echo "=== webserv config-structure tests ==="
run valid_events_before   EXPECT_OK
run valid_events_after    EXPECT_OK
run valid_no_events       EXPECT_OK
run valid_allman_braces   EXPECT_OK
run bad_no_http           EXPECT_FAIL
run bad_server_toplevel   EXPECT_FAIL "only allowed directly inside"
run bad_events_inside     EXPECT_FAIL "top level"
run bad_garbage_in_http   EXPECT_FAIL "Unknown block inside"
run bad_two_http          EXPECT_FAIL "Only one 'http'"

echo "--- repo configs (should all be OK) ---"
for c in config/webserv_basic.conf config/webserv_redirect.conf config/webserv_complete.conf; do
    out="$(timeout 1 "$BIN" "$c" 2>&1)"
    if echo "$out" | grep -q "Config error"; then
        printf "  \033[0;31mFAIL\033[0m  %-32s %s\n" "$c" "$(echo "$out"|grep -i 'Config error'|head -1)"; fail=$((fail+1))
    else
        printf "  \033[0;32mPASS\033[0m  %-32s %s\n" "$c" "$(echo "$out"|grep -i ServerBlocks|head -1)"; pass=$((pass+1))
    fi
done

echo "======================================="
echo "  passed: $pass   failed: $fail"
[ "$fail" -eq 0 ]
