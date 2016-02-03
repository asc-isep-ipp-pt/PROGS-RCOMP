


void readLineCRLF(int sock, char *line)
{
char *aux=line;
for(;;) {
	read(sock,aux,1);
	if(*aux=='\n')
		{
		*aux=0;return;
		}
	else
	if(*aux!='\r') aux++;
	}
}

void writeLineCRLF(int sock, char *line)
{
char *aux=line;
while(*aux) {write(sock,aux,1); aux++;}
write(sock,"\r\n",2);
}

