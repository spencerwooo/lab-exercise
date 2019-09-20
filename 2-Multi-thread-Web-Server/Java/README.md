# Project2 Multi-thread Web Server

## Target

This project needs to build a multithread Web server. The server should handle more than one client request. The server should support HTTP1.1 protocol. First the server creates a main thread which waits for the connect request from a client. Once main thread establish a connection, it creates a thread to process http request. Then main thread return to wait new client request.

## Usage
