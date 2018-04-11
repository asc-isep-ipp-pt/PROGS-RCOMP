import java.io.*; 
import java.net.*; 
import java.util.HashMap;

class TcpChatSrvSingleThread {    

	private static final int SO_TIMEOUT = 100;

	private static HashMap<Socket,DataOutputStream> cliListOut = new HashMap<>();
	private static HashMap<Socket,DataInputStream> cliListIn = new HashMap<>();


	private static ServerSocket sock;

	public static void main(String args[]) throws Exception {       
		int nChars;
		byte[] data = new byte[300];
		Socket cliS;

		try { sock = new ServerSocket(9999); }
		catch(IOException ex) {
			System.out.println("Local port number not available.");
			System.exit(1); }

		sock.setSoTimeout(SO_TIMEOUT); // set the socket timeout

		while(true) {
			try { 	// check for new client connection requests
				cliS=sock.accept();
				cliS.setSoTimeout(SO_TIMEOUT); // set the connected socket timeout
				cliListOut.put(cliS,new DataOutputStream(cliS.getOutputStream()));
				cliListIn.put(cliS,new DataInputStream(cliS.getInputStream()));
				}
			catch(SocketTimeoutException to) {} // no new connections


			for(Socket s: cliListIn.keySet()) {   // all connected clients
				DataInputStream sIn = cliListIn.get(s);
   				try {				// try reading the line size
        				nChars=sIn.read();
   					if(nChars==0) { 	// empty line - client wants to exit
						DataOutputStream sOut = cliListOut.get(s);
						sOut.write(nChars);  // send back an empty line
						cliListIn.remove(s);
						cliListOut.remove(s);
						s.close();
						} 
					else {
						sIn.read(data,0,nChars);  // read the line
						for(DataOutputStream sOut: cliListOut.values()) {
							sOut.write(nChars);
                					sOut.write(data,0,nChars);
							}
						}
					}
				catch(SocketTimeoutException to) {} // no text line from client
			   	}

			}
		}
	}



