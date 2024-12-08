CC = gcc
CFLAGS = -Wall -Wextra -O2 -pedantic -std=c99
LIBS = -lm -lSDL2 -lSDL2_image
TARGET = train
SRCS = train.c
OBJS = train.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(OBJS) $(TARGET)
