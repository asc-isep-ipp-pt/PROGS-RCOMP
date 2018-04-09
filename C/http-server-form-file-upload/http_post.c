#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#include "http.h"
#include "http_post.h"

void processPOST(int sock, char *request, char *baseFolder) {
	if(!strncmp(request+5,"/upload",7)) processPOSTupload(sock, baseFolder);
	else processPOSTlist(sock, baseFolder);
	}


void processPOSTupload(int sock, char *baseFolder) {
	char line[200];
	char separator[100];
	char filename[100];
	char filePath[100];
	int readNow,done,len;
	char *cLen="Content-Length: ";
	char *cTypeMP="Content-Type: multipart/form-data; boundary=";
	char *cDisp="Content-Disposition: form-data; name=\"filename\"; filename=\"";
	int cLenS=strlen(cLen);
	int cTypeMPS=strlen(cTypeMP);
	int cDispS=strlen(cDisp);
	FILE *f;

	*separator=0;
	*filename=0;
	len=0;

	do {	// FIRST HEADER
        	readLineCRLF(sock,line);
		if(!strncmp(line,cLen,cLenS)) {
			len=atoi(line+cLenS);
			}		
		else
			if(!strncmp(line,cTypeMP,cTypeMPS)) {
			strcpy(separator,line+cTypeMPS);
			}
        	}
	while(*line);

	if(!*separator)
		replyPostError(sock, "Content-Type: multipart/form-data; expected and not found");
	if(!len)
		replyPostError(sock, "Content-Length: expected and not found");

	readLineCRLF(sock,line); // SEPARATOR
	if(strcmp(line+2,separator))
		replyPostError(sock, "Multipart separator expected and not found");
	len=len-strlen(line)-2;

	do {	// SECOND HEADER
		readLineCRLF(sock,line);
		len=len-strlen(line)-2;
		if(!strncmp(line,cDisp,cDispS)) {
			strcpy(filename,line+cDispS); filename[strlen(filename)-1]=0;
			}
		}
	while(*line);

	if(!*filename) {
		do {  				// READ THE CONTENT
			done=read(sock,line,200); len=len-done; }
		while(len>0);
		replyPostError(sock, "Content-Disposition: form-data; expected and not found (NO FILENAME)");
		}

	strcpy(filePath,baseFolder); strcat(filePath,"/"); strcat(filePath,filename);
	f=fopen(filePath,"w+");
	if(!f) {
		sprintf(line, "Failed to create %s file\n",filePath);
		replyPostError(sock, line);
		}

	// SUBTRACT THE SEPARATOR LENGHT, PLUS -- ON START PLUS -- ON END PLUS CRLF
	len=len-strlen(separator)-6;

	do { // FILE CONTENT
		if(len>200) readNow=200; else readNow=len;
		done=read(sock,line,readNow);
		len=len-done;
		if(done>0) fwrite(line,1,done,f);
        	}	
	while(len>0);
	readLineCRLF(sock,line);
	fclose(f);
	replyPostList(sock, baseFolder);
	}




void processPOSTlist(int sock, char *baseFolder) {
	char line[200];
	do		// READ (AND IGNORE) THE HEADER
        	{
        	readLineCRLF(sock,line);
        	}
	while(*line);
	replyPostList(sock, baseFolder);
	}

void replyPostList(int sock, char *baseFolder) {
	char *s1="<html><head><title>File List</title></head><body><h1>File List:</h1><big><ul>";
	char *s2="</ul></big><hr><p><a href=/>BACK</a></body></html>";
	char list[1000];
	DIR *d;
	struct dirent *e;

	d=opendir ("www/");
	if(!d) replyPostError(sock,"Failed to open directory for listing");
	strcpy(list,s1);
	do {
		e=readdir(d);  // IGNORE FILENAMES STARTED BY A DOT
		if(e && *e->d_name!='.') {strcat(list,"<li><a href=/");strcat(list,e->d_name);
			strcat(list,"><b>");strcat(list,e->d_name);strcat(list,"</b></a>");}
		}
	while(e);
	closedir(d);
	strcat(list,s2);
	sendHttpStringResponse(sock, "200 Ok", "text/html", list);
	close(sock);exit(0);
	}

void replyPostError(int sock, char *error) {
	char *s1="<html><head><title>Server Error</title></head><body><center><img src=500.png><br>(500.png)</center><h1>Server error on POST</h1><p>ERROR: ";
	char *s2="<hr><p><a href=/>BACK</a></body></html>";
	char line[300];

	strcpy(line,s1); strcat(line,error); strcat(line,s2);
	sendHttpStringResponse(sock, "500 Internal Server Error", "text/html", line);
	close(sock);
	exit(1);
	}
