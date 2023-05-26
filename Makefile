CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread
LDFLAGS = -lpaho-mqtt3c -pthread

TARGET = mqtt_client
SRCDIR = src
INCDIR = include

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
