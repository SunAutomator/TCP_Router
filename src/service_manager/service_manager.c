#include "service_manager.h"

#include <windows.h>
#include <stdio.h>

TcpServer *server;
DatabaseConnection db_conn;

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) {
    
    // Register the handler function for the service
    SERVICE_STATUS_HANDLE hStatus = RegisterServiceCtrlHandler("TCP_Router", ServiceCtrlHandler);
    if (hStatus == 0) {
        return;
    }

    // Connect to the database
    db_connection_info_t connection_info = get_db_connection_config();
    db_conn = db_connect(&connection_info);
    if (db_conn == NULL) {
        printf("Database connection failed\n");
        return;
    }

    // Inform the service control manager that the service is running
    SERVICE_STATUS status = {0};
    status.dwServiceType = SERVICE_WIN32;
    status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &status);

    // thread {

    // Create a server socket and bind it to the specified port
    int port = get_server_port_config();
    server = tcp_server_create(port);
    if (server == NULL) {
        printf("Server creation failed\n");
        db_disconnect(db_conn);
        return;
    }

    // Perform the server operations
    // loop {

    tcp_server_listen(server);
    TcpClient *client = tcp_server_accept(server);
    Message *message = tcp_server_read_message(client);

    record_message_in_database(db_conn, message, NULL);
    
    // *Identify the destination server using dictionary read from configuration file*

    tcp_server_forward_message(server, message);
    tcp_server_respond(client, message);
    
    record_response_in_database(db_conn, message, NULL);
    // } loop

    // } thread
}

// Control handler function
VOID WINAPI ServiceCtrlHandler(DWORD fdwControl) {
    switch (fdwControl) {
        case SERVICE_CONTROL_SHUTDOWN:
            tcp_server_destroy(server);
            db_disconnect(db_conn);
            break;
        default:
            break;
    }
}

int initialize_service(int argc, char *argv[]) {
    // Register the handler function for the service
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { "TCP_Router", ServiceMain },
        { NULL, NULL }
    };

    // Start the service control dispatcher
    if (!StartServiceCtrlDispatcher(serviceTable)) {
        // Handle error
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
