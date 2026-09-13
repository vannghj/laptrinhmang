CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra
TARGET = exercise2
OBJS = main.o account.o logger.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c account.h logger.h
	$(CC) $(CFLAGS) -c main.c

account.o: account.c account.h
	$(CC) $(CFLAGS) -c account.c

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
