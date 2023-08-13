#ifndef _SERVICE_MANAGER_H_
#define _SERVICE_MANAGER_H_
#pragma once

#include "../tcp_server/tcp_server.h"
#include "../database/database.h"
#include "../config/config.h"
#include "../logger/logger.h"

#include <windows.h>

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceCtrlHandler(DWORD fdwControl);
int initialize_service(int argc, char *argv[]);
void initialize_database();
void initialize_server();

#endif // _SERVICE_MANAGER_H_