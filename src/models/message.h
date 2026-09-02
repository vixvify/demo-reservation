#ifndef MESSAGE_H
#define MESSAGE_H

struct Message {
    long mtype;
    int clientId;
    char command[128];
    char response[2048];
};

#endif