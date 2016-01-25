#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BASE_PORT 9009
void main(void)
{
struct sockaddr_in me, client;
int sock[6], adl, res, i, a, max;
char linha[300], linha1[300];
fd_set readSocks;

adl=sizeof(me);
max=0;
for(i=0; i<6; i++) 
  {
  sock[i]=socket(AF_INET,SOCK_DGRAM,0);
  bzero((char *)&me,adl);
  me.sin_family=AF_INET;
  me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
  me.sin_port=htons(BASE_PORT+100*i); /* porto local  */
  if(bind(sock[i],(struct sockaddr *)&me,adl)==-1)
        { 
	perror("O bind falhou");
	printf("Porto=%i\n",BASE_PORT+100*i);
	do {close(sock[i]); i--; } while(i>-1);
	exit(1); }
  if(sock[i]>max) max=sock[i];
  }
max++;

puts("A escutar pedidos. Use CTRL+C para terminar");
while(1)
        {
	FD_ZERO(&readSocks); for(i=0;i<6;i++) FD_SET(sock[i], &readSocks);
	select(max,&readSocks,NULL,NULL,NULL);
	for(i=0;i<6;i++)
	  if(FD_ISSET(sock[i],&readSocks))
	   {
           res=recvfrom(sock[i],linha,300,0,(struct sockaddr *)&client,&adl);
	   for(a=0;a<res;a++) linha1[res-1-a]=linha[a];
           sendto(sock[i],linha1,res,0,(struct sockaddr *)&client,adl);
	   }
        }
}
