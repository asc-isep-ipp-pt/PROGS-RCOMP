CC=gcc

P=http_srv_ajax_voting

all: $(P) http.o

$(P): $(P).c http.o
	$(CC) -Wall $(P).c http.o -o $(P)

http.o: http.c
	$(CC) -Wall -c http.c -o http.o


clean:
	rm -f $(P) *.o

