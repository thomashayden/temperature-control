#include "rxtx.h"
#include "signal_data.c"

void setup() {
    // TODO: Setup pins that are needed
    wiringPiSetup();
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

void write_remote(const int on, const int id) {
    const int* signal;
    const int* signal_timing;
    int array_size;
    if (on == 1 && id == 1) {
        array_size = sizeof on_1_timing / sizeof on_1_timing[0];
        signal = on_1;
        signal_timing = on_1_timing;
	} else if (on == 0 && id == 1) {
        array_size = sizeof off_1_timing / sizeof off_1_timing[0];
        signal = off_1;
        signal_timing = off_1_timing;
    } else {
        fprintf(stderr, "Invalid write on? id combo: on %d, id %d. Ignoring call.\n", on, id);
    }

    int index = 0;
    long long int start = get_time_microsecond();
    while (index < array_size) {
        if (get_time_microsecond() - start >= signal_timing[index]) {
            digitalWrite(PIN_TRANSMIT, signal[index]);
            index = index + 1;
        }
    }
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

    write_tx(LOW);
    
    // Prompt user to press button once
    fprintf(stdout, "Beginning detection of button. Press button once now.\n");

    // Monitor Rx for a HIGH signal
    long long int start = get_time_microsecond();
    while (read_rx() == LOW && start - get_time_microsecond() < WAIT_FOR_BUTTON_TIMEOUT_MS * 1000);
    if (get_time_microsecond() - start > WAIT_FOR_BUTTON_TIMEOUT_MS * 1000) {
        fprintf(stderr, "Time out waiting for button press. No high signal detected on Rx.");
        return;
    }

    // TODO: Start recording the changes in signal along with their time delta
    // When no HIGH signal has been detected for LISTEN_TIMEOUT stop listening
    fprintf(stdout, "Signal start detected");
    start = get_time_microsecond();
    int last_signal = LOW;
    long long int time_delta = get_time_microsecond() - start;
    long long int time_last_signal = start;
    while (get_time_microsecond() - time_last_signal < LISTEN_TIMEOUT_MS * 1000) {
    	time_delta = get_time_microsecond() - start;
        if (read_rx() != last_signal) {
            last_signal = read_rx();
            time_last_signal = get_time_microsecond();
            fprintf(stdout, "%lld,%d\n", time_delta, last_signal);
        }
    }
    fprintf(stdout, "\n");

    // TODO: Trim everything before the first HIGH signal and the after the last HIGH->LOW transition

}
