import java.io.*; 
import java.net.*; 

class UdpBcastTest
{    
static InetAddress IPdestino;

public static void main(String args[]) throws Exception    
{       
if(args.length!=1) 
	{
	System.out.println("Deve indicar uma frase na linha de comando");
	System.exit(1);
	}

BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
DatagramSocket sock = new DatagramSocket(17766);
sock.setBroadcast(true);
sock.setSoTimeout(30000);
IPdestino=InetAddress.getByName("255.255.255.255");
byte[] data = new byte[300];
String frase;

while(true)
	{
	System.out.println("\n\nSending broadcast (CTRL+C para sair) ...");
	data = args[0].getBytes();
	DatagramPacket request = new DatagramPacket(data, args[0].length(), IPdestino, 17766);
	sock.send(request);
    while(true)
		{
		DatagramPacket reply = new DatagramPacket(data, data.length);
		try {
		sock.receive(reply);
		frase = new String( reply.getData(), 0, reply.getLength());
        System.out.println("BCAST from " + reply.getAddress().getHostAddress() + " : " + frase);
		}
		catch(SocketTimeoutException ex) {break;}
     
		}
		
	}

} 
}

