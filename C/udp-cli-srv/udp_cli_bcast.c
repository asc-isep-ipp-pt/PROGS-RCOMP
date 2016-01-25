#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ler do stdin uma frase com menos de S carateres para o buffer B
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

void main(int argc, char **argv)
{
struct sockaddr_in me, server;
int sock, adl, res;
char linha[300];

sock=socket(AF_INET,SOCK_DGRAM,0);
adl=1; setsockopt(sock,SOL_SOCKET, SO_BROADCAST, &adl, sizeof(adl));
adl=sizeof(me);

bzero((char *)&me,adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
me.sin_port=htons(0); /* porto local (0=auto assign) */
bind(sock,(struct sockaddr *)&me,adl);

bzero((char *)&server,adl);
server.sin_family=AF_INET;
server.sin_addr.s_addr=inet_addr("172.17.255.255"); /* endereco de broadcast na rede local */
server.sin_port=htons(9999); /* porto do servidor */
while(1)
	{
	printf("Frase a enviar (\"sair\" para terminar): ");
        GETS(linha,300);
	if(!strcmp(linha,"sair")) break;
	sendto(sock,linha,strlen(linha),0,(struct sockaddr *)&server,adl);
	res=recvfrom(sock,linha,300,0,(struct sockaddr *)&server,&adl);
        linha[res]=0; /* NULL terminate the string */
        printf("Recebido: %s\n",linha);
        }
close(sock);
}
