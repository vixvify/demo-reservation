#include <iostream>
#include <cstring>

#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct Message {
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

        cout << "Client "
             << msg.client_id
             << " sent: "
             << msg.text
             << endl;

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