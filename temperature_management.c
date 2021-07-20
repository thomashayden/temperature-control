#include "temperature_management.h"

int desired_ac_state = 0;
char* filename;

float get_current_temperature() {
    FILE* fp;
    fp = fopen(SENSOR_FILE, "r");
    int temp;
    fscanf(fp, "t=%d", &temp);
    float temp_c = temp / 1000.0;
    float temp_f = (temp_c * 1.8) + 32.0;
    return temp_f;
}

uint32_t get_current_time_sec() {
    return (unsigned)time(NULL);
}

void initialize_file_name() {
    filename = malloc(32);
    time_t timeIn;
    time(&timeIn);
    struct tm* dateTime;
    dateTime = localtime(&timeIn);
    sprintf(filename, "%d%d%d", 1900 + dateTime->tm_year, 1 + dateTime->tm_mon, dateTime->tm_mday);
}

int get_desired_ac_state() {
    uint8_t target_temp = get_current_temperature_target();
    float actual_temp = get_current_temperature();
    if (target_temp + MAXIMUM_ALLOWABLE_TEMPERATURE_DELTA <= actual_temp) {
        desired_ac_state = 1;
    } else if (actual_temp <= target_temp - MAXIMUM_ALLOWABLE_TEMPERATURE_DELTA) {
        desired_ac_state = 0;
    }
    return desired_ac_state;
}

void log_temperature_and_ac_state() {
    FILE* fp;
    fp = fopen(filename, "a");
    fprintf(fp, "%d,%.1f,%d,%d\n", get_current_time_sec(), get_current_temperature(), get_current_temperature_target(), desired_ac_state);
    fclose(fp);
}