import java.io.*; 
import java.net.*; 

class udp_rcv
{    
static DatagramSocket sock;
public static void main(String args[]) throws Exception       
	{          
	try { sock = new DatagramSocket(9999); }
	catch(BindException ex)
		{
		System.out.println("O porto esta ocupado");
                System.exit(1);
                }

	byte[] data = new byte[300];
	String frase;
             do               
		{
		DatagramPacket packet = new DatagramPacket(data, data.length);
		sock.receive(packet);
		frase = new String( packet.getData(), 0, packet.getLength());
		System.out.println("Recebido: " + frase);
		}
	while(frase.compareTo("sair")!=0);

	sock.close();
	}
}

