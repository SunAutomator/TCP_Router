#ifndef tcp_server_h
#define tcp_server_h
#pragma once

#include "../model/message.h"

#include <winsock2.h>
#include <stdio.h>

typedef struct TcpServer {
    SOCKET socket;
} TcpServer;

typedef struct TcpClient {
    SOCKET socket;
} TcpClient;

TcpServer *tcp_server_create(int port);

void tcp_server_listen(TcpServer *server);
TcpClient *tcp_server_accept(TcpServer *server);

Message *tcp_server_read_message(TcpClient *client);

void tcp_server_forward_message(TcpServer *server, Message *message);
void tcp_server_respond(TcpClient *client, Message *message);

void tcp_server_destroy(TcpServer *server);

#endif // tcp_server_h
