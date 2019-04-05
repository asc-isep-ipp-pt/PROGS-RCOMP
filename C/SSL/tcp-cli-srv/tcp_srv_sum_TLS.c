#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUF_SIZE 300
#define SERVER_PORT "9999"

#define SERVER_SSL_CERT_FILE "server.pem"
#define SERVER_SSL_KEY_FILE "server.key"
#define AUTH_CLIENTS_SSL_CERTS_FILE "authentic-clients.pem"


int main(void) {
	struct sockaddr_storage from;
	int err, newSock, sock;
	unsigned int adl;
	unsigned long i, f, n, num, sum;
	unsigned char bt;
	char cliIPtext[BUF_SIZE], cliPortText[BUF_SIZE];
	struct addrinfo  req, *list;

	bzero((char *)&req,sizeof(req));
	// requesting a IPv6 local address will allow both IPv4 and IPv6 clients to use it
	req.ai_family = AF_INET6;
	req.ai_socktype = SOCK_STREAM;
	req.ai_flags = AI_PASSIVE;      // local address

	err=getaddrinfo(NULL, SERVER_PORT , &req, &list);

	if(err) {
        	printf("Failed to get local address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(bind(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Bind failed");close(sock);freeaddrinfo(list);exit(1);}

	freeaddrinfo(list);

	listen(sock,SOMAXCONN);

	const SSL_METHOD *method;
        SSL_CTX *ctx;

	method = SSLv23_server_method();
        ctx = SSL_CTX_new(method);

	// Load the server's certificate and key
        SSL_CTX_use_certificate_file(ctx, SERVER_SSL_CERT_FILE, SSL_FILETYPE_PEM);
        SSL_CTX_use_PrivateKey_file(ctx, SERVER_SSL_KEY_FILE, SSL_FILETYPE_PEM);
        if (!SSL_CTX_check_private_key(ctx)) {
                puts("Error loading server's certificate/key");
		close(sock);
                exit(1);
        }

	// THE CLIENTS' CERTIFICATES ARE TRUSTED
        SSL_CTX_load_verify_locations(ctx,AUTH_CLIENTS_SSL_CERTS_FILE,NULL);

	// Restrict TLS version and cypher suite
        SSL_CTX_set_min_proto_version(ctx,TLS1_2_VERSION);
        SSL_CTX_set_cipher_list(ctx, "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4");

	// The client must provide a certificate and it must be trusted, the handshake will fail otherwise
	SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);

	puts("Accepting TCP connections (both over IPv6 or IPv4). Use CTRL+C to terminate the server");

	adl=sizeof(from);
	for(;;) {
        	newSock=accept(sock,(struct sockaddr *)&from,&adl);
        	if(!fork()) {
                	close(sock);
			getnameinfo((struct sockaddr *)&from,adl,cliIPtext,BUF_SIZE,cliPortText,BUF_SIZE,
				NI_NUMERICHOST|NI_NUMERICSERV);
                	printf("New connection from node %s, port number %s\n", cliIPtext, cliPortText);
			SSL *sslConn = SSL_new(ctx);
        		SSL_set_fd(sslConn, newSock);
			if(SSL_accept(sslConn)!=1) {
				puts("TLS handshake error: client not authorized");
				SSL_free(sslConn);
                		close(newSock);
				exit(1);
				}
        		printf("TLS version: %s\nCypher suite: %s\n",
					SSL_get_cipher_version(sslConn),SSL_get_cipher(sslConn));
			X509* cert=SSL_get_peer_certificate(sslConn);
        		X509_free(cert);
			X509_NAME_oneline(X509_get_subject_name(cert),cliIPtext,BUF_SIZE);
        		printf("Client's certificate subject: %s\n",cliIPtext);



                	do {
                    		sum=0;
                    		do {
		       			num=0;f=1;
					for(i=0;i<4;i++) {
						SSL_read(sslConn,&bt,1); num=num+bt*f; f=256*f;
						}
                       			sum=sum+num;
                       			}
                    		while(num);
		    		n=sum;
		    		for(i=0;i<4;i++) {
                        		bt=n%256; SSL_write(sslConn,&bt,1); n=n/256; }
                    		} 
                	while(sum);
                	SSL_free(sslConn);
                	close(newSock);
                	printf("Connection from node %s, port number %s closed\n", cliIPtext, cliPortText);
                	exit(0);
                	}
        	close(newSock);
        	}
	close(sock);
	}

