#include "rxtx.h"
#include "wiringPi.h"

void setup() {
    // TODO: Setup pins that are needed
    wiringPiSetupGpio();
    pinMode(PIN_RECEIVE, INPUT);
    pinMode(PIN_TRANSMIT, OUTPUT);
}

int read_rx() {
    return digitalRead(PIN_RECEIVE);
}

void write_tx(const int value) {
    digitalWrite(PIN_TRANSMIT, value);
}

// From https://programmingtoddler.wordpress.com/2014/11/09/c-how-to-get-system-timestamp-in-second-millisecond-and-microsecond/
long long int get_time_microsecond() {
    struct timeval timer_usec;
    long long int timestamp_usec;
    if (!gettimeofday(&timer_usec, NULL)) {
        timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll +
                         (long long int) timer_usec.tv_usec;
    }
    else {
        timestamp_usec = -1;
    }
    return timestamp_usec;
}

void find_remote_code() {
    // Procedure:
    //   Listen determine that there is no serious noise
    //   Prompt user to press button once
    //   Monitor Rx for a HIGH signal
    //   Start recording the changes in signal along with their time delta
    //   When no HIGH signal has been detected for LISTEN_TIMEOUT stop listening
    //   Trim everything before the first HIGH signal and the after the last HIGH->LOW transition
    // This should be the code. If it doesn't work, run this method again

    // Listen determine that there is no serious noise
//    fprintf(stdout, "Checking for noise. Please wait.\n");
//    long long int start = get_time_microsecond();
//    while (get_time_microsecond() - start < NOISE_CHECK_TIMEOUT_MS * 1000) {
//        if (read_rx() != LOW) {
//            fprintf(stderr, "Noise detected. Unable to detect button press.\n");
//            return;
//        }
//    }
//    fprintf(stdout, "No noise detected.\n");

    // Prompt user to press button once
    fprintf(stdout, "Beginning detection of button. Press button once now.\n");

    // Monitor Rx for a HIGH signal
    long long int start = get_time_microsecond();
    while (read_rx() == LOW && start - get_time_microsecond() < WAIT_FOR_BUTTON_TIMEOUT_MS * 1000);
    if (start - get_time_microsecond() < WAIT_FOR_BUTTON_TIMEOUT_MS * 1000) {
        fprintf(stderr, "Time out waiting for button press. No high signal detected on Rx.");
        return;
    }

    // TODO: Start recording the changes in signal along with their time delta
    // When no HIGH signal has been detected for LISTEN_TIMEOUT stop listening
    start = get_time_microsecond();
    int last_signal = LOW;
    long long int time_delta = get_time_microsecond() - start;
    long long int time_last_signal = start;
    while (get_time_microsecond() - time_last_signal < LISTEN_TIMEOUT_MS) {
        if (read_rx() != last_signal) {
            last_signal = read_rx();
            time_last_signal = get_time_microsecond();
            fprintf(stdout, "%lld,%d;", time_delta, last_signal);
        }
    }
    fprintf(stdout, "\n");

    // TODO: Trim everything before the first HIGH signal and the after the last HIGH->LOW transition

}