#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

#include "http.h"
#include "http_post.c"

#define BASE_FOLDER "www"


void processHttpRequest(int sock); //declared ahead

int main(int argc, char **argv) {
	struct sockaddr_storage from;
	int err, newSock, sock;
	socklen_t adl;
	struct addrinfo  req, *list;

	if(argc!=2) {puts("Oops, local TCP port number missing from command line"); exit(1);}

	bzero((char *)&req,sizeof(req));
	req.ai_family = AF_INET6;       // requesting a IPv6 local address will allow both IPv4 and IPv6 clients to use it
	req.ai_socktype = SOCK_STREAM;
	req.ai_flags = AI_PASSIVE;      // local address

	err=getaddrinfo(NULL, argv[1] , &req, &list);

	if(err) {
        	printf("Failed to get local address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(bind(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Bind failed");close(sock);freeaddrinfo(list);exit(1);}

	freeaddrinfo(list);

	signal(SIGCHLD, SIG_IGN); // AVOID LEAVING TERMINATED CHILD PROCESSES AS ZOMBIES

	listen(sock,SOMAXCONN);
	adl=sizeof(from);
	for(;;) {
        	newSock=accept(sock,(struct sockaddr *)&from,&adl); // WAIT FOR CLIENT CONNECTION
        	if(!fork()) {
                	close(sock);
			processHttpRequest(newSock);
                	close(newSock);
                	exit(0);
                	}
        	close(newSock);
        	}
	close(sock);
	}


void processGET(int sock, char *requestLine); // declared ahead

void processHttpRequest(int sock) {
	char requestLine[200];

	readLineCRLF(sock,requestLine);
	// printf("Request: %s\n", requestLine);
	if(!strncmp(requestLine,"GET /",5)) processGET(sock,requestLine);
	else
	if(!strncmp(requestLine,"POST /",6)) processPOST(sock,requestLine, BASE_FOLDER);
	else {
		sendHttpStringResponse(sock, "405 Method Not Allowed", "text/html",
                        "<html><body>HTTP method not supported</body></html>");
		puts("Oops, the method is not supported by this server");
		}

	}

void processGET(int sock, char *requestLine) {
        char *aux, line[200];
        char filePath[100];
        char uri[100];

        do {    // READ AND IGNORE HEADER LINES
                readLineCRLF(sock,line);
                }
        while(*line);

        strcpy(uri,requestLine+4);
        aux=uri;
        while(*aux!=32) aux++; *aux=0;
        if(!strcmp(uri,"/")) strcpy(uri,"/index.html"); // BASE URI
        strcpy(filePath,BASE_FOLDER);
        strcat(filePath,uri);
        sendHttpFileResponse(sock, NULL, filePath);
        }






