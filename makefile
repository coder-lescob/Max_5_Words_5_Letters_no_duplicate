CC    := gcc

SRC   := src
BUILD := build

.PHONY: all
all: clean build run

.PHONY: clean
clean:
	@rm $(BUILD)/main

.PHONY: build
build:
	@$(CC) $(SRC)/main.c -g -o $(BUILD)/main

.PHONY: run
run:
	@./$(BUILD)/main