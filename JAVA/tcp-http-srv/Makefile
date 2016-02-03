
P1=TcpHttpSrv
C2=HttpRequest

all: $(P1).class

$(P1).class: $(P1).java ${C2}.class
	javac $(P1).java

$(C2).class: $(C2).java
	javac $(C2).java

clean:
	rm -f *.class

