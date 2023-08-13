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

    initialize_database();

    // Inform the service control manager that the service is running
    SERVICE_STATUS status = {0};
    status.dwServiceType = SERVICE_WIN32;
    status.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &status);

    initialize_server();

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
}

// Control handler function
VOID WINAPI ServiceCtrlHandler(DWORD fdwControl) {
    switch (fdwControl) {
        case SERVICE_CONTROL_SHUTDOWN:
            tcp_server_destroy(server);
            db_disconnect(db_conn);
            close_logger();
            break;
        default:
            break;
    }
}

int initialize_service(int argc, char *argv[]) {
    // Initialize the logger
    init_logger();

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

void initialize_database() {
    // Read database configuration file
    char* db_config_data = get_decrypted_config("repository.config");
    db_connection_info_t connection_info;
    if (db_config_data) {
        if (sscanf(db_config_data, "%s %d %s %s %s", connection_info.host, &connection_info.port,
               connection_info.user, connection_info.password, connection_info.database) != 5) {
            // Handle parsing error
            free(db_config_data);
            printf("Database configuration file is corrupted\n");
            write_log("Database configuration file is corrupted");
            return;
        }
        free(db_config_data);
    }

    // Connect to the database
    db_conn = db_connect(&connection_info);
    if (db_conn == NULL) {
        printf("Database connection failed\n");
        write_log("Database connection failed");
        return;
    }
}

void initialize_server() {
    // Read server configuration file
    char* port_config_data = get_decrypted_config("server.config");
    int port;
    if (port_config_data) {
        if(sscanf(port_config_data, "%d", &port)) {
            // Handle parsing error
            free(port_config_data);
            printf("Server configuration file is corrupted\n");
            write_log("Server configuration file is corrupted");
            return;
        }
        free(port_config_data);
    }

    // Create a server socket and bind it to the specified port
    server = tcp_server_create(port);
    if (server == NULL) {
        printf("Server creation failed\n");
        write_log("Server creation failed");
        db_disconnect(db_conn);
        return;
    }
}
