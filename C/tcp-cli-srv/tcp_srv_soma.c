#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main(void)
{
struct sockaddr_in me, from;
int newSock, sock, adl;
unsigned long i, f, n, num, soma;
unsigned char bt;

adl=sizeof(me);
sock=socket(AF_INET,SOCK_STREAM,0);

bzero((char *)&me, adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY);
me.sin_port=htons(9999);

if(-1==bind(sock,(struct sockaddr *)&me, adl)){ puts("Porta ocupada"); close(sock); exit(1);}

listen(sock,SOMAXCONN);
for(;;)
        {
        newSock=accept(sock,(struct sockaddr *)&from,&adl);
        if(!fork())
                {
                close(sock);
                do  
                    {
                    soma=0;
                    do 
                       {
		       num=0;f=1; for(i=0;i<4;i++) 
				{ read(newSock,&bt,1); num=num+bt*f; f=256*f;}
                       soma=soma+num;
                       }
                    while(num);
		    n=soma;
		    for(i=0;i<4;i++)
                        {bt=n%256; write(newSock,&bt,1); n=n/256; }
                    } 
                while(soma);
                close(newSock);
                exit(0);
                }
        close(newSock);
        }
close(sock);
}

