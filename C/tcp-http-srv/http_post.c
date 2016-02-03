
#include <dirent.h>

void replyPostError(int sock, char *content);
void replyPostList(int sock);

void processPOSTupload(int sock);
void processPOSTlist(int sock);

void processPOST(int sock, char *request)
{
if(!strncmp(request+5,"/upload",7)) processPOSTupload(sock);
else processPOSTlist(sock);
}


void processPOSTupload(int sock)
{
char line[200];
char separator[100];
char filename[100];
char filePath[100];
int done,len;
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

do // FIRSH HEADER
        {
        readLineCRLF(sock,line);
	if(!strncmp(line,cLen,cLenS))
		{
		len=atoi(line+cLenS);
		}	
	else
	if(!strncmp(line,cTypeMP,cTypeMPS))
		{
		strcpy(separator,line+cTypeMPS);
		}
        }
while(*line);

if(!*separator) replyPostError(sock, "Content-Type: multipart/form-data; expected and not found");
if(!len) replyPostError(sock, "Content-Length: expected and not found");

readLineCRLF(sock,line); // SEPARATOR
if(strcmp(line+2,separator)) replyPostError(sock, "Multipart separator expected and not found");
len=len-strlen(line)-2;
do // SECOND HEADER
        {
	readLineCRLF(sock,line);
	len=len-strlen(line)-2;
	if(!strncmp(line,cDisp,cDispS))
		{
		strcpy(filename,line+cDispS); filename[strlen(filename)-1]=0;
		}
	}
while(*line);

if(!*filename) 
	{
	do {  // READ THE CONTENT
		done=read(sock,line,200); len=len-done; }
	while(len>0);
	replyPostError(sock, "Content-Disposition: form-data; expected and not found (NO FILENAME)");
	}

strcpy(filePath,"www/");
strcat(filePath,filename);
f=fopen(filePath,"w+");
if(!f) {
	sprintf(line, "Failed to create %s file\n",filePath);
	replyPostError(sock, line);
	}
// FILE CONTENT

do
        {
	done=read(sock,line,200);
	len=len-done;
	if(done>0) fwrite(line,1,done,f);
        }
while(len>0);

fclose(f);
replyPostList(sock);
}


void processPOSTlist(int sock)
{
char line[200];
do		// READ (AND IGNORE) THE HEADER
        {
        readLineCRLF(sock,line);
        }
while(*line);
replyPostList(sock);
}


void replyPostList(int sock)
{
char *s1="<html><head><title>File List</title></head><body><h1>File List:</h1><big><ul>";
char *s2="</ul></big><hr><p><a href=/>BACK</a></body></html>";
char list[1000];
char line[200];
DIR *d;
struct dirent *e;
int len;

writeLineCRLF(sock,"HTTP/1.0 200 OK");
writeLineCRLF(sock,"Content-type: text/html");
d=opendir ("www/");
if(!d) replyPostError(sock,"Failed to open directory for listing");
*list=0;
do
	{
	e=readdir(d);  // IGNORE FILENAMES STARTED BY A DOT
	if(e && *e->d_name!='.') {strcat(list,"<li><a href=/");strcat(list,e->d_name);
		strcat(list,"><b>");strcat(list,e->d_name);strcat(list,"</b></a>");}
	}
while(e);
len=strlen(s1)+strlen(s2)+strlen(list);
sprintf(line,"Content-length: %i",len);
writeLineCRLF(sock,line);
writeLineCRLF(sock,"Connection: close");
writeLineCRLF(sock,"");
write(sock,s1,strlen(s1));
write(sock,list,strlen(list));
write(sock,s2,strlen(s2));
close(sock);exit(0);
}




void replyPostError(int sock, char *error)
{
char *s1="<html><head><title>Server Error</title></head><body><center><img src=500.png><br>(500.png)</center><h1>Server error on POST</h1><p>ERROR: ";
char *s2="<hr><p><a href=/>BACK</a></body></html>";
char line[200];
int len;
writeLineCRLF(sock,"HTTP/1.0 500 Internal Server Error");
writeLineCRLF(sock,"Content-type: text/html");
len=strlen(s1)+strlen(s2)+strlen(error);
sprintf(line,"Content-length: %i",len);
writeLineCRLF(sock,line);
writeLineCRLF(sock,"Connection: close");
writeLineCRLF(sock,"");
write(sock,s1,strlen(s1));
write(sock,error,strlen(error));
write(sock,s2,strlen(s2));
close(sock);
exit(1);
}
