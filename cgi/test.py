#!/usr/bin/python3
import os
import sys


print("<html>")
print("<head><title>CGI Python Test</title></head>")
print("<body>")
print("<h1>Hello World from Python CGI!</h1>")

print("<h2>Environment Variables:</h2>")
print("<ul>")

for key, value in os.environ.items():
    print(f"<li><b>{key}</b>: {value}</li>")

print("</ul>")

if "CONTENT_LENGTH" in os.environ:
    try:
        length = int(os.environ["CONTENT_LENGTH"])
        body = sys.stdin.read(length)
        print("<h2>Post Body:</h2>")
        print(f"<p>{body}</p>")
    except Exception as e:
        print(f"<p>Error reading body: {e}</p>")

print("</body>")
print("</html>")