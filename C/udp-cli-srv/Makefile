
P1=udp_cli
P2=udp_srv
P3=udp_srv_mport
P4=udp_cli_bcast
P5=udp_cli_to
P6=udp_cli_to_sel

all: $(P1) $(P2) $(P3) $(P4) $(P5) $(P6)

$(P1): $(P1).c
	$(CC) -o $(P1) $(P1).c

$(P2): $(P2).c
	$(CC) -o $(P2) $(P2).c

$(P3): $(P3).c
	$(CC) -o $(P3) $(P3).c

$(P4): $(P4).c
	$(CC) -o $(P4) $(P4).c

$(P5): $(P5).c
	$(CC) -o $(P5) $(P5).c

$(P6): $(P6).c
	$(CC) -o $(P6) $(P6).c


clean:
	rm -f $(P1) $(P2) $(P3) $(P4) $(P5) $(P6)

