// message.c

#include "message.h"

Message* parse_message(const char *buffer, int length) {
    Message *message = (Message*)malloc(sizeof(Message));
    // Parse the buffer and populate the message structure
    strncpy(message->content, buffer, length);
    return message;
}