import java.io.*; 
import java.net.*; 

class TcpHttpSrv
{    
static InetAddress IPdestino;
static ServerSocket sock;

public static void main(String args[]) throws Exception    
 {       
 Socket cliSock;

 if(args.length!=1) {
        System.out.println("Deve indicar o porto local que o servidor deve usar");
        System.exit(1); }

 try { sock = new ServerSocket(Integer.parseInt(args[0])); }
 catch(IOException ex) {
	System.out.println("O porto local " + args[0] + " esta ocupado.");
	System.exit(1); }

 while(true)
	{
	cliSock=sock.accept();
	new Thread(new HttpRequest(cliSock)).start();
	}
 } 
}



