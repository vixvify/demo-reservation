#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

void setSynchronization(bool enabled);

std::string listSeats();

std::string getSeatStatus(
    int seatId
);

std::string reserveSeat(
    int workerId,
    int clientId,
    int seatId
);

std::string cancelSeat(
    int workerId,
    int clientId,
    int seatId
);

#endif