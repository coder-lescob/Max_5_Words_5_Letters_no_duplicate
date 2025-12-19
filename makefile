CC    := gcc

SRC   := src
BUILD := build

.PHONY: clean
clean:
	@rm $(BUILD)/main

.PHONY: build
build:
	@$(CC) $(SRC)/main.c -o $(BUILD)/main

.PHONY: run
run:
	@./$(BUILD)/main