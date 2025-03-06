#include "novatel.h"
#include "uart.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>


Novatel::Novatel(UART uart) : uart(uart) {}

std::string Novatel::readTime() {
    const int buffer_size = 256;
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);

    int bytes_read = uart.readData(buffer, buffer_size - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        return std::string(buffer);
    } else {
        return "";
    }
}
#include <iostream>
