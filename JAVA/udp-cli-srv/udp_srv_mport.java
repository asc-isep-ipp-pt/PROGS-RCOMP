import java.io.*; 
import java.net.*; 

class udp_srv_mport
{    
private static final int PORT_BASE=9009;
static DatagramSocket sock[];
public static void main(String args[]) throws Exception       
{          
int i;
sock = new DatagramSocket[6];

for(i=0;i<6;i++)
	{
	try { sock[i] = new DatagramSocket(PORT_BASE+100*i); }
	catch(BindException ex)
                {
                System.out.println("O porto " + PORT_BASE+100*i + "esta ocupado");
		do { sock[i].close(); i--; } while(i>-1);
                System.exit(1);
                }
	}


System.out.println("A escutar pedidos. Use CTRL+C para terminar");

for(i=0;i<6;i++)
  new Thread(new udp_srv_mport_thread(sock[i])).start();

}
}


class udp_srv_mport_thread implements Runnable
{
private DatagramSocket sock;


public udp_srv_mport_thread(DatagramSocket s) { sock=s;}

public void run()
{
byte[] data = new byte[300];
byte[] data1 = new byte[300];
String frase;
InetAddress clientIP;
int res, i, clientPort;
try {
 while(true)                
	{
	DatagramPacket pedido= new DatagramPacket(data, data.length);
	sock.receive(pedido);
	clientIP=pedido.getAddress();
	clientPort=pedido.getPort();
	res=pedido.getLength();
	for(i=0;i<res;i++) data1[res-1-i]=data[i];
	DatagramPacket resposta=  new DatagramPacket(data1, res, clientIP, clientPort);
	sock.send(resposta);
	}
   }
catch(IOException ex) { System.out.println("IOException"); }
}
}

