# Variables
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpthread
TARGET = randomizer
SRC = randomizer.c

# Default rule to build the program
all: $(TARGET)

# Linking rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Rule to clean the build artifacts
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all run clean
