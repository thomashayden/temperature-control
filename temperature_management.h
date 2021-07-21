#ifndef TEMPERATURE_CONTROL_TEMPERATURE_MANAGEMENT_H
#define TEMPERATURE_CONTROL_TEMPERATURE_MANAGEMENT_H

#include "temperature_schedule.h"
#include "rxtx.h"

#include <bits/types/FILE.h>
#include <stdio.h>
#include <time.h>

#define MINIMUM_AC_OFF_TIME 5 // Minutes
#define UPDATE_CYCLE_TIME 1 // Minutes
#define MAXIMUM_ALLOWABLE_TEMPERATURE_DELTA 1.5 // Deg F
#define SENSOR_FILE "/sys/bus/w1/devices/28-000000031105/w1_slave"

void initialize_file_name();

int get_desired_ac_state();

void log_temperature_and_ac_state();

void update();

#endif //TEMPERATURE_CONTROL_TEMPERATURE_MANAGEMENT_H
