#include "temperature_schedule.h"

uint8_t targets[HOURS_PER_DAY] = { AC_OFF };

float get_current_temperature_target() {
    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);
    int hour = now_tm->tm_hour;
    int next_hour = (hour + 1) % 24;
    int minutes = now_tm->tm_min;

    if (targets[hour] == AC_OFF) {
        return AC_OFF;
    } else if (targets[next_hour] == AC_OFF) {
        return targets[hour];
    } else {
        return targets[hour] +  (targets[next_hour] - targets[hour]) * (minutes / 60.0);
    }
}

uint8_t* get_temperature_targets() {
    uint8_t* target_copy = malloc(sizeof(uint8_t) * HOURS_PER_DAY);
    memcpy(target_copy, targets, sizeof(uint8_t) * HOURS_PER_DAY);
    return target_copy;
}

void set_temperature_targets(const uint8_t* new_targets) {
    memcpy(targets, new_targets, sizeof(uint8_t) * HOURS_PER_DAY);
}