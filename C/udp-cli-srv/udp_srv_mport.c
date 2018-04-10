#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 300
#define SERVER_BASE_PORT 9009
#define SERVER_NUM_PORTS 6
#define SERVER_PORT_STEP 100

int main(void) {
	struct sockaddr_storage client;
	int sock[SERVER_NUM_PORTS], err, res, i, a, max;
	unsigned int adl;
	char linha[BUF_SIZE], linha1[BUF_SIZE];
	char IPtext[BUF_SIZE], portText[BUF_SIZE];
	struct addrinfo  req, *list;
	fd_set readSocks;

	max=0;
	for(i=0; i<SERVER_NUM_PORTS; i++) {
		bzero((char *)&req,sizeof(req));
		// requesting a IPv6 local address will allow both IPv4 and IPv6 clients to use it
		req.ai_family = AF_INET6;
		req.ai_socktype = SOCK_DGRAM;
		req.ai_flags = AI_PASSIVE;      // local address
		sprintf(portText,"%i",SERVER_BASE_PORT+SERVER_PORT_STEP*i);

		err=getaddrinfo(NULL, portText , &req, &list);
		if(err) {
			printf("Failed to get local address for port %s, error: %s\n",
				portText,gai_strerror(err)); exit(1); }

  		sock[i]=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
  		if(sock[i]==-1) {
        		perror("Failed to open socket"); freeaddrinfo(list); i--;
			do {close(sock[i]); i--; } while(i>-1); exit(1);}

		if(bind(sock[i],(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        		perror("Bind failed");freeaddrinfo(list);
			do {close(sock[i]); i--; } while(i>-1); exit(1);}

		freeaddrinfo(list);
		if(sock[i]>max) max=sock[i];
		}
	max++;

	puts("Listening for UDP requests (both over IPv6 or IPv4). Use CTRL+C to terminate the server");
	adl=sizeof(client);
	while(1) {
		FD_ZERO(&readSocks); for(i=0;i<SERVER_NUM_PORTS;i++) FD_SET(sock[i], &readSocks);
		select(max,&readSocks,NULL,NULL,NULL);
		for(i=0;i<SERVER_NUM_PORTS;i++)
		  if(FD_ISSET(sock[i],&readSocks)) {
           		res=recvfrom(sock[i],linha,BUF_SIZE,0,(struct sockaddr *)&client,&adl);
	   		if(!getnameinfo((struct sockaddr *)&client,adl,
                        	IPtext,BUF_SIZE,portText,BUF_SIZE,NI_NUMERICHOST|NI_NUMERICSERV))
                		printf("Request from node %s, port number %s\n", IPtext, portText);
        		else puts("Got request, but failed to get client address");
	   		for(a=0;a<res;a++) linha1[res-1-a]=linha[a];
           		sendto(sock[i],linha1,res,0,(struct sockaddr *)&client,adl);
	   		}
        	}
	exit(0);
	}


