#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include "http.h"

#define BASE_FOLDER "www"

void processHttpRequest(int sock, int conSock); // implemented ahead
void processGET(int sock, char *requestLine);	// implemented ahead
void processPUT(int sock, char *requestLine);	// implemented ahead

#define NUM_CANDIDATES 4
char *candidateName[] = { "Candidate A", "Candidate B", "Candidate C" , "Candidate D" };
int candidateVotes[NUM_CANDIDATES];
unsigned int httpAccessesCounter=0;

int main(int argc, char **argv) {
	struct sockaddr_storage from;
	int err, i, newSock, sock;
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

	listen(sock,SOMAXCONN);
	adl=sizeof(from);

	for(i=0; i<NUM_CANDIDATES; i++) candidateVotes[i]=0;

	signal(SIGCHLD, SIG_IGN); // AVOID LEAVING TERMINATED CHILD PROCESSES AS ZOMBIES

	while(1) {
        	newSock=accept(sock,(struct sockaddr *)&from,&adl);
		httpAccessesCounter++;
		processHttpRequest(sock,newSock);
        	}
	close(sock);
	return(0);
	}

//////////////////////////////////////////////////////////////////////////////////

void processHttpRequest(int sock, int conSock) {
	char requestLine[200];

	readLineCRLF(conSock,requestLine);
	// printf("Request received: %s\n", requestLine);
	if(!strncmp(requestLine,"GET /",5)) {
		if(!fork()) {			// GET requests are processed in background
			close(sock);
			processGET(conSock,requestLine);
			close(conSock);
			exit(0);
			}
		close(conSock);
		return;
		}
	if(!strncmp(requestLine,"PUT /votes/",11)) processPUT(conSock,requestLine);
	else {
		sendHttpStringResponse(conSock, "405 Method Not Allowed", "text/html",
			"<html><body>HTTP method not supported</body></html>");
		puts("Oops, the method is not supported by this server");
		}

	close(conSock);
	}

//////////////////////////////////////////////////////////////////////////////////


void sendVotes(int sock) {
	char buffer[1024], line[200];;
	strcpy(buffer,"<hr><ul>");
	for(int i=0; i<NUM_CANDIDATES; i++) {
		sprintf(line,"<li><button type=\"button\" onclick=\"voteFor(%i)\">Vote for %s</button> %s - %d votes </li>",
			i+1, candidateName[i], candidateName[i], candidateVotes[i] ); 
		strcat(buffer,line);
		}
	sprintf(line, "</ul><hr><p>HTTP server accesses counter: %u</p><hr>", httpAccessesCounter);
	strcat(buffer,line);
	sendHttpStringResponse(sock, "200 Ok", "text/html", buffer);
	}


void processGET(int sock, char *requestLine) {
	char *aux, line[200], filePath[100], uri[100];

	do {	// READ AND IGNORE HEADER LINES
		readLineCRLF(sock,line);
		}
	while(*line);

	strcpy(uri,requestLine+4);
	aux=uri;
	while(*aux!=32) aux++; *aux=0;
	if(!strncmp(uri,"/votes",8)) {
		sendVotes(sock); return;
		}
	if(!strcmp(uri,"/")) strcpy(uri,"/index.html"); // BASE URI
	strcpy(filePath,BASE_FOLDER);
	strcat(filePath,uri);
	sendHttpFileResponse(sock, NULL, filePath);
	}

void processPUT(int sock, char *requestLine) {
	char *aux, line[200], uri[100];
	int candidate;

	do {	// READ AND IGNORE HEADER LINES
		readLineCRLF(sock,line);
		}
	while(*line);

	strcpy(uri,requestLine+4);
	aux=uri;
	while(*aux!=32) aux++; *aux=0;
	aux=uri+strlen(uri)-1;
	while(*aux!='/') aux--; // FIND LAST SLASH
	aux++;
	candidate=atoi(aux);
	candidate--; // CONVERT TO INDEX VALUE
	if(candidate<0||candidate>NUM_CANDIDATES) {	// BAD CANDIDATE INDEX
		sendHttpStringResponse(sock, "405 Method Not Allowed", "text/html",
			"<html><body>HTTP method not supported</body></html>");
		return;
		}
	candidateVotes[candidate]++;
	sendHttpStringResponse(sock, "200 Ok", "text/plain","");
	}



