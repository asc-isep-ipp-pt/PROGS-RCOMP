#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "aux.c"
#include "http_get.c"
#include "http_post.c"

void processHttpRequest(int sock);

void main(int argc, char **argv)
{
struct sockaddr_in me, from;
int i, port, newSock, sock;
socklen_t adl;

if(argc!=2) {puts("Ops, local TCP port number missing from command line"); exit(1);}
port=atoi(argv[1]);
if(!port) {printf("Ops, invalid local TCP port number: %s\n",argv[1]); exit(1);}
adl=sizeof(me);
sock=socket(AF_INET,SOCK_STREAM,0);

i=1; // AVOID LEAVING THE PORT IN USE AFTER A SERVER CRASH
setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));

bzero((char *)&me, adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY);
me.sin_port=htons(port);

if(-1==bind(sock,(struct sockaddr *)&me, adl)){ printf("Ops, TCP port number %i in use\n",port); close(sock); exit(1);}

listen(sock,SOMAXCONN);
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

}

