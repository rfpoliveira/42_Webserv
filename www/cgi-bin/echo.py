#!/usr/bin/env python3
import os, sys
length = int(os.environ.get("CONTENT_LENGTH", 0))
body = sys.stdin.read(length) if length else ""
print("Content-Type: text/plain")
print()
print("METHOD:", os.environ.get("REQUEST_METHOD"))
print("QUERY:", os.environ.get("QUERY_STRING"))
print("BODY:", body)
