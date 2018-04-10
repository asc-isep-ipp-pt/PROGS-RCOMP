#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 300
#define SERVER_PORT "9999"

// read a string from stdin protecting buffer overflow
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

int main(int argc, char **argv) {
	struct sockaddr_storage serverAddr;
	int sock, res, err;
	unsigned int serverAddrLen;
	char linha[BUF_SIZE];
	struct addrinfo  req, *list;

	if(argc!=2) {
		puts("Server IPv4/IPv6 address or DNS name is required as argument");
		exit(1);
		}

	bzero((char *)&req,sizeof(req));
	req.ai_family = AF_UNSPEC;		// let getaddrinfo set the family depending on the supplied server address
	req.ai_socktype = SOCK_DGRAM;
	err=getaddrinfo(argv[1], SERVER_PORT , &req, &list);
	if(err) {
        	printf("Failed to get server address, error: %s\n",gai_strerror(err)); exit(1); }
	serverAddrLen=list->ai_addrlen;
	memcpy(&serverAddr,list->ai_addr,serverAddrLen);  // store the server address for later use when sending requests
	freeaddrinfo(list);

	bzero((char *)&req,sizeof(req));
	
	// for the local address, request the same family asdetermined for the server address
	req.ai_family = serverAddr.ss_family;
	req.ai_socktype = SOCK_DGRAM;
	req.ai_flags = AI_PASSIVE;			// local address
	err=getaddrinfo(NULL, "0" , &req, &list);	// port 0 = auto assign
	if(err) {
        	printf("Failed to get local address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
		perror("Failed to open socket"); freeaddrinfo(list); exit(1);}
	if(bind(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) { 
		perror("Failed to bind socket");close(sock);freeaddrinfo(list);exit(1);}

	freeaddrinfo(list);

	while(1) {
		printf("Request sentence to send (\"exit\" to quit): ");
	        GETS(linha,BUF_SIZE);
		if(!strcmp(linha,"exit")) break;
		sendto(sock,linha,strlen(linha),0,(struct sockaddr *)&serverAddr,serverAddrLen);
		res=recvfrom(sock,linha,BUF_SIZE,0,(struct sockaddr *)&serverAddr,&serverAddrLen);
	        linha[res]=0; /* NULL terminate the string */
		printf("Received reply: %s\n",linha);
        	}	
	close(sock);
	exit(0);
	}
