#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void main(void)
{
struct sockaddr_in me, client;
int sock, adl, res, i;
char linha[300], linha1[300];


sock=socket(AF_INET,SOCK_DGRAM,0);
adl=sizeof(me);
bzero((char *)&me,adl);
me.sin_family=AF_INET;
me.sin_addr.s_addr=htonl(INADDR_ANY); /* endereco IP local */
me.sin_port=htons(9999); /* porto local  */
if(bind(sock,(struct sockaddr *)&me,adl)==-1)
        { perror("O bind falhou"); close(sock); exit(1); }
puts("A escutar pedidos. Use CTRL+C para terminar");
while(1)
        {
        res=recvfrom(sock,linha,300,0,(struct sockaddr *)&client,&adl);
	for(i=0;i<res;i++) linha1[res-1-i]=linha[i];
        sendto(sock,linha1,res,0,(struct sockaddr *)&client,adl);
        }
close(sock);
}
