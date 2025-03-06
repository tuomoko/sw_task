/*
 * Copyright (c) 2025 Tuomo Kohtamäki
 * 
 * This program initializes a UART interface and a GPIO pin for PPS signal detection.
 * It configures a Novatel GPS receiver to output time messages and PPS signals.
 * The program waits for a valid TIME message and a PPS signal, then sets the system time accordingly.
 */

#include <iostream>
#include "uart.h"
#include "novatel.h"
#include "gpio.h"
#include <unistd.h>
#include <fcntl.h>

int main() {
    UART uart("/dev/ttyS1", 9600); //UART1 on pins P9.24, P9.26
    Novatel novatel(uart);

    // Initialize UART interface
    if (!uart.init() || !uart.open()) {
        std::cerr << "Failed to initialize UART interface." << std::endl;
        return 1;
    }

    // Initialize GPIO49 (P9.23) as PPS input
    unsigned int pps_pin = 27;
    gpio_export(pps_pin);
    gpio_set_dir(pps_pin, INPUT_PIN);
    
    // Set the operating mode of the receiver
    uart.sendData("LOG TIMEA ONTIME 1\r\n");
    uart.sendData("PPSCONTROL ENABLE POSITIVE\r\n");

    // Wait for the first valid TIME message (max 3 seconds)
    struct timespec current_time = {0};
    bool time_success = false;

    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = 20 * 1000000L; // 20 milliseconds

    for (int i = 0; i < 150; ++i) {
        time_success = novatel.readTime(&current_time);
        if (time_success) {
            std::cout << "Received TIME message: " << current_time.tv_sec << std::endl;
            break;
        }
        nanosleep(&req, (struct timespec *)NULL);
    }
    if (!time_success) {
        std::cerr << "Failed to receive TIME message." << std::endl;
        return 1;
    }

    // Wait for the next PPS signal (max 1 second)
    unsigned int pps_state=0;

    for (int i = 0; i < 50; ++i) {
        gpio_get_value(pps_pin, &pps_state);
        if (pps_state == 1) {
            std::cout << "PPS signal detected." << std::endl;
            break;
        }
        nanosleep(&req, (struct timespec *)NULL);
    }
    if (pps_state == 0) {
        std::cerr << "Failed to detect PPS signal." << std::endl;
        return 1;
    }

    // Set the time using clock_settime() system call
    if (clock_settime(CLOCK_REALTIME, &current_time) == -1) {
        std::cerr << "Failed to set system time." << std::endl;
        return 1;
    } else {
        std::cout << "System time set successfully." << std::endl;
    }

    return 0;
}