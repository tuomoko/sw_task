/*
 * Copyright (c) 2025 Tuomo Kohtamäki
 * 
 * This file is part of the Novatel OEM719 time synchronization library.
 */

#include "novatel.h"
#include "uart.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <bits/algorithmfwd.h>
#include <cmath>


Novatel::Novatel(UART uart) : uart(uart) {}

bool Novatel::readTime(struct timespec *time) {
    std::string uart_line = uart.readData();
    if (uart_line.empty()) {
        std::cerr << "Empty UART line." << std::endl;
        return false;
    }

    // Split the header and data part
    std::stringstream ss(uart_line);
    std::string header_string, data_string;
    getline( ss, header_string, ';' );
    getline( ss, data_string, ';' );

    if (header_string.empty() || data_string.empty()) {
        std::cerr << "Empty header or data string." << std::endl;
        return false;
    }

    // Split the header and data into individual fields
    std::stringstream ss_header(header_string);
    std::vector<std::string> header;
    while( ss_header.good() )
    {
        std::string substr;
        getline( ss_header, substr, ',' );
        header.push_back( substr );
    }
    std::stringstream ss_data(data_string);
    std::vector<std::string> data;
    while( ss_data.good() )
    {
        std::string substr;
        getline( ss_data, substr, ',' );
        data.push_back( substr );
    }

    // Some basic sanity checks. We check it's indeed the time message, and that it's valid
    if (header.size() < 2) {
        std::cerr << "Invalid header size." << std::endl;
        return false;
    }
    if (header[0] != "#TIMEA") {
        std::cerr << "Wrong packet type." << std::endl;
        return false;
    }
    if (data.size() < 11) {
        std::cerr << "Invalid data size " << data.size() << std::endl;
        return false;
    }
    if (data[0] != "VALID") {
        std::cerr << "Data is not VALID." << std::endl;
        return false;
    }

    // Extract the week number from the message
    int week = std::stoi(header[5]);
    // Extract the seconds from the message
    float seconds = std::stof(header[6]);
    // Extract the offset from the message
    float offset = std::stof(data[1]);

    // Calculate the seconds for the *next* PPS pulse
    seconds = std::floor(seconds) - offset + 1;

    // Calculate the GPS time in seconds since the epoch
    time_t gps_time = week * 7 * 24 * 60 * 60 + seconds;

    time->tv_sec = gps_time;
    return true;
}
#include <iostream>
