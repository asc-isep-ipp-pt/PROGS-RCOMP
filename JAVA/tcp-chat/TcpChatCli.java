import java.io.*; 
import java.net.*; 

class TcpChatCli
{    
static InetAddress IPdestino;
static Socket sock;


public static void main(String args[]) throws Exception
{
String nick, frase;
byte[] data = new byte[300];

if(args.length!=1) {
     System.out.println("Server IPv4/IPv6 address or DNS name is required as argument");
     System.exit(1); }

try { IPdestino = InetAddress.getByName(args[0]); }
catch(UnknownHostException ex) {
   System.out.println("Invalid server: " + args[0]);
    System.exit(1); }

try { sock = new Socket(IPdestino, 9999); }
catch(IOException ex) {
   System.out.println("Failed to connect.");
   System.exit(1); }

BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
DataOutputStream sOut = new DataOutputStream(sock.getOutputStream());

System.out.print("Nickname: "); nick = in.readLine();

Thread serverConn = new Thread(new TcpChatCliConn(sock));
serverConn.start();


while(true)
	{
	frase=in.readLine();
	if(frase.compareTo("exit")==0) 
		{ sOut.write(0); break;}
	frase="(" + nick + ") " + frase;
	data = frase.getBytes();
	sOut.write((byte)frase.length());
	sOut.write(data,0,(byte)frase.length());
	}

serverConn.join();
sock.close();
} 
}



class TcpChatCliConn implements Runnable 
{    
private Socket s;
private DataInputStream sIn;

public TcpChatCliConn(Socket tcp_s) { s=tcp_s;}

public void run()
{
int nChars;
byte[] data = new byte[300];
String frase;

try {
sIn = new DataInputStream(s.getInputStream());
while(true)
   {
   nChars=sIn.read();
   if(nChars==0) break;
   sIn.read(data,0,nChars);
   frase = new String( data, 0, nChars);
   System.out.println(frase);
   }
} catch(IOException ex) { System.out.println("Client disconnected."); }
}
}
