// tcp_server.c

#include "tcp_server.h"

// Create a server socket and bind it to the specified port
TcpServer *tcp_server_create(int port) {
    TcpServer *server = malloc(sizeof(TcpServer));
    struct sockaddr_in server_address;

    // Create socket
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        free(server);
        return NULL;
    }

    // Prepare the sockaddr_in structure
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind
    if (bind(server->socket, (struct sockaddr *)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        free(server);
        return NULL;
    }

    return server;
}

// Make the server listen for connections
void tcp_server_listen(TcpServer *server) {
    if (listen(server->socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(server->socket);
        return;
    }
}

// Accept a client connection
TcpClient *tcp_server_accept(TcpServer *server) {
    SOCKET clientSocket = accept(server->socket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed with error: %d\n", WSAGetLastError());
        return NULL;
    }
    TcpClient *client = (TcpClient*)malloc(sizeof(TcpClient));
    client->socket = clientSocket;
    return client;
}

// Read a message from a client
Message *tcp_server_read_message(TcpClient *client) {
    char buffer[1024]; // Adjust buffer size according to your needs
    int bytesReceived = recv(client->socket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        // Process the received data and build the message
        Message *message = parse_message(buffer, bytesReceived);
        return message;
    } else if (bytesReceived == 0) {
        printf("Connection closing...\n");
    } else {
        printf("Recv failed with error: %d\n", WSAGetLastError());
    }
    return NULL;
}

// Forward the message to the next server *connect to next server socket*
void tcp_server_forward_message(TcpServer *server, Message *message) {
    // Implement logic to forward the message to the next server, e.g., using a separate client socket
}

// Respond to the client with the status of the message
void tcp_server_respond(TcpClient *client, Message *message) {
    // Build a response message based on the input message's status
    char response[1024];
    // Populate 'response' based on 'message'...

    // Send the response to the client
    send(client->socket, response, strlen(response), 0);
}

// Destroy the server socket
void tcp_server_destroy(TcpServer *server) {
    closesocket(server->socket);
    free(server);
}
