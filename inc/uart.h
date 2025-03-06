/*
 * Copyright (c) 2025 Tuomo Kohtamäki
 * 
 * This is a simple UART wrapper for reading and writing data to a UART port.
 */

#ifndef UART_H
#define UART_H

#include <cstdint>
#include <cstddef> 
#include <string>

#define UART_BUFFER_SIZE 256

class UART {
public:
    UART(const char* portName, int baudRate);
    ~UART();

    bool init();
    bool open();
    std::string readData();
    void close();
    bool sendData(const std::string& data);

private:
    const char* portName; // Name of the UART port
    int baudRate; // Baud rate for the UART
    int uart_fd; // File descriptor for the UART
};

#endif // UART_H