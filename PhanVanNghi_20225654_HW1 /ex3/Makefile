CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra
TARGET = exercise3
OBJS = main.o student.o scoreboard.o
LDLIBS = -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

main.o: main.c student.h scoreboard.h
	$(CC) $(CFLAGS) -c main.c

student.o: student.c student.h
	$(CC) $(CFLAGS) -c student.c

scoreboard.o: scoreboard.c scoreboard.h student.h
	$(CC) $(CFLAGS) -c scoreboard.c

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
