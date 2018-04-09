
P1=HttpServerFormFileUpload
C2=HTTP
C3=HttpRequest

all: $(P1).class 

$(P1).class: $(P1).java $(C2).class $(C3).class
	javac $(P1).java

$(C2).class: $(C2).java
	javac $(C2).java

$(C3).class: $(C3).java
	javac $(C3).java

clean:
	rm -f *.class

