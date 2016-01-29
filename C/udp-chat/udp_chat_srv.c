#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main(void)
{
struct sockaddr_in me, from;
int newSock, sock, adl, i, j;
unsigned char lsize;
int num, soma, maxfd, newMaxfd;
char linha[400];
fd_set rfds, rfds_master;

adl=sizeof(me);
sock=socket(AF_INET,SOCK_STREAM,0);

bzero((char *)&me, adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY);
me.sin_port=htons(9999);

if(-1==bind(sock,(struct sockaddr *)&me, adl))
	{ puts("Porto ocupado"); close(sock); exit(1);}

listen(sock,SOMAXCONN);
FD_ZERO(&rfds_master);
FD_SET(sock,&rfds_master);
newMaxfd=sock;
for(;;)
        {
        maxfd=newMaxfd;
        FD_ZERO(&rfds);
        for(i=0;i<=maxfd;i++) if(FD_ISSET(i,&rfds_master)) FD_SET(i,&rfds);
        select(maxfd+1,&rfds,NULL,NULL,NULL);
        for(i=0;i<=maxfd;i++)
           if(FD_ISSET(i,&rfds_master) && FD_ISSET(i,&rfds))
                {
                if(i==sock)
                        {
                        newSock=accept(sock,(struct sockaddr *)&from,&adl);
                        FD_SET(newSock,&rfds_master);
                        if(newSock>newMaxfd) newMaxfd=newSock;
                        }
                else
                        {
                        read(i,&lsize,1);
                        if(!lsize) { FD_CLR(i,&rfds_master); 
					write(i,&lsize,1);close(i);}
                        else
                                {
                                read(i,linha,lsize);
                                for(j=0;j<=maxfd;j++)
                                  if(j!=sock)
                                    if(FD_ISSET(j,&rfds_master))
                                        {
                                        write(j,&lsize,1);
                                        write(j,linha,lsize);
                                        }

                                }
                        }
                }
        }
}

