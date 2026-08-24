#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype;
    int client_id;
    char text[100];
};

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <client_id>\n", argv[0]);
        return 1;
    }

    int client_id = atoi(argv[1]);

    key_t key = ftok("/tmp", 'A');

    int msgid = msgget(key, 0666);

    struct message msg;

    msg.mtype = 1;
    msg.client_id = client_id;

    printf("Enter message: ");

    fgets(
        msg.text,
        sizeof(msg.text),
        stdin
    );

    msgsnd(
        msgid,
        &msg,
        sizeof(msg) - sizeof(long),
        0
    );

    long response_type = 100 + client_id;

    msgrcv(
        msgid,
        &msg,
        sizeof(msg) - sizeof(long),
        response_type,
        0
    );

    printf(
        "Server: %s\n",
        msg.text
    );

    return 0;
}