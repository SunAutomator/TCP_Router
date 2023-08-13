// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Message {
    char content[1024]; // Adjust size according to your needs
    // Additional fields if needed, such as type, length, etc.
} Message;

Message* parse_message(const char *buffer, int length);

#endif // MESSAGE_H