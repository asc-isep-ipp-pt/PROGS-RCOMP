
P1=HttpsServerAjaxVoting
C2=HTTPmessage
C3=HttpAjaxVotingRequest

all: $(P1).class

$(P1).class: $(P1).java $(C2).class $(C3).class
	javac $(P1).java

$(C2).class: $(C2).java
	javac $(C2).java

$(C3).class: $(C3).java
	javac $(C3).java
cert:
	./make_cert

clean:
	rm -f *.class *.jks

