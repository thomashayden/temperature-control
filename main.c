#include <stdio.h>
#include <unistd.h>

#include "temperature_management.h"
#include "temperature_schedule.h"
#include "rxtx.h"

#define UPDATE_FREQUENCY 1

int main() {
    initialize_file_name();
    setup();

    uint8_t new_targets[24] = { 68, 70, 70, 70, 70, 70, 70, 70, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68 };
    set_temperature_targets(new_targets);

    while (1) {
        update();
        sleep(UPDATE_CYCLE_TIME * 60);
    }

//    find_remote_code();
//    write_on(1);

    return 0;
}
