cmake_minimum_required(VERSION 3.16)
project(temperature_control C)

set(CMAKE_C_STANDARD 99)

add_executable(temperature_control main.c temperature_schedule.c rxtx.c temperature_management.c)

find_library(WIRINGPI_LIBRRIES wiringPi)
target_link_libraries(temperature_control ${WIRINGPI_LIBRARIES})

# gcc -o temperature_control main.c temperature_management.c rxtx.c temperature_schedule.c -lwiringPi