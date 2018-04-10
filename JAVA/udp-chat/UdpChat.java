import java.io.*; 
import java.net.*; 
import java.util.HashSet;

class UdpChat {    

	private static final String BCAST_ADDR = "255.255.255.255";
	private static final int SERVICE_PORT = 9999;

	private static HashSet<InetAddress> peersList = new HashSet<>();

	public static synchronized void addIP(InetAddress ip) { peersList.add(ip);}

	public static synchronized void remIP(InetAddress ip) { peersList.remove(ip);}

	public static synchronized void printIPs() { 
		for(InetAddress ip: peersList) {
			System.out.print(" " + ip.getHostAddress());
			}
		}

	public static synchronized void sendToAll(DatagramSocket s, DatagramPacket p) throws Exception { 
		for(InetAddress ip: peersList) {
			p.setAddress(ip);
			s.send(p);
			}
		}

	static InetAddress bcastAddress;
	static DatagramSocket sock;

	public static void main(String args[]) throws Exception {
 		String nick, frase;
 		byte[] data = new byte[300];
 		byte[] fraseData;
 		int i;
		DatagramPacket udpPacket;

		try { sock = new DatagramSocket(SERVICE_PORT); }
		catch(IOException ex) {
			System.out.println("Failed to open local port");
			System.exit(1); }

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		System.out.print("Nickname: "); nick = in.readLine();

		bcastAddress=InetAddress.getByName(BCAST_ADDR);
		sock.setBroadcast(true);
		data[0]=1;
		udpPacket = new DatagramPacket(data, 1, bcastAddress, SERVICE_PORT);
		sock.send(udpPacket);

		Thread udpReceiver = new Thread(new UdpChatReceive(sock));
		udpReceiver.start();

		while(true) { // handle user inputs
  			frase=in.readLine();
			if(frase.compareTo("EXIT")==0) break;
			if(frase.compareTo("LIST")==0) {
				System.out.print("Active peers:");
				printIPs();
				System.out.println("");
				}
  			else {
				frase="(" + nick + ") " + frase;
				fraseData = frase.getBytes();
				udpPacket.setData(fraseData);
				udpPacket.setLength(frase.length());
				sendToAll(sock,udpPacket);
				}
  			}
		data[0]=0; // announce I'm leaving
		udpPacket.setData(data);
		udpPacket.setLength(1);
		sendToAll(sock,udpPacket);
		sock.close();
		udpReceiver.join(); // wait for the thread to end
		} 
	}



class UdpChatReceive implements Runnable {    
	private DatagramSocket s;

	public UdpChatReceive(DatagramSocket udp_s) { s=udp_s;}

	public void run() {
		int i;
		byte[] data = new byte[300];
		String frase;
		DatagramPacket p;
		InetAddress currPeerAddress;

		p=new DatagramPacket(data, data.length);

		while(true) {
   			p.setLength(data.length);
   			try { s.receive(p); }
			catch(IOException ex) { return; }
   			currPeerAddress=p.getAddress();
			
   			if(data[0]==1) { // peer start
				UdpChat.addIP(p.getAddress());
				try { s.send(p); } 
				catch(IOException ex) { return; }
				}
   			else
   			if(data[0]==0) { // peer exit
				UdpChat.remIP(p.getAddress());
				}
   			else {		// chat message
        			frase = new String( p.getData(), 0, p.getLength());
   				System.out.println(frase);
				}
   			}
 		}
	}

