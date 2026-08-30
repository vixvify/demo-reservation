#ifndef MESSAGE_H
#define MESSAGE_H

struct Message {
    long mtype;

    int clientId;

    char command[64];

    char response[1024];
};

#endif