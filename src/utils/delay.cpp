#include "delay.h"

#include "../constants/constants.h"

#include <thread>
#include <chrono>
#include <random>

using namespace std;

void randomDelay() {

    thread_local random_device rd;

    thread_local mt19937 generator(rd());

    uniform_int_distribution<int> distribution(
        Constants::MIN_DELAY_MS,
        Constants::MAX_DELAY_MS
    );

    int delay = distribution(generator);

    this_thread::sleep_for(
        chrono::milliseconds(delay)
    );
}