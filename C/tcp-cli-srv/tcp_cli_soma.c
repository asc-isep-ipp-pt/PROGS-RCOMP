#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// ler do stdin uma frase com menos de S carateres para o buffer B
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

void main(int argc, char **argv)
{
struct sockaddr_in target;
int sock;
unsigned long f, i, n, num;
unsigned char bt, linha[30];
   
if(argc!=2) {printf("Utilização: %s Endereço-IP-Servidor\n",argv[0]);exit(1);}

sock=socket(AF_INET,SOCK_STREAM,0);

bzero((char *)&target,sizeof(target));
target.sin_family = AF_INET;
target.sin_addr.s_addr=inet_addr(argv[1]);
target.sin_port=htons(9999);

if(connect(sock,(struct sockaddr *)&target, sizeof(target))==-1) 
	{puts("A ligação falhou"); exit(1);}

do 
	{
        do 
        	{
		printf("Numero inteiro positivo a somar (zero para terminar): ");
		GETS(linha,30);
		while(sscanf(linha,"%li",&num)!=1 || num<0)
			{
			puts("Numero invalido");
			GETS(linha,30);
			}
		n=num;
		for(i=0;i<4;i++) 
			{bt=n%256; write(sock,&bt,1); n=n/256; }
        	}
        while(num);
	num=0; f=1; for(i=0;i<4;i++) {read(sock,&bt,1); num=num+bt*f; f=f*256;}
        printf("SOMA=%lu\n",num);
        }
    while(num);
close(sock);
}

