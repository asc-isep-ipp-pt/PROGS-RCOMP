CC=gcc

PROGS=tcp_cli_sum_TLS tcp_srv_sum_TLS

all: $(PROGS)

tcp_cli_sum_TLS: tcp_cli_sum_TLS.c
	gcc -Wall tcp_cli_sum_TLS.c -o tcp_cli_sum_TLS -lssl -lcrypto

tcp_srv_sum_TLS: tcp_srv_sum_TLS.c
	gcc -Wall tcp_srv_sum_TLS.c -o tcp_srv_sum_TLS -lssl -lcrypto

certs:
	./make_certs

clean:
	rm -f $(PROGS) *.crt *.key *.pem

