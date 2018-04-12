
C and JAVA versions of the sample programs for RCOMP (REDES DE COMPUTADORES) unit

------------------------------------------------------------------------------------
DISCLAIMER:  these code samples are intended for learning purpose, they were planned
for easy reading in the communication protocols perspective. They are not expected
to be efficient neither good samples of well-structured software. Error handling is
frequently ignored for the sake of easing the code analysis within the purposed 
objectives.
------------------------------------------------------------------------------------

Used application protocols are implementation independent, therefore different
implementations like C and Java can communicate with each other without any problem.

------------------------------------------------------------------------------------

		*** UDP client and server ***

C/udp-cli-srv/udp_srv.c

JAVA/udp-cli-srv/UdpSrv.java
				Supports both IPv4 and IPv6
				(1) Receives a UDP datagram and stores its content in a string.
				Reverse the string.
				Sends the reversed string inside a UDP datagram to the
				originator address/port.
				Repeat (1)

C/udp-cli-srv/udp_cli.c

JAVA/udp-cli-srv/UdpCli.java

				Get the server address as first argument in the command line.
				Server address can be either a DNS hostname, an IPv4 address
				or an IPv6 address.
				(1) Reads a string from keyboard, exits if the string is "exit",
				otherwise sends the string inside a UDP datagram.
				Receives a UDP datagram and prints it contents as a string.
				Repeat (1).



------------------------------------------------------------------------------------

		*** Reception timeout and UDP broadcast ***


C/udp-cli-srv/udp_cli_bcast.c

JAVA/udp-cli-srv/UdpCliBcast.java

				Same as previous client but no arguments because UDP
				first request is sent in broadcast (255.255.255.255).
				Operates only over IPv4 (in IPv6 there is no broadcast address).
				Only the first request is sent to the broadcast address,
				next requests are sent to the unicast address of the
				server that replies to the first request.

C/udp-cli-srv/udp_cli_to.c

JAVA/udp-cli-srv/UdpCliTo.java

				Same as the first UDP client but with timeout, if there is
				no reply from the server it does not hang forever as
				previous clients.


------------------------------------------------------------------------------------

		*** TCP client and server ***

C/tcp-cli-srv/tcp_srv_soma.c

JAVA/tcp-cli-srv/TcpSrvSoma.java

				Accepts TCP connections, either over IPv4 or IPv6,
				on each connection receives unsigned integer numbers
				in the form of 4 bytes, LSB (“Least Significant Byte”)
				first, until integer zero is received.
				When zero is received reply to client with the sum
				result in the same form (4 bytes).
				If first number received is zero closes the connection with
				the client.


JAVA/tcp-cli-srv/TcpCliSoma.java

C/tcp-cli-srv/tcp_cli_soma.c

				Get the server address as first argument in the command line.
				Server address can be either a DNS hostname, an IPv4 address
				or an IPv6 address.
				Establish a TCP connection to the server, read unsigned integers
				from keyboard and send then to the server in the form above specified.
				when the number is zero read the result from the server.
				If the first number entered is zero, send it to the server and exit.


------------------------------------------------------------------------------------

		*** Asynchronous reception, a TCP CHAT client, and server ***

C/udp-cli-srv/udp_srv_mport.c

JAVA/udp-cli-srv/UdpSrvMport.java

				UDP server compatible with previous UDP clients that
				is able to receive requests in a set of alternative
				port numbers. The C version makes use of the select()
				function while the Java version uses multiple threads.


C/tcp-chat/tcp_chat_srv.c

JAVA/tcp-chat/TcpChatSrv.java
				Accepts TCP connections, on all active connections
				receives strings as follows: 1st a byte with the
				number of characters in the string and then the
				string itself.
				For each string received retransmits it to every
				active connection, using the form specified above.
				If the string received is empty send it back to
				than client and closes that clients connection.

C/tcp-chat/tcp_chat_cli.c

JAVA/tcp-chat/TcpChatCli.java

				Get the server address as first argument in the command line.
				Server address can be either a DNS hostname, an IPv4 address
				or an IPv6 address.
				Connect to the server and read the nickname from the keyboard.
				If a string is entered by keyboard, send the nickname and
				the string to the server (in the form described above).
				If the string typed at keyboard is "exit", send an empty
				string to the server, wait for an empty reply and exit.
				If a string is received from the server (in the form
				described above) print it on the screen.

JAVA/tcp-chat/TcpChatCliGui.java

				GUI (swing) version of the above client.
				Server address is prompted in a dialog box.



JAVA/tcp-chat/TcpChatSrvSingleThread.java

				Single thread Java version of the TCP CHAT server, based
				on polling connected sockets with a low timeout read.




------------------------------------------------------------------------------------

		*** Demo HTTP servers ***

C/http-server-form-file-upload/http_srv_form_file_upload.c

JAVA/http-server-form-file-upload/HttpServerFormFileUpload.java

				Get the port number to use as first argument in the
				command line.
				Documents directory is sub-folder www/
				Reply to GET requests for some files types:
				(HTML,PDF,GIF,PNG,JavaSript).
				Reply to POST for /upload and /list:
				/upload to upload files with multipart/form-data
				/list to list the folder files



C/http-server-ajax-voting/http_srv_ajax_voting.c

(Java version: to-do)
				Get the port number to use as first argument in the
				command line.
				Documents directory is sub-folder www/
				Reply to GET requests for some files types:
				(HTML,PDF,GIF,PNG,JavaSript).
				Web services: GET /votes and PUT /votes/N (N=1..)



------------------------------------------------------------------------------------

		*** Peer-to-peer CHAT application ***

C/udp-chat/udp_chat.c

JAVA/udp-chat/UdpChat.java
				Gets a nickname from the user. 
				Manages a list of active peers. At startup one UDP
				datagram is sent to the broadcast address to announce
				itself, this is the peer start announcement.
				Also, when a new peer start announcement is received a
				peer start announcement is sent back. Before exiting a
				peer exit announcement is sent to each active peer.
				Other received UDP datagrams that are not a peer start
				announcements neither peer exit announcements are supposed
				to be text messages and are printed at the console.
				When the user enters a text line at the console, it is
				sent to each active peer in active peer list.
				If the typed line is EXIT the peer application exits,
				if it is LIST, then a list of active peers is printed.


------------------------------------------------------------------------------------
ANDRE MOREIRA
asc@isep.ipp.pt
https://www.dei.isep.ipp.pt/~andre
------------------------------------------------------------------------------------
Departamento de Engenharia Informática (DEI)
https://www.dei.isep.ipp.pt/
------------------------------------------------------------------------------------
Instituto Superior de Engenharia do Porto (ISEP)
https://www.isep.ipp.pt/
------------------------------------------------------------------------------------
Instituto Politécnico do Porto (IPP)
https://www.ipp.pt/
------------------------------------------------------------------------------------
Rua Dr. António Bernardino de Almeida, 431
4249-015 Porto
PORTUGAL
------------------------------------------------------------------------------------

