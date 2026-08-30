#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {

    constexpr int SEAT_COUNT = 20;

    constexpr int WORKER_COUNT = 3;

    constexpr long REQUEST_TYPE = 1;
    constexpr long RESPONSE_TYPE_BASE = 1000;

    constexpr int MIN_DELAY_MS = 50;
    constexpr int MAX_DELAY_MS = 500;

    constexpr int AVAILABLE = 0;

    constexpr const char* QUEUE_PATH = "/tmp";
    constexpr int QUEUE_PROJECT_ID = 'A';

}

#endif