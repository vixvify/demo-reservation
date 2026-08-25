#include <iostream>
#include <cstring>

#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_RESOURCES 20

using namespace std;

struct Message {
    long mtype;
    int client_id;
    int resource_id;
    char command[20];
    char text[100];
};

int main() {
    key_t key = ftok("/tmp", 'A');

    int msgid = msgget(
        key,
        IPC_CREAT | 0666
    );

    int reservation[MAX_RESOURCES] = {0};

    cout << "Server started..." << endl;

    while (true) {
        Message msg;

        msgrcv(
            msgid,
            &msg,
            sizeof(msg) - sizeof(long),
            1,
            0
        );

        cout
            << "Client " << msg.client_id
            << ": "
            << msg.command
            << " "
            << msg.resource_id
            << "\n";

        if (strcmp(msg.command, "STATUS") == 0) {

            int index = msg.resource_id - 1;

            if (reservation[index] == 0) {
                strcpy(
                    msg.text,
                    "AVAILABLE"
                );
            } else {
                snprintf(
                    msg.text,
                    sizeof(msg.text),
                    "RESERVED by Client %d",
                    reservation[index]
                );
            }
        }

        else if (strcmp(msg.command, "RESERVE") == 0) {

            int index = msg.resource_id - 1;

            if (reservation[index] == 0) {

                reservation[index] = msg.client_id;

                strcpy(
                    msg.text,
                    "RESERVE SUCCESS"
                );

            } else {

                snprintf(
                    msg.text,
                    sizeof(msg.text),
                    "FAILED: already reserved by Client %d",
                    reservation[index]
                );
            }
        }

        else if (strcmp(msg.command, "CANCEL") == 0) {

            int index = msg.resource_id - 1;

            if (reservation[index] == msg.client_id) {

                reservation[index] = 0;

                strcpy(
                    msg.text,
                    "CANCEL SUCCESS"
                );

            } else {

                strcpy(
                    msg.text,
                    "CANCEL FAILED"
                );
            }
        }

        else {

            strcpy(
                msg.text,
                "Unknown command"
            );
        }

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