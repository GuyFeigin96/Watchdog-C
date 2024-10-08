# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinc -g -fPIC
LDFLAGS = -Wl,-rpath=/home/guyfeigin/Documents/myGit/Watchdog/bin/debug -L$(DEBUG_DIR) -ldlist -lpqueue -lscheduler -lsrtlist -ltask -luid -lwatchdog_client -lpthread -lrt

# Directories
SRC_DIR = src
INC_DIR = inc
TEST_DIR = test
BIN_DIR = bin
DEBUG_DIR = $(BIN_DIR)/debug
RELEASE_DIR = $(BIN_DIR)/release

# Executables
WATCHDOG_EXEC = $(DEBUG_DIR)/watchdog
CLIENT_TEST_EXEC = $(DEBUG_DIR)/watchdog_client_test

# Shared object files
SO_FILES = $(DEBUG_DIR)/libdlist.so $(DEBUG_DIR)/libpqueue.so \
           $(DEBUG_DIR)/libscheduler.so $(DEBUG_DIR)/libsrtlist.so \
           $(DEBUG_DIR)/libtask.so $(DEBUG_DIR)/libuid.so \
           $(DEBUG_DIR)/libwatchdog_client.so

# Source files for shared libraries
SRC_FILES = $(SRC_DIR)/dlist.c $(SRC_DIR)/pqueue.c $(SRC_DIR)/scheduler.c \
            $(SRC_DIR)/srtlist.c $(SRC_DIR)/task.c $(SRC_DIR)/uid.c \
            $(SRC_DIR)/watchdog_client.c

# Build targets
all: $(SO_FILES) $(WATCHDOG_EXEC) $(CLIENT_TEST_EXEC)

# Build shared libraries
$(DEBUG_DIR)/lib%.so: $(SRC_DIR)/%.c
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $<

# Build watchdog executable
$(WATCHDOG_EXEC): $(SRC_FILES) $(SO_FILES)
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/watchdog.c $(LDFLAGS)

# Build watchdog client test executable
$(CLIENT_TEST_EXEC): $(SRC_FILES) $(SO_FILES)
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_DIR)/watchdog_client_test.c $(LDFLAGS)

# Specific rule for building the watchdog_client shared library
$(DEBUG_DIR)/libwatchdog_client.so: $(SRC_DIR)/watchdog_client.c $(SRC_DIR)/pqueue.c $(SRC_DIR)/task.c $(SRC_DIR)/uid.c $(SRC_DIR)/srtlist.c $(SRC_DIR)/scheduler.c $(SRC_DIR)/dlist.c
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $^

# Clean up build artifacts, but keep the debug directory
clean:
	rm -f $(DEBUG_DIR)/*.so $(WATCHDOG_EXEC) $(CLIENT_TEST_EXEC)

.PHONY: all clean
