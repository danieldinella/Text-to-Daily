CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS =

SRCS = Main.c
OBJS = $(SRCS:.c=.o)
HEADER = Header.h
TARGET = Text-to-daily

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
