#ifndef UART_H
#define UART_H

#include <cstdint>
#include <cstddef> 

class UART {
public:
    UART(const char* portName, int baud_rate);
    ~UART();

    bool init();
    bool open();
    int readData(char* buffer, size_t size);
    void close();

private:
    int uart_fd; // File descriptor for the UART
    int baudRate; // Baud rate for the UART
    const char* portName; // Name of the UART port
};

#endif // UART_H