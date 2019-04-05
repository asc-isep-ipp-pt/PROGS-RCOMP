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
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#define BUF_SIZE 500
#define DEFAULT_SERVER_PORT "443"

int main(int argc, char **argv) {
	int err, sock;
	char line[BUF_SIZE];
	struct addrinfo  req, *list;
	char *serverPort=DEFAULT_SERVER_PORT;
   
	if(argc<2) {
        	puts("HTTPS server's DNS name is required as argument");
        	exit(1);
        	}
	if(argc==3) serverPort=argv[2];
	bzero((char *)&req,sizeof(req));
	req.ai_family = AF_UNSPEC;
	req.ai_socktype = SOCK_STREAM;
	err=getaddrinfo(argv[1], serverPort , &req, &list);
	if(err) {
        	printf("Failed to get server address, error: %s\n",gai_strerror(err)); exit(1); }

	sock=socket(list->ai_family,list->ai_socktype,list->ai_protocol);
	if(sock==-1) {
        	perror("Failed to open socket"); freeaddrinfo(list); exit(1);}

	if(connect(sock,(struct sockaddr *)list->ai_addr, list->ai_addrlen)==-1) {
        	perror("Failed connect"); freeaddrinfo(list); close(sock); exit(1);}

	puts("----------------------\nConnected (TCP)");

	const SSL_METHOD *method=SSLv23_method();
        SSL_CTX *ctx = SSL_CTX_new(method);

	// ABORT ON HANDSHAKE IF CERTIFICATE UNTRUSTED
	// SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER,NULL);

	// LOAD TRUSTED ISSUERS CERTIFICATES LIST
	SSL_CTX_load_verify_locations(ctx,NULL,"/etc/ssl/certs");

	// Require HIGH, standing for 128-bits or above keys.
	// Disable some less safe cyphers
	// SSL_CTX_set_cipher_list(ctx,"HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4");

	// SOME SERVERS MAY NOT SUPPORT TLS1.2, and thus the handshake would fail
	// To support lower versions, comment this.
	// SSL_CTX_set_min_proto_version(ctx,TLS1_2_VERSION);

	SSL *sslConn = SSL_new(ctx);

        SSL_set_fd(sslConn, sock);
        if(SSL_connect(sslConn)!=1) {
		puts("SSL/TLS handshake error");
		SSL_free(sslConn); close(sock); exit(1);
	}
	puts("SSL/TLS handshake successful.");

	printf("Selected TLS version: %s\nSelected cypher suite: %s\n------------------------\n",
			SSL_get_cipher_version(sslConn),SSL_get_cipher(sslConn));

	X509* cert=SSL_get_peer_certificate(sslConn);
	X509_free(cert);

	if(cert==NULL) {
		puts("Sorry: no certificate was provided by the server, aborting.");
		SSL_free(sslConn); close(sock); exit(1);
	}

	X509_NAME_oneline(X509_get_subject_name(cert),line,BUF_SIZE);
	printf("Server's certificate subject: %s\n",line);

	char *cn=strstr(line,"/CN=");
	if(cn==NULL) {
		puts("SECURITY WARNING: certificate CN attribute not found");
	}
	else {
		cn+=4;  // remove any additional attribute
		char *nextAttr=strstr(cn,"/"); if(nextAttr) *nextAttr=0;
		if(strcmp(cn,argv[1])) {
			printf("SECURITY WARNING: the certificate CN attribute (%s) doesn't match the server's DNS name (%s).\n",
				       	cn, argv[1]);
		}
		else {
			printf("OK: the server's DNS name (%s) matches the certificate's CN attribute.\n", argv[1]);
		}
	}

	X509_NAME_oneline(X509_get_issuer_name(cert),line,BUF_SIZE);
	printf("\nServer's certificate issuer: %s\n---------------------------\n",line);

	long result=SSL_get_verify_result(sslConn);
	if(result!=X509_V_OK) {
		puts("SECURITY WARNING: untrusted server certificate");
		printf("The certificate's problem is: %s\n", X509_verify_cert_error_string(result));
	}
	else {
		puts("OK: The certificate is trusted.");
	}
	puts("-------------------------");

	
	SSL_free(sslConn);
	close(sock);
	exit(0);
	}

