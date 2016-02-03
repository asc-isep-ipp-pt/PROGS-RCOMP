
P1=tcp_srv_http

all: $(P1)

$(P1): $(P1).c http_get.c http_post.c aux.c
	$(CC) -o $(P1) $(P1).c


clean:
	rm -f $(P1)

