#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype;
    int client_id;
    char text[100];
};

int main() {
    key_t key = ftok("/tmp", 'A');

    int msgid = msgget(
        key,
        IPC_CREAT | 0666
    );

    printf("Server started...\n");

    while (1) {
        struct message msg;

        msgrcv(
            msgid,
            &msg,
            sizeof(msg) - sizeof(long),
            1,
            0
        );

        printf(
            "Client %d sent: %s\n",
            msg.client_id,
            msg.text
        );

        msg.mtype = 100 + msg.client_id;

        strcpy(
            msg.text,
            "Server received your message"
        );

        msgsnd(
            msgid,
            &msg,
            sizeof(msg) - sizeof(long),
            0
        );
    }

    return 0;
}