CC=gcc

PROGS=check_server_TLS

all: $(PROGS)

check_server_TLS: check_server_TLS.c
	gcc -Wall check_server_TLS.c -o check_server_TLS -lssl -lcrypto

clean:
	rm -f $(PROGS)

