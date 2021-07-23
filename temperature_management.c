#include "temperature_management.h"

int desired_ac_state = 0;
char* filename;
uint32_t last_ac_state_change_time = 0;

void init_update_file_name() {
    filename = malloc(32);
    time_t timeIn;
    time(&timeIn);
    struct tm* dateTime;
    dateTime = localtime(&timeIn);
    sprintf(filename, "logs/%d%02d%02d", 1900 + dateTime->tm_year, 1 + dateTime->tm_mon, dateTime->tm_mday);
}

void log_message(char* message) {
    init_update_file_name();
    FILE* fp;
    fp = fopen(filename, "a");
    fprintf(fp, "%s\n", message);
    fclose(fp);
}

float get_current_temperature() {
    // 7c 01 7f 80 7f ff 04 10 86 : crc=86 YES
    // 7c 01 7f 80 7f ff 04 10 86 t=23750
    FILE* fp;
    fp = fopen(SENSOR_FILE, "r");
    if (fp == NULL) {
        size_t needed = snprintf(NULL, 0, "  Error opening file: %s\n", strerror(errno)) + 1;
        char* buffer = malloc(needed);
        sprintf(buffer, "  Error opening file: %s\n", strerror(errno));
        log_message(buffer);
        free(buffer);
        perror("");
        return -1;
    }

    int temp_raw;
    char data[256];
    char* dataP;
    fread(data, sizeof(char), 256, fp);
    fclose(fp);
    if (strstr(data, "YES") != NULL) {
        dataP = strstr(data, "t=");
        if (dataP != NULL) {
            temp_raw = atoi(dataP+2);
            float temp_c = temp_raw / 1000.0;
            float temp_f = (temp_c * 1.8) + 32.0;
            if (temp_f > 150 || temp_f < 0) {
                log_message("Temperature out of range");
                return -2;
            }
            return temp_f;
        }
    }
    log_message("CRC Check failed");
    return -3;
}

uint32_t get_current_time_sec() {
    return (unsigned)time(NULL);
}

int get_desired_ac_state() {
    float target_temp = get_current_temperature_target();
    float actual_temp = get_current_temperature();
    if (get_current_temperature() < 0) {
        return desired_ac_state;
    }
    if (target_temp + MAXIMUM_ALLOWABLE_TEMPERATURE_DELTA <= actual_temp) {
        desired_ac_state = 1;
    } else if (actual_temp <= target_temp - MAXIMUM_ALLOWABLE_TEMPERATURE_DELTA) {
        desired_ac_state = 0;
    }
    return desired_ac_state;
}

void log_temperature_and_ac_state() {
    init_update_file_name();
    FILE* fp;
    fp = fopen(filename, "a");
    fprintf(fp, "%d,%.1f,%.1f,%d\n", get_current_time_sec(), get_current_temperature(), get_current_temperature_target(), desired_ac_state);
    fclose(fp);
}

void update() {
    int last_desired = desired_ac_state;
    int ac_state_desired = get_desired_ac_state();
    if (last_desired != ac_state_desired) {
        if (get_current_time_sec() - last_ac_state_change_time >= MINIMUM_AC_OFF_TIME * 60) {
            size_t needed = snprintf(NULL, 0, "Setting AC to state %d\n", ac_state_desired) + 1;
            char* buffer = malloc(needed);
            sprintf(buffer, "Setting AC to state %d\n", ac_state_desired);
            log_message(buffer);
            free(buffer);
            write_remote(ac_state_desired, 1);
            last_ac_state_change_time = get_current_time_sec();
        } else {
            desired_ac_state = last_desired;
            size_t needed = snprintf(NULL, 0, "Desire to change AC state to %d, but minimum state change timing is blocking.",
                                     ac_state_desired) + 1;
            char* buffer = malloc(needed);
            sprintf(buffer, "Desire to change AC state to %d, but minimum state change timing is blocking.",
                    ac_state_desired);
            log_message(buffer);
            free(buffer);
        }
    }
    log_temperature_and_ac_state();
}