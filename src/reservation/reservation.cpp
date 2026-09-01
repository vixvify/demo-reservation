#include "reservation.h"

#include "../constants/constants.h"
#include "../utils/delay.h"
#include "../utils/logger.h"

#include <sstream>
#include <mutex>

using namespace std;

int seats[Constants::SEAT_COUNT] = {0};

mutex reservationMutex;

bool synchronizationEnabled = true;


void setSynchronization(bool enabled) {
    synchronizationEnabled = enabled;
}

bool isValidSeat(int seatId) {

    return seatId >= 1
        && seatId <= Constants::SEAT_COUNT;
}

string listSeats() {

    stringstream result;

    result << "\n===== Airplane Seat Map =====\n";

    for (
        int i = 0;
        i < Constants::SEAT_COUNT;
        i++
    ) {

        int seatNumber = i + 1;

        result
            << "Seat "
            << seatNumber
            << " : ";

        if (seats[i] == Constants::AVAILABLE) {

            result << "AVAILABLE";

        } else {

            result
                << "RESERVED by Client-"
                << seats[i];
        }

        result << "\n";
    }

    result << "=============================\n";

    return result.str();
}

string getSeatStatus(int seatId) {

    if (!isValidSeat(seatId)) {
        return "ERROR: Invalid seat number";
    }

    int index = seatId - 1;

    if (seats[index] == Constants::AVAILABLE) {

        return
            "Seat "
            + to_string(seatId)
            + " is AVAILABLE";
    }

    return
        "Seat "
        + to_string(seatId)
        + " is RESERVED by Client-"
        + to_string(seats[index]);
}

string reserveWithoutSync(
    int workerId,
    int clientId,
    int seatId
) {

    int index = seatId - 1;

    logMessage(
        workerId,
        clientId,
        "checking Seat "
        + to_string(seatId)
    );

    if (seats[index] == Constants::AVAILABLE) {

        logMessage(
            workerId,
            clientId,
            "Seat "
            + to_string(seatId)
            + " is AVAILABLE"
        );

        randomDelay();

        seats[index] = clientId;

        logMessage(
            workerId,
            clientId,
            "Seat "
            + to_string(seatId)
            + " reserved"
        );

        return
            "SUCCESS: Seat "
            + to_string(seatId)
            + " reserved";
    }

    return
        "FAILED: Seat "
        + to_string(seatId)
        + " is already reserved";
}

string reserveWithSync(
    int workerId,
    int clientId,
    int seatId
) {

    int index = seatId - 1;

    logMessage(
        workerId,
        clientId,
        "waiting for critical section"
    );

    lock_guard<mutex> lock(reservationMutex);

    logMessage(
        workerId,
        clientId,
        "entering critical section"
    );

    if (seats[index] == Constants::AVAILABLE) {

        logMessage(
            workerId,
            clientId,
            "Seat "
            + to_string(seatId)
            + " is AVAILABLE"
        );

        randomDelay();

        seats[index] = clientId;

        logMessage(
            workerId,
            clientId,
            "Seat "
            + to_string(seatId)
            + " reserved"
        );

        logMessage(
            workerId,
            clientId,
            "leaving critical section"
        );

        return
            "SUCCESS: Seat "
            + to_string(seatId)
            + " reserved";
    }

    logMessage(
        workerId,
        clientId,
        "Seat "
        + to_string(seatId)
        + " is already reserved"
    );

    logMessage(
        workerId,
        clientId,
        "leaving critical section"
    );

    return
        "FAILED: Seat "
        + to_string(seatId)
        + " is already reserved";
}

string reserveSeat(
    int workerId,
    int clientId,
    int seatId
) {

    if (!isValidSeat(seatId)) {
        return "ERROR: Invalid seat number";
    }

    if (synchronizationEnabled) {

        return reserveWithSync(
            workerId,
            clientId,
            seatId
        );
    }

    return reserveWithoutSync(
        workerId,
        clientId,
        seatId
    );
}

string cancelWithoutSync(
    int workerId,
    int clientId,
    int seatId
) {

    int index = seatId - 1;

    if (seats[index] == Constants::AVAILABLE) {

        return "FAILED: Seat is not reserved";
    }

    if (seats[index] != clientId) {

        return
            "FAILED: Seat belongs to another client";
    }

    randomDelay();

    seats[index] = Constants::AVAILABLE;

    logMessage(
        workerId,
        clientId,
        "cancelled Seat "
        + to_string(seatId)
    );

    return
        "SUCCESS: Seat "
        + to_string(seatId)
        + " cancelled";
}

string cancelWithSync(
    int workerId,
    int clientId,
    int seatId
) {

    lock_guard<mutex> lock(reservationMutex);

    int index = seatId - 1;

    if (seats[index] == Constants::AVAILABLE) {

        return "FAILED: Seat is not reserved";
    }

    if (seats[index] != clientId) {

        return
            "FAILED: Seat belongs to another client";
    }

    seats[index] = Constants::AVAILABLE;

    logMessage(
        workerId,
        clientId,
        "cancelled Seat "
        + to_string(seatId)
    );

    return
        "SUCCESS: Seat "
        + to_string(seatId)
        + " cancelled";
}

string cancelSeat(
    int workerId,
    int clientId,
    int seatId
) {

    if (!isValidSeat(seatId)) {
        return "ERROR: Invalid seat number";
    }

    if (synchronizationEnabled) {

        return cancelWithSync(
            workerId,
            clientId,
            seatId
        );
    }

    return cancelWithoutSync(
        workerId,
        clientId,
        seatId
    );
}