CC = gcc
CFLAGS  = -g -Wall

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
	$(CC) $(CFLAGS) -o ./$(OUTPUT_DIR)/$(TARGET) $(TARGET).c

crude: # nie wiem jak tam podlinkować matha (dodać -lm) i mam to w dupie więc dodałem takie coś
	gcc main.c -o main -lm
	./main