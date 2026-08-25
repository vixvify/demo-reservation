#include <iostream>
#include <cstdlib>
#include <cstring>

#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct Message {
    long mtype;
    int client_id;
    int resource_id;
    char command[20];
    char text[100];
};

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout
            << "Usage: "
            << argv[0]
            << " <client_id>\n";

        return 1;
    }

    int client_id = atoi(argv[1]);

    key_t key = ftok("/tmp", 'A');

    int msgid = msgget(key, 0666);

    while (true) {

        Message msg;

        msg.mtype = 1;
        msg.client_id = client_id;

        cout << "Client-" << client_id << "> ";

        cin >> msg.command;

        if (
            strcmp(msg.command, "STATUS") == 0 ||
            strcmp(msg.command, "RESERVE") == 0 ||
            strcmp(msg.command, "CANCEL") == 0
        ) {
            cin >> msg.resource_id;
        } else {
            msg.resource_id = 0;
        }

        msgsnd(
            msgid,
            &msg,
            sizeof(msg) - sizeof(long),
            0
        );

        msgrcv(
            msgid,
            &msg,
            sizeof(msg) - sizeof(long),
            100 + client_id,
            0
        );

        cout
            << "Server: "
            << msg.text
            << "\n";
    }

    return 0;
}