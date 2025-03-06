#ifndef NOVATEL_H
#define NOVATEL_H

#include "uart.h"
#include <string>

class Novatel {
public:
    Novatel(UART uart);
    std::string readTime();

private:
    UART uart;
};

#endif // NOVATEL_H