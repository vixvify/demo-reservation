#ifndef LOGGER_H
#define LOGGER_H

#include <string>

void logMessage(
    int workerId,
    int clientId,
    const std::string& message
);

#endif