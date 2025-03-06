# Time sync tool
This project is a C++ application that reads time data from a Novatel OEM719 device through a UART interface and sets the system time.

This is intended to be used in BeagleBone Black.

## Dependencies
- **arm-linux-gnueabihf-g++** compiler for cross compilation
- **g++** compiler for unit tests & local compilation
- **clang-tidy** for static code analysis
- **cppcheck** also for static code analysis
- **libgtest-dev** for unit tests


## How to build
Build, static analysis and unit tests are included in Github Actions. They may be invoked in the command line as well:
- `make` to build the binaries in the target
- `make cross` to cross compile the binary in other system
- `make static-analysis` to run static analysis using cppcheck and clang-tidy

## Further improvements
- Test using actual hardware and debug potential bugs
- Utilize interrupts for PPS signal
