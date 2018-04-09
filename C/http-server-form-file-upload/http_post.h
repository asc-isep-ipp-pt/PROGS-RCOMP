#ifndef __RCOMP_HTTP_POST_H
#define __RCOMP_HTTP_POST_H

void processPOST(int sock, char *request, char *baseFolder);

void processPOSTupload(int sock, char *baseFolder);
void processPOSTlist(int sock, char *baseFolder);

void replyPostList(int sock, char *baseFolder);
void replyPostError(int sock, char *error);
#endif

