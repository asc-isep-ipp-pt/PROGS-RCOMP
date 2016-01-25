import java.io.*; 
import java.net.*; 

class UdpCliBcast
{    
static InetAddress IPdestino;

public static void main(String args[]) throws Exception    
{       

BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
DatagramSocket sock = new DatagramSocket();
sock.setBroadcast(true);
IPdestino=InetAddress.getByName("255.255.255.255");
byte[] data = new byte[300];
String frase;
while(true)
	{
	System.out.print("Frase a enviar (\"sair\" para terminar): ");
	frase = in.readLine();
	if(frase.compareTo("sair")==0) break;
	data = frase.getBytes();
	DatagramPacket request = new DatagramPacket(data, frase.length(), IPdestino, 9999);
	sock.send(request);
	DatagramPacket reply = new DatagramPacket(data, data.length);
        sock.receive(reply);
	IPdestino=reply.getAddress();
	frase = new String( reply.getData(), 0, reply.getLength());
        System.out.println("Resposta: " + frase);
	}
sock.close();
} 
}

