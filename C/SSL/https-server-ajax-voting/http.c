#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "http.h"

void readLineCRLF(SSL *sslConn, char *line) {
	char *aux=line;
	for(;;) {
		*aux=0;
		SSL_read(sslConn,aux,1);
		if(!*aux) return;
		if(*aux=='\n') {
			*aux=0;return;
			}
		else
		if(*aux!='\r') aux++;
		}
	}

void writeLineCRLF(SSL *sslConn, char *line) {
	char *aux=line;
	while(*aux) {SSL_write(sslConn,aux,1); aux++;}
	SSL_write(sslConn,"\r\n",2);
	}



void sendHttpResponseHeader(SSL *sslConn, char *status, char *contentType, int contentLength) {
	char aux[200];
	sprintf(aux,"%s %s",HTTP_VERSION,status);
	writeLineCRLF(sslConn,aux);
	sprintf(aux,"Content-type: %s",contentType);
	writeLineCRLF(sslConn,aux);
	sprintf(aux,"Content-length: %d",contentLength);
	writeLineCRLF(sslConn,aux);
	writeLineCRLF(sslConn,HTTP_CONNECTION_CLOSE);
	writeLineCRLF(sslConn,"");
	}

int sendHttpResponse(SSL *sslConn, char *status, char *contentType, char *content, int contentLength) {
	int done, todo;
	char *aux;
	sendHttpResponseHeader(sslConn, status, contentType, contentLength);
	aux=content; todo=contentLength;
	while(todo) {
		done=SSL_write(sslConn,aux,todo);
		if(done<1) return(0);
		todo=todo-done;
		aux=aux+done;
		}
	return(1);
	}

void sendHttpStringResponse(SSL *sslConn, char *status, char *contentType, char *content) {
	sendHttpResponse(sslConn,status,contentType,content,strlen(content));
	}



void sendHttpFileResponse(SSL *sslConn, char *status, char *filename) {
	FILE *f;
	char *aux;
	char line[200];
	int done;
	long len;
	char *contentType="text/html";

	f=fopen(filename,"r");
	if(!f) {
		sendHttpStringResponse(sslConn, "404 Not Found", contentType, "<html><body><h1>404 File not found</h1></body></html>");
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
	sendHttpResponseHeader(sslConn, status, contentType, len);
	rewind(f);
	do {
		done=fread(line,1,200,f);
		if(done>0) SSL_write(sslConn,line,done);
		}
	while(done>=0);
	fclose(f);
	}







