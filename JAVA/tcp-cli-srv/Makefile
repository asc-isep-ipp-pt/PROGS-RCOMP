
P1=TcpCliSum
P2=TcpSrvSum

all: $(P1).class $(P2).class

$(P1).class: $(P1).java
	javac $(P1).java

$(P2).class: $(P2).java
	javac $(P2).java

clean:
	rm -f *.class

