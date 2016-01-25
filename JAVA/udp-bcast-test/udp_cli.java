import java.io.*; 
import java.net.*; 

class udp_cli
{    
static InetAddress IPdestino;

public static void main(String args[]) throws Exception    
{       
if(args.length!=1) 
	{
	System.out.println("Deve indicar o host de destino na linha de comando");
	System.exit(1);
	}

try { IPdestino = InetAddress.getByName(args[0]); }
catch(UnknownHostException ex) {
	System.out.println("O nome de host de destino fornecido (" + args[0] + ") nao foi resolvido");
	System.exit(1);
	} 

BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
DatagramSocket sock = new DatagramSocket();
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
	frase = new String( reply.getData(), 0, reply.getLength());
        System.out.println("Resposta: " + frase);
	}
sock.close();
} 
}

