import java.io.*; 
import java.net.*; 

class UdpSrvMport {    
	private static final int SERVER_PORT_BASE=9009;
	static DatagramSocket sock[];
	public static void main(String args[]) throws Exception {
		int i;
		sock = new DatagramSocket[6];

		for(i=0;i<6;i++) {
			try { sock[i] = new DatagramSocket(SERVER_PORT_BASE+100*i); }
			catch(BindException ex) {
                		System.out.println("Failed to bind to port number " + SERVER_PORT_BASE+100*i);
				do { sock[i].close(); i--; } while(i>-1);
                		System.exit(1);
                		}
			}


		System.out.println("Listening for UDP requests (IPv6/IPv4). Use CTRL+C to terminate the server");

		for(i=0;i<6;i++)  // start one thread for each socket
  			new Thread(new UdpSrvMportThread(sock[i])).start();

		}
	}


class UdpSrvMportThread implements Runnable {
	private DatagramSocket sock;


	public UdpSrvMportThread(DatagramSocket s) { sock=s;}

	public void run() {
		byte[] data = new byte[300];
		byte[] data1 = new byte[300];
		String frase;
		int len, i;

		DatagramPacket udpPacket = new DatagramPacket(data, data.length);

		try {
			while(true) {
				udpPacket.setData(data);
			        udpPacket.setLength(data.length);
			        sock.receive(udpPacket);
			        len=udpPacket.getLength();
			        System.out.println("Request from: " + udpPacket.getAddress().getHostAddress() +
			        	" port: " + udpPacket.getPort());
			        for(i=0;i<len;i++) data1[len-1-i]=data[i];
			        udpPacket.setData(data1);
			        udpPacket.setLength(len);
			        sock.send(udpPacket);
				}
   			}
		catch(IOException ex) { System.out.println("IOException"); }
		}
	}

