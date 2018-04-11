import java.io.*; 
import java.net.*; 

class UdpCliTo {    
	static InetAddress serverIP;

	private static final int TIMEOUT=3; // seconds

	public static void main(String args[]) throws Exception {       
		byte[] data = new byte[300];
		String frase;

		if(args.length!=1) {
			System.out.println("Server IPv4/IPv6 address or DNS name is required as argument");
			System.exit(1);
			}

		try { serverIP = InetAddress.getByName(args[0]); }
		catch(UnknownHostException ex) {
			System.out.println("Invalid server address supplied: "  + args[0]);
			System.exit(1);
			} 

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		DatagramSocket sock = new DatagramSocket();
		sock.setSoTimeout(1000*TIMEOUT); // set the socket timeout

		DatagramPacket udpPacket = new DatagramPacket(data, data.length, serverIP, 9999);

		while(true) {
			System.out.print("Request sentence to send (\"exit\" to quit): ");
			frase = in.readLine();
			if(frase.compareTo("exit")==0) break;
			udpPacket.setData(frase.getBytes());
		        udpPacket.setLength(frase.length());
		        sock.send(udpPacket);
		        udpPacket.setData(data);
		        udpPacket.setLength(data.length);
			try {
        			sock.receive(udpPacket);
        			frase = new String( udpPacket.getData(), 0, udpPacket.getLength());
        			System.out.println("Received reply: " + frase);
				}
			catch(SocketTimeoutException ex) {
                		System.out.println("No reply from server");}
			}
		sock.close();
		} 
	}





