import java.io.*; 
import java.net.*; 

class udp_srv
{    
static DatagramSocket sock;
public static void main(String args[]) throws Exception       
{          
byte[] data = new byte[300];
byte[] data1 = new byte[300];
String frase;
InetAddress clientIP;
int res, i, clientPort;

try { sock = new DatagramSocket(9999); }
catch(BindException ex)
                {
                System.out.println("O porto esta ocupado");
                System.exit(1);
                }

System.out.println("A escutar pedidos. Use CTRL+C para terminar");
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
}

