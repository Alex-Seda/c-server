# Server in C

This project shows my study and experimentation as I learn to make a web server in C.

## The Goal
The goal was to get a working website without using any online guides.
Documentation and forums were the primary source of research, with minimal AI usage (and only for explaining a concept, not helping solve anything).

Ideally, by the end of the project, I will be able to sit down and recreate my project without internet access whatsoever.
To achieve this, I will be required to deeply understand C, unix sockets, and networking, along with HTTP requests and response formats.

## Logic Flow
After setting up all necessary variables and files, the server runs the following loop:
1. Wait for connection request on listening socket
1. Pass successful connection to an active socket to pass requests and response through
1. Read request from new active socket
1. Parse Request to determine proper response
1. Formulate response
1. Send response back to requestor
1. Close the connection socket

## File System Organization
```
/
│
├─ public/						    (Webpages in HTML)
│   ├─ about.html						(About page)
│   └─ index.html						(Home page)
│
├─ services/						(Logic called by "server.c")
│   ├─ decodeHeader.c					(Logic to parse request headers)
│   ├─ decodeHeader.h
│   ├─ headers.c					    (Generic header includes and HeaderFields struct definition)
│   ├─ helpers.c					    (Helper function(s) separated for code modularity and readability)
│   ├─ helpers.h
│   ├─ parseRequest.c					(Logic called by main loop to take a request, parse through the header, determine response, and generate response)
│   └─ parseRequest.h
│
├─ Makefile							(Definitions for make commands, i.e. "make run" and "make clean")
│
├─ README.md						(Project Documentation)
│
├─ server.c							(Logic called by "server.c")
│
└─ server.h
```

## How to Run the Code
All code is run using unix "make" commands.

Run "make run" to compile and run the project. Then you can visit "localhost:8080/" to see the home page.

Press "Ctrl+C" to stop the server at any time.

Run "make clean" to clean up all the object files and executable after you are done.

## Adding Pages
The current way that the server parses requests is by trying to find a page matching the URL you request.

For instance, "http://localhost:8080/about" will tell the server to look for "public/about.html" and return that file's contents. When the server does not find a page, it just returns a 404 page (defined in "services/parseRequest.c").

Therefore, to add pages, you need only place a file in "public/" with the name you would like it accessed at, and the server will find it. For example, if you would like a "http://localhost:8080/contact" page, you would add "contact.html" to the "public/" directory.

## Current Progress:
- Makefile is used to compile and run the server
- Boilerplate Client and Server compile and run
- Server responds with dynamically contructed response based on html file
- Server parses requests
- Server respond differently to different requests (including 404 when a requested page is not found)

## Next Steps:
- Security tighten existing code (prevent open/read race conditions with files, inspect other code)
- Add threading to handling requests so that more requests can be handled concurrently
- Rate limiting

