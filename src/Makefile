CC = gcc
CFLAGS  = -g -Wall -lm

TARGET = main
OUTPUT_DIR = ./

default: all

all: $(TARGET)

clean:
	$(RM) ./$(OUTPUT_DIR)/$(TARGET)

run: all
	@echo -e 'Running $(TARGET) ...\n'
	@./$(OUTPUT_DIR)/$(TARGET)

$(TARGET): $(TARGET).c
	@echo -e 'Compiling $(TARGET).c ...'
	@$(CC) $(CFLAGS) -o ./$(OUTPUT_DIR)/$(TARGET) $(TARGET).c