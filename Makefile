CC=gcc

all: ntpclient

ntpclient: ntpclient.c
	$(CC) -g -o ntpclient ntpclient.c lib.c packets.c servers.c -lm

clean:
	rm ntpclient
