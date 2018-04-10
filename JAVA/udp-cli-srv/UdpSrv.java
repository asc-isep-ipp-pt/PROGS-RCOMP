import java.io.*;
import java.net.*;

class UdpSrv {
	static DatagramSocket sock;
	public static void main(String args[]) throws Exception {
		byte[] data = new byte[300];
		byte[] data1 = new byte[300];
		int i, len;

		try { sock = new DatagramSocket(9999); }
		catch(BindException ex) {
                	System.out.println("Bind to local port failed");
                	System.exit(1);
                	}

		DatagramPacket udpPacket= new DatagramPacket(data, data.length);

		System.out.println("Listening for UDP requests (IPv6/IPv4). Use CTRL+C to terminate the server");
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
	}


