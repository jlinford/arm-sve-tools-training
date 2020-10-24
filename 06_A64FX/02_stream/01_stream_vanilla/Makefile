NTIMES = 20

# Total memory: 0.2 GiB
STREAM_ARRAY_SIZE = 10000000

CC = gcc
CFLAGS = -DSTREAM_ARRAY_SIZE=$(STREAM_ARRAY_SIZE)UL -DNTIMES=$(NTIMES)

FLAVOR = vanilla
EXE = stream_$(FLAVOR).exe

SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(EXE)

clean:
	rm -f $(OBJS) $(EXE)

run: $(EXE)
	$(RUN) ./$^

$(EXE): stream.o kernel-$(FLAVOR).o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c Makefile $(HDRS)
	$(CC) -c $(CFLAGS) -o $@ $<
