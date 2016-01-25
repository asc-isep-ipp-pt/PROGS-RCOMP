import java.io.*; 
import java.net.*; 

class udp_snd
{    
	static InetAddress IPdestino;

	public static void main(String args[]) throws Exception    
	{       
	BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
	String frase;
	System.out.print("Indicar o host de destino (IP ou nome): ");
	frase = in.readLine();
	try { IPdestino = InetAddress.getByName(frase); }
	catch(UnknownHostException ex) {
		System.out.println("O nome de host de destino fornecido (" + frase + ") nao e valido");
		System.exit(1);
		} 

	DatagramSocket sock = new DatagramSocket();
	byte[] data = new byte[300];
	do
		{
		System.out.print("Frase a enviar (\"sair\" para terminar): ");
		frase = in.readLine();
		data = frase.getBytes();
		DatagramPacket packet = new DatagramPacket(data, frase.length(), IPdestino, 9999);
		sock.send(packet);
		}
	while(frase.compareTo("sair")!=0);
	sock.close();
	} 
}

