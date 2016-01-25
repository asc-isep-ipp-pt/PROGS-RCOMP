#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void main(void)
{
struct sockaddr_in me, from;
int sock, rec, adl;
char linha[300];

sock=socket(AF_INET,SOCK_DGRAM,0);
adl=sizeof(me);
bzero((char *)&me,adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
me.sin_port=htons(9999); /* porto local  */
if(bind(sock,(struct sockaddr *)&me,adl)==-1)
	{ perror("O bind falhou"); close(sock); exit(1); }
do
	{
	rec=recvfrom(sock,linha,300,0,(struct sockaddr *)&from,&adl);
	linha[rec]=0; /* NULL terminate the string */
	printf("Recebido: %s\n",linha);
	}
while(strcmp(linha,"sair"));
close(sock);
}
