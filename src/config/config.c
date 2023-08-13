// config.c

#include "config.h"

int get_server_port_config() {
    int port;

    // Read encrypted file contents
    BYTE *encryptedData;
    DWORD encryptedSize;
    ReadEncryptedFile("port.config", &encryptedData, &encryptedSize);

    // Decrypt file contents
    BYTE *decryptedData;
    DWORD decryptedSize;
    DecryptData(encryptedData, encryptedSize, &decryptedData, &decryptedSize);

    // Parse file contents
    sscanf((char *)decryptedData, "%d", port);

    // Free memory
    free(encryptedData);
    LocalFree(decryptedData);
}

db_connection_info_t get_db_connection_config() {
    db_connection_info_t connection_info;

    // Read encrypted file contents
    BYTE *encryptedData;
    DWORD encryptedSize;
    if (!ReadEncryptedFile("repository.config", &encryptedData, &encryptedSize)) {
        // Handle error
    }

    // Decrypt file contents
    BYTE *decryptedData;
    DWORD decryptedSize;
    if (!DecryptData(encryptedData, encryptedSize, &decryptedData, &decryptedSize)) {
        // Handle error
    }

    // Parse file contents
    if (sscanf((char *)decryptedData, "%s %d %s %s %s", connection_info.host, &connection_info.port,
           connection_info.user, connection_info.password, connection_info.database) != 5) {
        // Handle parsing error
    }

    // Free memory
    free(encryptedData);
    free(decryptedData); // Assuming decryptedData is allocated using a compatible method

    return connection_info;
}

