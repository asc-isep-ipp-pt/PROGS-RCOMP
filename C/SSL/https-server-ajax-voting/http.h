#ifndef __RCOMP_HTTP_H
#define __RCOMP_HTTP_H

#define HTTP_VERSION "HTTP/1.0"

#define HTTP_CONNECTION_CLOSE "Connection: close"

void readLineCRLF(SSL *sslConn, char *line);
void writeLineCRLF(SSL *sslConn, char *line);

void sendHttpResponseHeader(SSL *sslConn, char *status, char *contentType, int contentLength);
int sendHttpResponse(SSL *sslConn, char *status, char *contentType, char *content, int contentLength);
void sendHttpStringResponse(SSL *sslConn, char *status, char *contentType, char *content);
void sendHttpFileResponse(SSL *sslConn, char *status, char *filename);


#endif
