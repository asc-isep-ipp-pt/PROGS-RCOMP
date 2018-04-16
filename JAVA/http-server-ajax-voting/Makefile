
P1=HttpServerAjaxVoting
C2=HTTPmessage
C3=HttpAjaxVotingRequest
P2=DemoConsumer

all: $(P1).class $(P2).class

$(P1).class: $(P1).java $(C2).class $(C3).class
	javac $(P1).java

$(P2).class: $(P2).java $(C2).class
	javac $(P2).java

$(C2).class: $(C2).java
	javac $(C2).java

$(C3).class: $(C3).java
	javac $(C3).java

clean:
	rm -f *.class

