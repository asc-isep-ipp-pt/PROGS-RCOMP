
P1=TcpCliSumTLS
P2=TcpSrvSumTLS

all: $(P1).class $(P2).class

$(P1).class: $(P1).java
	javac $(P1).java

$(P2).class: $(P2).java
	javac $(P2).java

certs:
	./make_certs

clean:
	rm -f *.class *.crt *.key *.pem *.jks


