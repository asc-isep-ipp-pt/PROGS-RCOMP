import java.io.*; 
import java.net.*; 

class UdpRcv1
{    
public static void main(String args[]) throws Exception       
	{          
	DatagramSocket sock = new DatagramSocket(9999);             
	byte[] data = new byte[300];
	String frase;
	InetAddress IPorigem;
             do               
		{
		DatagramPacket packet = new DatagramPacket(data, data.length);
		sock.receive(packet); IPorigem = packet.getAddress();
		frase = new String( packet.getData(), 0, packet.getLength());
		System.out.println("Recebido: " + frase);
		System.out.println("IP origem = " + IPorigem.getHostAddress() + 
			" ; Porto origem = " + packet.getPort());
		}
	while(frase.compareTo("sair")!=0);

	sock.close();
	}
}

