
P1=TcpChatCli
P2=TcpChatCliGui
P3=TcpChatSrv
P4=TcpChatSrvSingleThread

all: $(P1).class $(P2).class $(P3).class $(P4).class

$(P1).class: $(P1).java
	javac $(P1).java

$(P2).class: $(P2).java
	javac $(P2).java

$(P3).class: $(P3).java
	javac $(P3).java

$(P4).class: $(P4).java
	javac $(P4).java


clean:
	rm -f *.class

