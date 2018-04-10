#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 300
#define PORT_NUMBER "9999"
#define BCAST_ADDRESS "255.255.255.255"
#define MAX_PEERS 100

#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

int main(int argc, char **argv) {

	struct sockaddr_storage peerAddr[MAX_PEERS];
	char peerActive[MAX_PEERS];

	struct sockaddr_storage bcastAddr, currPeerAddr;
	socklen_t addrLen;
	int i, err, sock;
	char nick[BUF_SIZE], linha[BUF_SIZE], buff[BUF_SIZE];
	struct addrinfo  req, *list;
	fd_set rfds;


	bzero((char *)&req,sizeof(req));
	req.ai_family = AF_INET;	// we use broadcast, so there is no point in supporting IPv6
	req.ai_socktype = SOCK_DGRAM;	// UDP
	err=getaddrinfo(BCAST_ADDRESS, PORT_NUMBER , &req, &list);
	if(err) {
        	printf("Failed to get the broadcast address, error: %s\n",gai_strerror(err)); exit(1); }
	addrLen=list->ai_addrlen;
	memcpy(&bcastAddr,list->ai_addr,addrLen);
	freeaddrinfo(list);

	bzero((char *)&req,sizeof(req));
	req.ai_family = AF_INET;	// we use broadcast, so there is no point in supporting IPv6
	req.ai_socktype = SOCK_DGRAM;	// UDP
	req.ai_flags = AI_PASSIVE;      // local address
	err=getaddrinfo(NULL, PORT_NUMBER , &req, &list);
	if(err) {
        	printf("Failed to get local address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(bind(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Bind failed");close(sock);freeaddrinfo(list);exit(1);}

	freeaddrinfo(list);

	i=1;setsockopt(sock,SOL_SOCKET, SO_BROADCAST, &i, sizeof(i));

	for(i=0;i<MAX_PEERS;i++) peerActive[i]=0; // to start, all peers inactive

	// ACTION STARTS

	printf("Enter nickname: ");GETS(nick,BUF_SIZE);

	buff[0]=1;
	// send a peer start announcement to broadcast address
	sendto(sock, &buff, 1, 0, (struct sockaddr *) &bcastAddr, addrLen);

	for(;;) {
        	FD_ZERO(&rfds);
       		FD_SET(0,&rfds); FD_SET(sock,&rfds);
        	select(sock+1,&rfds,NULL,NULL,NULL);

        	if(FD_ISSET(0,&rfds)) { // user wrote something on the console
                	GETS(linha,BUF_SIZE);
                	if(!strcmp(linha,"EXIT")) break;
                	if(!strcmp(linha,"LIST")) {
				printf("Active peers list:");
				for(i=0;i<MAX_PEERS;i++)
                      	   		if(peerActive[i]) {
						getnameinfo((struct sockaddr *) &peerAddr[i], addrLen, buff,BUF_SIZE,
								NULL,0,NI_NUMERICHOST|NI_NUMERICSERV);
						printf(" %s",buff);
						}
				printf("\n");
				}
			else {
                		sprintf(buff,"(%s) %s",nick,linha);
				for(i=0;i<MAX_PEERS;i++)  // send the text line to all active peers
                      	   		if(peerActive[i]) {
						sendto(sock,&buff,strlen(buff),0, (struct sockaddr *) &peerAddr[i], addrLen);
						}
				}
                	}

        	if(FD_ISSET(sock,&rfds)) {
			err=recvfrom(sock, &buff, BUF_SIZE, 0, (struct sockaddr *) &currPeerAddr, &addrLen);
			if(err>0) {
				if(buff[0]==1) {  // peer start announcement
					for(i=0;i<MAX_PEERS;i++)
                        			if(peerActive[i])
							if(!memcmp(&peerAddr[i],&currPeerAddr,addrLen)) break;

					if(i==MAX_PEERS) { // new peer
						for(i=0;i<MAX_PEERS;i++) if(!peerActive[i]) break;
						if(i==MAX_PEERS) puts("Sorry, no space for more peers");
						else {
							peerActive[i]=1;
							memcpy(&peerAddr[i],&currPeerAddr,addrLen);
							buff[0]=1; // send back a peer start announcement
							sendto(sock, &buff, 1, 0, (struct sockaddr *) &currPeerAddr, addrLen);
							}
						}
					}
				else
					if(buff[0]==0) {  // peer exit announcement
						for(i=0;i<MAX_PEERS;i++)
                        				if(peerActive[i])
								if(!memcmp(&peerAddr[i],&currPeerAddr,addrLen)) break;
						if(i<MAX_PEERS) peerActive[i]=0;
						}
					else {
						buff[err]=0;
                				puts(buff);
						}
					}
                		}
		}
	buff[0]=0;
	for(i=0;i<MAX_PEERS;i++)  // send exit announcement to all active peers
    		if(peerActive[i])
			sendto(sock, &buff, 1, 0, (struct sockaddr *) &peerAddr[i], addrLen);
	close(sock);
	exit(0);
	}

