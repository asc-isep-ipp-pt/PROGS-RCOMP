import java.io.*; 
import java.net.*; 

class tcp_srv_soma
{    
static InetAddress IPdestino;
static ServerSocket sock;

public static void main(String args[]) throws Exception    
{       
Socket cliSock;

try { sock = new ServerSocket(9999); }
catch(IOException ex) {
	System.out.println("O porto local esta ocupado.");
	System.exit(1); }

while(true)
	{
	cliSock=sock.accept();
	new Thread(new tcp_srv_soma_thread(cliSock)).start();
	}
} 
}



class tcp_srv_soma_thread implements Runnable 
{    
private Socket s;
private DataOutputStream sOut;
private DataInputStream sIn;

public tcp_srv_soma_thread(Socket cli_s) { s=cli_s;}

public void run()
{
long f,i,num,soma;
try {
sOut = new DataOutputStream(s.getOutputStream());
sIn = new DataInputStream(s.getInputStream());
do
   {
   soma=0;
   do
	{
   	num=0; f=1; for(i=0;i<4;i++) {num=num+f*sIn.read(); f=f*256;}
	soma=soma+num;
	}
   while(num>0);
   num=soma; for(i=0;i<4;i++) {sOut.write((byte)(num%256)); num=num/256; }
   }
while(soma>0);

s.close();
} catch(IOException ex) { System.out.println("IOException"); }
}
}
