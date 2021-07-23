#include <stdio.h>
#include <unistd.h>

#include "temperature_management.h"
#include "temperature_schedule.h"
#include "rxtx.h"

#define UPDATE_FREQUENCY 1

int main() {
    init_update_file_name();
    setup();

    uint8_t new_targets[24] = { 73, 72, 72, 72, 72, 72, 72, 72, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73, 73 };
    set_temperature_targets(new_targets);

    write_remote(0, 1);
//    write_remote(0, 2);
//    write_remote(0, 3);
//    write_remote(0, 4);
//    write_remote(0, 5);

    while (1) {
        update();
        sleep(UPDATE_CYCLE_TIME * 60);
    }

//    find_remote_code();
//    write_on(1);

    return 0;
}
