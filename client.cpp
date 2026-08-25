#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>

#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct Message {
    long mtype;
    int client_id;
    char text[100];
};

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <client_id>\n";
        return 1;
    }

    int client_id = atoi(argv[1]);

    key_t key = ftok("/tmp", 'A');

    int msgid = msgget(key, 0666);

    Message msg;

    msg.mtype = 1;
    msg.client_id = client_id;

    cout << "Enter message: ";

    string input;
    getline(cin, input);

    strncpy(
        msg.text,
        input.c_str(),
        sizeof(msg.text) - 1
    );

    msg.text[sizeof(msg.text) - 1] = '\0';

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

    cout << "Server: " << msg.text << '\n';

    return 0;
}