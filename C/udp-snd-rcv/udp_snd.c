#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ler stdin a frase com menos de S carateres para o buffer B
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

void main(void)
{
struct sockaddr_in me, target;
int sock,adl;
char linha[300];

sock=socket(AF_INET,SOCK_DGRAM,0);
adl=sizeof(me);
bzero((char *)&me,adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
me.sin_port=htons(0); /* porto local (0=auto assign) */
bind(sock,(struct sockaddr *)&me,adl);
bzero((char *)&target,adl);
target.sin_family=AF_INET;
printf("Endereco IPv4 de destino: "); 
GETS(linha,300);
target.sin_addr.s_addr=inet_addr(linha);
target.sin_port=htons(9999); /* porto de destino */
do
	{
	printf("Frase a enviar (\"sair\" para terminar): ");
	GETS(linha,300);
	sendto(sock,linha,strlen(linha),0,(struct sockaddr *)&target,adl);
	}
while(strcmp(linha,"sair"));
close(sock);
}
