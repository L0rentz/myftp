# FTP Server (my_ftp)
* binary name: **myftp**
* language: **C**
* compilation: via **Makefile** (all, client, server, clean, fclean, re)

## Description

The goal of this project is to create a FTP server.  
The network communication is achieved through the use of TCP sockets.

## Server

```
USAGE: ./myftp port path
    port is the port number on which the server socket listens
    path is the path to the home directory for the Anonymous user
```

The server is able to handle several clients at the same time by using select for command management and fork for data transfer  
  
The server has an authentication with an Anonymous account and an empty password.  
Data transfers uses active or passive mode.  

## Client

You can use any ftp client or the one that I've done with ```make client``` or ```make```

## Commands

```
USER <SP> <username> <CRLF>   : Specify user for authentication
PASS <SP> <password> <CRLF>   : Specify password for authentication

CWD  <SP> <pathname> <CRLF>   : Change working directory
CDUP <CRLF>                   : Change working directory to parent directory

QUIT <CRLF>                   : Disconnection

DELE <SP> <pathname> <CRLF>   : Delete file on the server

PWD  <CRLF>                   : Print working directory

PASV <CRLF>                   : Enable "passive" mode for data transfer

PORT <SP> <host-port> <CRLF>  : Enable "active" mode for data transfer


HELP [<SP> <string>] <CRLF>   : List available commands

NOOP <CRLF>                   : Do nothing

(the following are commands using data transfer )

RETR <SP> <pathname> <CRLF>   : Download file from server to client
STOR <SP> <pathname> <CRLF>   : Upload file from client to server
LIST [<SP> <pathname>] <CRLF> : List files in the current working directory 
```
