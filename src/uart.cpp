/*
 * Copyright (c) 2025 Tuomo Kohtamäki
 * 
 * This is a simple UART wrapper for reading and writing data to a UART port.
 */

#include "uart.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <cstddef> 

UART::UART(const char* portName, int baudRate) : portName(portName), baudRate(baudRate), uart_fd(-1) {}

UART::~UART() {
    close();
}

bool UART::open() {
    this->uart_fd = ::open(this->portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->uart_fd == -1) {
        std::cerr << "Error opening UART port: " << this->portName << std::endl;
        return false;
    }
    return true;
}

bool UART::init() {
    if (this->uart_fd == -1) {
        return false;
    }
    struct termios options;
    tcgetattr(this->uart_fd, &options);
    cfsetispeed(&options, this->baudRate);
    cfsetospeed(&options, this->baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;   // Clear the current data size setting
    options.c_cflag |= CS8;      // 8 data bits
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    options.c_oflag &= ~OPOST;   // Raw output

    tcsetattr(this->uart_fd, TCSANOW, &options);
    return true;
}

std::string UART::readData() {
    char buffer[UART_BUFFER_SIZE];
    std::memset(buffer, 0, UART_BUFFER_SIZE);
    int bytes_read = ::read(this->uart_fd, buffer, UART_BUFFER_SIZE);
    if (bytes_read > 0) {
        return std::string(buffer, bytes_read);
    } else {
        return "";
    }
}

void UART::close() {
    if (this->uart_fd != -1) {
        ::close(this->uart_fd);
        this->uart_fd = -1;
    }
}

bool UART::sendData(const std::string& data) {
    if (uart_fd == -1) {
        std::cerr << "UART port not open." << std::endl;
        return false;
    }

    int bytes_written = ::write(uart_fd, data.c_str(), data.size());
    return bytes_written == static_cast<int>(data.size());
}
