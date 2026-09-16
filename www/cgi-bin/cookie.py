#!/usr/bin/env python3
import os
print("Content-Type: text/plain")
print()
print("HTTP_COOKIE:", os.environ.get("HTTP_COOKIE", "NOT SET"))
