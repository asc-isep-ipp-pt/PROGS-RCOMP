#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUF_SIZE 400
#define SERVER_PORT "9999"

int main(void) {
	struct sockaddr_storage from;
	int err, newSock, sock, i, j;
	unsigned int adl;
	unsigned char lsize;
	int maxfd, newMaxfd;
	char linha[BUF_SIZE], cliIPtext[BUF_SIZE], cliPortText[BUF_SIZE];
	struct addrinfo  req, *list;
	fd_set rfds, rfds_master;

	bzero((char *)&req,sizeof(req));
	// requesting a IPv6 local address will allow both IPv4 and IPv6 clients to use it
	req.ai_family = AF_INET6;
	req.ai_socktype = SOCK_STREAM;	// TCP
	req.ai_flags = AI_PASSIVE;      // local address

	err=getaddrinfo(NULL, SERVER_PORT , &req, &list);

	if(err) {
        	printf("Failed to get local address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(bind(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Bind failed");close(sock);freeaddrinfo(list);exit(1);}

	freeaddrinfo(list);

	listen(sock,SOMAXCONN);
	FD_ZERO(&rfds_master);
	FD_SET(sock,&rfds_master);
	newMaxfd=sock;

	puts("Accepting TCP connections (IPv6/IPv4). Use CTRL+C to terminate the server");

	adl=sizeof(from);
	for(;;) {
        	maxfd=newMaxfd;
        	FD_ZERO(&rfds);
        	for(i=0;i<=maxfd;i++) if(FD_ISSET(i,&rfds_master)) FD_SET(i,&rfds);
        	select(maxfd+1,&rfds,NULL,NULL,NULL);
        	for(i=0;i<=maxfd;i++)
           		if(FD_ISSET(i,&rfds_master) && FD_ISSET(i,&rfds)) {
                		if(i==sock) {
                        		newSock=accept(sock,(struct sockaddr *)&from,&adl);
					getnameinfo((struct sockaddr *)&from,adl,cliIPtext,BUF_SIZE,cliPortText,BUF_SIZE,
						NI_NUMERICHOST|NI_NUMERICSERV);
                			printf("New connection from node %s, port number %s\n", cliIPtext, cliPortText);
                        		FD_SET(newSock,&rfds_master);
                        		if(newSock>newMaxfd) newMaxfd=newSock;
                        		}
                		else {
                        		read(i,&lsize,1);
                        		if(!lsize) {
							FD_CLR(i,&rfds_master); 
							write(i,&lsize,1);close(i);
							puts("One client disconnected");
							}
                        		else {
                        			read(i,linha,lsize);
                                		for(j=0;j<=maxfd;j++)
                                			if(j!=sock)
                                				if(FD_ISSET(j,&rfds_master)) {
                                        					write(j,&lsize,1);
                                        					write(j,linha,lsize);
                                        					}

                                		}
                			}
        			}
			}
	exit(0);
	}


