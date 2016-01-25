
P1=tcp_chat_cli
P2=tcp_chat_srv

all: $(P1) $(P2)

$(P1): $(P1).c
	$(CC) -o $(P1) $(P1).c

$(P2): $(P2).c
	$(CC) -o $(P2) $(P2).c


clean:
	rm -f $(P1) $(P2)

