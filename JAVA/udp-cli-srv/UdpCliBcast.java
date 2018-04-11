import java.io.*;
import java.net.*;

class UdpCliBcast {
	static InetAddress targetIP;

	public static void main(String args[]) throws Exception {
		byte[] data = new byte[300];
		String frase;
		targetIP=InetAddress.getByName("255.255.255.255");

		DatagramSocket sock = new DatagramSocket();
		sock.setBroadcast(true);
		DatagramPacket udpPacket = new DatagramPacket(data, data.length, targetIP, 9999);

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while(true) {
        		System.out.print("Request sentence to send (\"exit\" to quit): ");
		        frase = in.readLine();
        		if(frase.compareTo("exit")==0) break;
	        	udpPacket.setData(frase.getBytes());
	        	udpPacket.setLength(frase.length());
		        sock.send(udpPacket);
	        	udpPacket.setData(data);
	        	udpPacket.setLength(data.length);
		        sock.receive(udpPacket);
        		frase = new String( udpPacket.getData(), 0, udpPacket.getLength());
			System.out.println("Received reply: " + frase);
        		}
		sock.close();
		}
	}


