/*
 * Copyright (c) 2025 Tuomo Kohtamäki
 * 
 * This file is part of the Novatel OEM719 time synchronization library.
 */

#ifndef NOVATEL_H
#define NOVATEL_H

#include "uart.h"
#include <string>

class Novatel {
public:
    explicit Novatel(UART uart);
    bool readTime(struct timespec* time);

private:
    UART uart;
};

#endif // NOVATEL_H