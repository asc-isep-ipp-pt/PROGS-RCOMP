#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "aux.c"
#include "http_get.c"
#include "http_post.c"

void processHttpRequest(int sock);

void main(int argc, char **argv)
{
struct sockaddr_storage from;
int err, i, port, newSock, sock;
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

i=1; // AVOID LEAVING THE PORT IN USE AFTER A SERVER CRASH
setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

listen(sock,SOMAXCONN);
adl=sizeof(from);
for(;;)
        {
        newSock=accept(sock,(struct sockaddr *)&from,&adl);
        if(!fork())
                {
                close(sock);
		processHttpRequest(newSock);
                close(newSock);
                exit(0);
                }
        close(newSock);
        }
close(sock);
}

void processHttpRequest(int sock)
{
char requestLine[200];

readLineCRLF(sock,requestLine);
printf("Request: %s\n", requestLine);
if(!strncmp(requestLine,"GET ",4)) processGET(sock,requestLine+5);
else
if(!strncmp(requestLine,"POST ",5)) processPOST(sock,requestLine);
else
	puts("Oops, the method is not supported by me");

}

