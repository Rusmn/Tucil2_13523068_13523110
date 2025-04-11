CC = g++
CFLAGS = -std=c++11 -Wall -Wextra
SRC_DIR = src
BIN_DIR = bin

$(shell mkdir -p $(BIN_DIR))

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
EXEC = $(BIN_DIR)/main

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

run: $(EXEC)
	./bin/main

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all run clean