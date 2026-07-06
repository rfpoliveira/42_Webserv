*This project has been created as part of the 42 curriculum by rpedrosa and mreinald.*

## Description
[cite_start]Webserv is a custom HTTP/1.1 server written in C++ 98. The primary goal of this project is to understand the inner workings of the Hypertext Transfer Protocol (HTTP) by implementing a non-blocking server capable of handling multiple client connections simultaneously. [cite: 4, 91]

The server is designed to:
* [cite_start]Process and deliver static web pages (HTML, CSS, JS, etc.)[cite: 127].
* [cite_start]Handle file uploads from clients[cite: 128].
* [cite_start]Support GET, POST, and DELETE methods[cite: 129].
* [cite_start]Execute CGI scripts (e.g., Python, PHP) to handle dynamic content[cite: 150, 158].
* [cite_start]Manage multiple ports and configurations via a dedicated configuration file[cite: 134, 144].

## Features
* [cite_start]**Non-blocking I/O:** The server uses a single `poll()` (or equivalent like `select`, `kqueue`, or `epoll`) to manage all I/O operations, ensuring it never hangs[cite: 110, 111, 113].
* [cite_start]**Resilience:** Designed to remain operational even under stress or memory pressure[cite: 28, 133].
* [cite_start]**Configuration:** Flexible setup inspired by NGINX, allowing users to define error pages, client body limits, and route-specific rules[cite: 143, 145, 146].
* [cite_start]**CGI Support:** Full communication between the server and scripts using environment variables and pipes[cite: 152, 153].

## Instructions

### Compilation
[cite_start]The project includes a Makefile with the mandatory rules: `all`, `clean`, `fclean`, and `re`[cite: 32]. To compile the server, run:

```bash
make
```
### Execution
Once compiled, run the executable with a configuration file as an argument:  

```bash
./webserv [path_to_configuration_file]
```

## Resources

### Documentation & References

RFC 2616 (HTTP/1.1): Used to understand protocol specifications and header structures.  
Beej's Guide to Network Programming: A primary resource for socket programming in C++.
NGINX Documentation: Used as a reference point for configuration file logic and header behavior.  

## AI Usage 
Disclosure in accordance with the project rules, AI tools (such as ChatGPT/Copilot) were utilized for the following tasks:

Debugging: Explaining complex error messages related to socket multiplexing (poll()).
Technical Explanations: Clarifying the specific behavior of CGI environment variables.

Refinement: All AI-generated logic was manually reviewed, tested with peers, and modified to ensure compliance with the C++98 standard. 

useful links: 
https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7


https://datatracker.ietf.org/doc/html/rfc9110


https://datatracker.ietf.org/doc/html/rfc9112

https://github.com/1337fury/42Webserv

https://beej.us/guide/bgnet/html/#client-server-background


https://docs.nginx.com/nginx/admin-guide/basic-functionality/managing-configuration-files/

https://www.digitalocean.com/community/tutorials/understanding-the-nginx-configuration-file-structure-and-configuration-contexts

https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
