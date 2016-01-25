
P1=UdpCliBcast
P2=UdpCli
P3=UdpCliTo
P4=UdpSrv
P5=UdpSrvMport

all: $(P1).class $(P2).class $(P3).class $(P3).class $(P4).class $(P5).class

$(P1).class: $(P1).java
	javac $(P1).java

$(P2).class: $(P2).java
	javac $(P2).java

$(P3).class: $(P3).java
	javac $(P3).java

$(P4).class: $(P4).java
	javac $(P4).java

$(P5).class: $(P5).java
	javac $(P5).java

clean:
	rm -f *.class

