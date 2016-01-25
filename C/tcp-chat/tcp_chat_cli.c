#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netdb.h>

// ler do stdin uma frase com menos de S carateres para o buffer B
#define GETS(B,S) {fgets(B,S-2,stdin);B[strlen(B)-1]=0;}

void main(int argc, char **argv)
{
struct sockaddr_in target;
struct hostent *he;
int sock, timeout;
unsigned char lsize;
char nick[50], linha[180], buff[200];
fd_set rfds;


if(argc!=2) {puts("falta indicar o nome do servidor");exit(1);}

he=gethostbyname(argv[1]);
if(!he) {printf("Falhou a resolução do nome %s\n",argv[1]);exit(1);}

sock=socket(AF_INET,SOCK_STREAM,0);

bzero((char *)&target,sizeof(target));
target.sin_family = AF_INET;
target.sin_addr=*(struct in_addr *)he->h_addr_list[0];
target.sin_port=htons(9999);

if(connect(sock,(struct sockaddr *)&target, sizeof(target))==-1) {
        puts("A ligação falhou"); close(sock); exit(1); }

printf("Nickname:");GETS(nick,50);

    for(;;) {
        FD_ZERO(&rfds);
        FD_SET(0,&rfds); FD_SET(sock,&rfds);
        select(sock+1,&rfds,NULL,NULL,NULL);
        if(FD_ISSET(0,&rfds))
                {
                GETS(linha,300);
                if(!strcmp(linha,"sair"))
                        {
                        lsize=0;
                        write(sock,&lsize,1);
                        read(sock,&lsize,1);
                        break;
                        }
                sprintf(buff,"(%s) %s",nick,linha);
                lsize=strlen(buff);
                write(sock,&lsize,1);
                write(sock,buff,lsize);
                }
        if(FD_ISSET(sock,&rfds))
                {
                read(sock,&lsize,sizeof(lsize));
                read(sock,buff,lsize);
		buff[lsize]=0;
                puts(buff);
                }
      }

    close(sock);
    }

