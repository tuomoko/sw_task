#include <iostream>
#include "uart.h"
#include "novatel.h"

int main() {
    UART uart("/dev/tty0", 9600);
    Novatel novatel(uart);

    // Initialize UART interface
    if (!uart.init() || !uart.open()) {
        std::cerr << "Failed to initialize UART interface." << std::endl;
        return 1;
    }

    while (true) {
        // Read data from Novatel OEM719
        std::string data = novatel.readTime();
        if (!data.empty()) {
            // Parse the time data
            //std::string time = novatel.parseTime(data);
            std::cout << "Current Time: " << time << std::endl;
        }
    }

    // Close UART connection (this line will never be reached in the current loop)
    uart.close();
    return 0;
}