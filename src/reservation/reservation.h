#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <vector>

void setSynchronization(bool enabled);

std::string listSeats();

std::string getSeatStatus(
    int seatId
);

std::string reserveSeats(
    int workerId,
    int clientId,
    const std::vector<int>& requestedSeats
);

std::string cancelSeat(
    int workerId,
    int clientId,
    int seatId
);

#endif