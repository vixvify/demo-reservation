#include "logger.h"

#include <iostream>
#include <mutex>

using namespace std;

mutex logMutex;

int sequenceNumber = 0;

void logMessage(
    int workerId,
    int clientId,
    const string& message
) {

    lock_guard<mutex> lock(logMutex);

    sequenceNumber++;

    cout
        << "[SEQ " << sequenceNumber << "] "
        << "[Worker-" << workerId << "] "
        << "[Client-" << clientId << "] "
        << message
        << endl;
}