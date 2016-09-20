CC=gcc

P=tcp_srv_http

all: $(P)

$(P): $(P).c http_get.c http_post.c aux.c
	$(CC) -o $(P) $(P).c


clean:
	rm -f $(P)

