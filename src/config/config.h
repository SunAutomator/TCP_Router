#ifndef _CONFIG_H
#define _CONFIG_H
#pragma once

#include "../util/crypto.h"
#include "../database/database.h"

int get_server_port_config();

db_connection_info_t get_db_connection_config();

#endif // _CONFIG_H