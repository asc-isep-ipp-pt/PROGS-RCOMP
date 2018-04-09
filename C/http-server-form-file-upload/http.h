#ifndef __RCOMP_HTTP_H
#define __RCOMP_HTTP_H

#define HTTP_VERSION "HTTP/1.0"

#define HTTP_CONNECTION_CLOSE "Connection: close"

void readLineCRLF(int sock, char *line);
void writeLineCRLF(int sock, char *line);

void sendHttpResponseHeader(int sock, char *status, char *contentType, int contentLength);
int sendHttpResponse(int sock, char *status, char *contentType, char *content, int contentLength);
void sendHttpStringResponse(int sock, char *status, char *contentType, char *content);
void sendHttpFileResponse(int sock, char *status, char *filename);


#endif
