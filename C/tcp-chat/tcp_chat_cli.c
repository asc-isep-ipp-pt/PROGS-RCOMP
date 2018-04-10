#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define BUF_SIZE 300
#define SERVER_PORT "9999"

// read a string from stdin protecting buffer overflow
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

int main(int argc, char **argv) {
	int err, sock;
	unsigned char lsize;
	char nick[BUF_SIZE], linha[BUF_SIZE], buff[BUF_SIZE];
	struct addrinfo  req, *list;
	fd_set rfds;


	if(argc!=2) {
        	puts("Server IPv4/IPv6 address or DNS name is required as argument");
        	exit(1); }

	bzero((char *)&req,sizeof(req));
	// let getaddrinfo set the family depending on the supplied server address
	req.ai_family = AF_UNSPEC;
	req.ai_socktype = SOCK_STREAM;
	err=getaddrinfo(argv[1], SERVER_PORT , &req, &list);
	if(err) {
        	printf("Failed to get server address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(connect(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Failed connect"); freeaddrinfo(list); close(sock); exit(1);}

	freeaddrinfo(list);

	printf("Connected, enter nickname: ");GETS(nick,BUF_SIZE);

	for(;;) {
        	FD_ZERO(&rfds);
        	FD_SET(0,&rfds); FD_SET(sock,&rfds);
        	select(sock+1,&rfds,NULL,NULL,NULL);
        	if(FD_ISSET(0,&rfds)) {
                	GETS(linha,BUF_SIZE);
                	if(!strcmp(linha,"exit")) {
                        	lsize=0;
                        	write(sock,&lsize,1);
                        	read(sock,&lsize,1);
                        	break;
                        	}
                	sprintf(buff,"(%s) %s",nick,linha);
                	lsize=strlen(buff);
                	write(sock,&lsize,1);
               		write(sock,buff,lsize);
                	}
        	if(FD_ISSET(sock,&rfds)) {
                	read(sock,&lsize,sizeof(lsize));
                	read(sock,buff,lsize);
			buff[lsize]=0;
                	puts(buff);
                	}
      		}
	close(sock);
	exit(0);
	}

