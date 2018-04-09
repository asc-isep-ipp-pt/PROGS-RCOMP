#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "http.h"

void readLineCRLF(int sock, char *line) {
	char *aux=line;
	for(;;) {
		read(sock,aux,1);
		if(*aux=='\n') {
			*aux=0;return;
			}
		else
		if(*aux!='\r') aux++;
		}
	}

void writeLineCRLF(int sock, char *line) {
	char *aux=line;
	while(*aux) {write(sock,aux,1); aux++;}
	write(sock,"\r\n",2);
	}



void sendHttpResponseHeader(int sock, char *status, char *contentType, int contentLength) {
	char aux[200];
	sprintf(aux,"%s %s",HTTP_VERSION,status);
	writeLineCRLF(sock,aux);
	sprintf(aux,"Content-type: %s",contentType);
	writeLineCRLF(sock,aux);
	sprintf(aux,"Content-length: %d",contentLength);
	writeLineCRLF(sock,aux);
	writeLineCRLF(sock,HTTP_CONNECTION_CLOSE);
	writeLineCRLF(sock,"");
	}

int sendHttpResponse(int sock, char *status, char *contentType, char *content, int contentLength) {
	int done, todo;
	char *aux;
	sendHttpResponseHeader(sock, status, contentType, contentLength);
	aux=content; todo=contentLength;
	while(todo) {
		done=write(sock,aux,todo);
		if(done<1) return(0);
		todo=todo-done;
		aux=aux+done;
		}
	return(1);
	}

void sendHttpStringResponse(int sock, char *status, char *contentType, char *content) {
	sendHttpResponse(sock,status,contentType,content,strlen(content));
	}



void sendHttpFileResponse(int sock, char *status, char *filename) {
	FILE *f;
	char *aux;
	char line[200];
	int done;
	long len;
	char *contentType="text/html";

	f=fopen(filename,"r");
	if(!f) {
		sendHttpStringResponse(sock, "404 Not Found", contentType, "<html><body><h1>404 File not found</h1></body></html>");
		return;
		}
	aux=filename+strlen(filename)-1;
	while(*aux!='.' && aux!=filename) aux--;
	if(*aux=='.')
		{
		if(!strcmp(aux,".pdf")) contentType="application/pdf";
		else
		if(!strcmp(aux,".js")) contentType="application/javascript";
		else
		if(!strcmp(aux,".txt")) contentType="text/plain";
		else
		if(!strcmp(aux,".gif")) contentType="image/gif";
		else
		if(!strcmp(aux,".png")) contentType="image/png";
		}
	else
		contentType="application/x-binary";
	
	fseek(f,0,SEEK_END);
	len=ftell(f);
	if(!status) status="200 Ok";
	sendHttpResponseHeader(sock, status, contentType, len);
	rewind(f);
	do {
		done=fread(line,1,200,f);
		if(done>0) write(sock,line,done);
		}
	while(done);
	fclose(f);
	}







