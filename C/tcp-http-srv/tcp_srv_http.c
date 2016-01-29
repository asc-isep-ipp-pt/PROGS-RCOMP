#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void processHttpRequest(int sock);

void main(int argc, char **argv)
{
struct sockaddr_in me, from;
int port, newSock, sock, adl;
unsigned long i, f, n, num, soma;
unsigned char bt;

if(argc!=2) {puts("Ops, local TCP port number missing from command line"); exit(1);}
port=atoi(argv[1]);
if(!port) {printf("Ops, invalid local TCP port number: %s\n",argv[1]); exit(1);}
adl=sizeof(me);
sock=socket(AF_INET,SOCK_STREAM,0);

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

void readLineCRLF(int sock, char *line)
{
char *aux=line;
read(sock,


void processHttpRequest(int sock)
{

}
