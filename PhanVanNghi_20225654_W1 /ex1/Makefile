CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra
TARGET = exercise1
OBJS = main.o hazchem.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c hazchem.h
	$(CC) $(CFLAGS) -c main.c

hazchem.o: hazchem.c hazchem.h
	$(CC) $(CFLAGS) -c hazchem.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
