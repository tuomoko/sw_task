#include "uart.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

UART::UART(const char* portName, int baudRate) : portName(portName), baudRate(baudRate), uart_fd(-1) {}

bool UART::open()
{
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

int UART::readData(char* buffer, size_t size) {
    return read(this->uart_fd, buffer, size);
}

void UART::close() {
    if (this->uart_fd != -1) {
        ::close(this->uart_fd);
        this->uart_fd = -1;
    }
}

UART::~UART() {
    this->close();
}
