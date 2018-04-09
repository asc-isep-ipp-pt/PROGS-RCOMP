CC=gcc

P=http_srv_form_file_upload

all: $(P) http.o http_post.o

$(P): $(P).c http.o http_post.o
	$(CC) -Wall $(P).c http.o -o $(P)

http.o: http.c
	$(CC) -Wall -c http.c -o http.o


http_post.o: http_post.c http.o
	$(CC) -Wall -c http_post.c -o http_post.o


clean:
	rm -f $(P) *.o

