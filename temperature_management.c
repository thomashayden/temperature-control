#include "temperature_management.h"

int desired_ac_state = 0;
char* filename;

float get_current_temperature() {
    // 7c 01 7f 80 7f ff 04 10 86 : crc=86 YES
    // 7c 01 7f 80 7f ff 04 10 86 t=23750
    FILE* fp;
    fp = fopen(SENSOR_FILE, "r");
    if (fp == NULL) {
        return -1;
    }
    int crc, temp;
    char hex1[2], hex2[2], hex3[2], hex4[2], hex5[2], hex6[2], hex7[2], hex8[2], hex9[2], crc_good[10];
    char hex10[2], hex11[2], hex12[2], hex13[2], hex14[2], hex15[2], hex16[2], hex17[2], hex18[2];
    fscanf(fp, "%s %s %s %s %s %s %s %s %s : crc=%d %s\n"
               "%s %s %s %s %s %s %s %s %s t=%d",
               &hex1, &hex2, &hex3, &hex4, &hex5, &hex6, &hex7, &hex8, &hex9, &crc, &crc_good,
               &hex10, &hex11, &hex12, &hex13, &hex14, &hex15, &hex16, &hex17, &hex18, &temp);
    float temp_c = temp / 1000.0;
    float temp_f = (temp_c * 1.8) + 32.0;
    if (temp_f > 150 || temp_f < 0) {
        return -1;
    }
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