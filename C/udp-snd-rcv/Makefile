
P1=udp_snd
P2=udp_rcv
P3=udp_rcv1

all: $(P1) $(P2) $(P3)

$(P1): $(P1).c
	$(CC) -o $(P1) $(P1).c

$(P2): $(P2).c
	$(CC) -o $(P2) $(P2).c

$(P3): $(P3).c
	$(CC) -o $(P3) $(P3).c

clean:
	rm -f $(P1) $(P2) $(P3)

