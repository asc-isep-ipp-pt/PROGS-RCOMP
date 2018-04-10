import java.io.*; 
import java.net.*; 
import java.util.concurrent.*;
import java.util.HashMap;

class TcpChatSrv {

	private static HashMap<Socket,DataOutputStream> cliList = new HashMap<>();

	public static synchronized void sendToAll(int len, byte[] data) throws Exception {
		for(DataOutputStream cOut: cliList.values()) {
			cOut.write(len);
			cOut.write(data,0,len);
			}

		}
	
	public static synchronized void addCli(Socket s) throws Exception {
		cliList.put(s,new DataOutputStream(s.getOutputStream()));
		}

	public static synchronized void remCli(Socket s) throws Exception {
		cliList.get(s).write(0);
		cliList.remove(s);
		s.close();
		}



	private static ServerSocket sock;

	public static void main(String args[]) throws Exception {
		int i;

		try { sock = new ServerSocket(9999); }
		catch(IOException ex) {
			System.out.println("Local port number not available.");
			System.exit(1); }

		while(true) {
			Socket s=sock.accept(); // wait for a new client connection request
			addCli(s);
			Thread cli = new TcpChatSrvClient(s);
			cli.start();
			}
		} 
	}



class TcpChatSrvClient extends Thread {
	private Socket myS;
	private DataInputStream sIn;

	public TcpChatSrvClient(Socket s) { myS=s;}
	
	public void run() {
		int nChars;
		byte[] data = new byte[300];

		try {
			sIn = new DataInputStream(myS.getInputStream());
			while(true) {
   				nChars=sIn.read();
   				if(nChars==0) break; // empty line means client wants to exit
   				sIn.read(data,0,nChars);
				TcpChatSrv.sendToAll(nChars,data);
   				}
   			// the client wants to exit
   			TcpChatSrv.remCli(myS);
			}
  		catch(Exception ex) { System.out.println("Error"); }
		}
	}



