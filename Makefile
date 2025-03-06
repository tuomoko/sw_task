# Compilers
CXX=g++
CPPFLAGS= -I$(INCDIR) -D__BBB__
LDFLAGS=
LDLIBS=

CXX_CROSS=arm-linux-gnueabihf-g++
CPPFLAGS_CROSS= -I$(INCDIR) -D__BBB__
LDFLAGS_CROSS=
LDLIBS_CROSS=

RM=rm -f

# Directories
SRCDIR = src/
INCDIR = inc/
BUILDDIR = build/
BUILDDIR_CROSS = buildcross/

# Files
SRC = $(wildcard $(SRCDIR)*.cpp) 
OBJ = $(patsubst $(SRCDIR)%.cpp,$(BUILDDIR)%.o,$(SRC))
OBJ_CROSS = $(patsubst $(SRCDIR)%.cpp,$(BUILDDIR_CROSS)%.o,$(SRC))
ANALYSIS_SRC = src/uart.cpp src/novatel.cpp src/main.cpp

# Definitions for the static analysis
CPPCHECK = cppcheck --enable=all --inconclusive --error-exitcode=1
CLANG_TIDY = clang-tidy

# Default is local compile
all: setclock

setclock: $(OBJ)
	$(CXX) $(LDFLAGS) -o setclock $(OBJ) $(LDLIBS)

$(BUILDDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CPPFLAGS) -c $< -o $@

# Cross compile
cross: $(OBJ_CROSS)
	$(CXX_CROSS) $(LDFLAGS_CROSS) -o setclock $(OBJ_CROSS) $(LDLIBS_CROSS)

$(BUILDDIR_CROSS)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(BUILDDIR_CROSS)
	$(CXX_CROSS) $(CPPFLAGS_CROSS) -c $< -o $@


# Static Analysis
static-analysis:
	@echo "Running cppcheck..."
	$(CPPCHECK) $(ANALYSIS_SRC) --suppress=missingIncludeSystem --suppress=checkersReport --inline-suppr -I$(INCDIR)
	@echo "Running clang-tidy..."
	$(CLANG_TIDY) $(ANALYSIS_SRC) -- -I$(INCDIR)


clean:
	$(RM) $(OBJ)
	$(RM) $(OBJ_CROSS)

distclean: clean
	$(RM) setclock