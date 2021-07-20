#include <stdio.h>
#include <unistd.h>

#include "temperature_management.h"
#include "temperature_schedule.h"
#include "rxtx.h"

#define UPDATE_FREQUENCY 1

int main() {
    initialize_file_name();
    setup();

    uint8_t new_targets[24] = { 68, 72, 72, 72, 72, 72, 72, 72, 68, 68, 68, 68, 68, 68, 68, 68, 68, 72, 68, 68, 68, 68, 68, 68 };
    set_temperature_targets(new_targets);

//    while (1) {
//        int ac_state_desired = get_desired_ac_state();
//        fprintf(stdout, "Setting AC to state %d\n", ac_state_desired);
//        // TODO: Actually update the AC
//        log_temperature_and_ac_state();
//        sleep(10);
//    }



    uint8_t* targets = get_temperature_targets();
//    for (uint8_t i = 0; i < 24; i++) {
//        printf("%d ", targets[i]);
//    }
//    printf("\n");
//    free(targets);
//
//    uint8_t new_targets[24] = { 68, 72, 72, 72, 72, 72, 72, 72, 68, 68, 68, 68, 68, 68, 68, 68, 68, 72, 68, 68, 68, 68, 68, 68 };
//    set_temperature_targets(new_targets);
//
//    targets = get_temperature_targets();
//    for (uint8_t i = 0; i < 24; i++) {
//        printf("%d ", targets[i]);
//    }
//    printf("\n");
//    free(targets);
//
//    printf("%d\n", get_current_temperature_target());
//
//    printf("Starting remote code detection\n-----------------------------\n");

//    find_remote_code();
    write_on(1);

    return 0;
}
