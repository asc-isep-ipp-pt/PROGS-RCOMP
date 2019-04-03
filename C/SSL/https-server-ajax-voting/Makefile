CC=gcc

P=https_srv_ajax_voting

all: $(P) http.o

$(P): $(P).c http.o
	$(CC) -Wall $(P).c http.o -o $(P) -lssl -lcrypto

http.o: http.c
	$(CC) -Wall -c http.c -o http.o

cert:
	./make_server_cert

clean:
	rm -f $(P) *.o *.key *.pem

