


void processGET(int sock, char *file)
{
char *aux, *reply;
char line[200];
char filePath[100];
long len;
char *type;
int done;
FILE *f;

do	// READ AND IGNORE HEADER LINES
	{
	readLineCRLF(sock,line);
	}
while(*line);
aux=file;
while(*aux!=32) aux++; *aux=0;
if(*file=='/') file++;
if(!*file) strcpy(file,"index.html"); // DEFAULT FILE IS index.html
strcpy(filePath,"www/");
strcat(filePath,file);
f=fopen(filePath,"r");
reply="200 OK";
type="text/html";
if(!f)
	{
	reply="404 Not Found";
	f=fopen("www/not-found.html","r");
	if(!f) return;
	}
else
	{ // SOME FILE TYPES BY NAME
	aux=filePath+strlen(filePath)-1;
	while(*aux!='.' && aux!=filePath) aux--;
	if(*aux=='.')
		{
		if(!strcmp(aux,".pdf")) type="application/pdf";
		else
		if(!strcmp(aux,".txt")) type="text/plain";
		else
		if(!strcmp(aux,".gif")) type="image/gif";
		else
		if(!strcmp(aux,".png")) type="image/png";
		}
	else
		type="application/x-binary";
	
	}

sprintf(line,"HTTP/1.0 %s",reply);
writeLineCRLF(sock,line);
fseek(f,0,SEEK_END);
len=ftell(f);
sprintf(line,"Content-Length: %li",len);
writeLineCRLF(sock,line);
sprintf(line,"Content-Type: %s",type);
writeLineCRLF(sock,line);
writeLineCRLF(sock,"Connection: close");
writeLineCRLF(sock,"");
rewind(f);
do {
	done=fread(line,1,200,f);
	if(done>0) write(sock,line,done);
	}
while(done);
fclose(f);
}



