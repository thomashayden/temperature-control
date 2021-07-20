#ifndef TEMPERATURE_CONTROL_TEMPERATURE_SCHEDULE_H
#define TEMPERATURE_CONTROL_TEMPERATURE_SCHEDULE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define AC_OFF 0
#define HOURS_PER_DAY 24

/**
 * Get the target temperature at the current time. A value of AC_OFF means AC is turned off.
 *
 * @return target temperature in fahrenheit
 */
float get_current_temperature_target();

/**
 * Get the array of temperature targets across a day. The format is one target for every hour (0:00 - 23:00) in fahrenheit. A value of AC_OFF means AC is turned off.
 * Create new space in memory, and does not manage pointer.
 *
 * @return the temperature targets for each hour in a day
 */
uint8_t* get_temperature_targets();

/**
 * Set the array of temperature targets across a day. The format is one target for every hour (0:00 - 23:00) in fahrenheit. A value of AC_OFF means AC is turned off.
 * Does not take control of point.
 */
void set_temperature_targets(const uint8_t*);

#endif //TEMPERATURE_CONTROL_TEMPERATURE_SCHEDULE_H
