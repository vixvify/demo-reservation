#include "worker.h"

#include "../models/message.h"
#include "../constants/constants.h"
#include "../reservation/reservation.h"
#include "../utils/logger.h"

#include <sys/msg.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

string processCommand(
    int workerId,
    int clientId,
    const string& command
) {

    stringstream ss(command);

    string action;

    ss >> action;

    if (action == "LIST") {
        return listSeats();
    }

    if (action == "STATUS") {

        int seatId;

        if (!(ss >> seatId)) {

            return
                "Usage: STATUS <seat_id>";
        }

        return getSeatStatus(seatId);
    }

    if (action == "RESERVE") {

        int seatId;

        if (!(ss >> seatId)) {

            return
                "Usage: RESERVE <seat_id>";
        }

        return reserveSeat(
            workerId,
            clientId,
            seatId
        );
    }

    if (action == "CANCEL") {

        int seatId;

        if (!(ss >> seatId)) {

            return
                "Usage: CANCEL <seat_id>";
        }

        return cancelSeat(
            workerId,
            clientId,
            seatId
        );
    }

    return "ERROR: Unknown command";
}

