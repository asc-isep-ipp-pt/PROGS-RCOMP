import java.io.*; 
import java.net.*; 
import java.util.concurrent.*;

class UdpChat
{    
public static int MAXCLI=100;
public static Boolean[] peerActive= new Boolean[MAXCLI];
public static InetAddress[] peerAddress= new InetAddress[MAXCLI];

public static Semaphore changeLock = new Semaphore(1);

static InetAddress bcastAddress;
static DatagramSocket sock;

public static void main(String args[]) throws Exception {
 String nick, frase;
 byte[] data = new byte[300];
 byte[] fraseData;
 int i;
 DatagramPacket udpPacket;

 try { sock = new DatagramSocket(9999); }
 catch(IOException ex) {
   System.out.println("Failed to open local port");
   System.exit(1); }

 BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

 System.out.print("Nickname: "); nick = in.readLine();

 for(i=0; i<MAXCLI; i++) peerActive[i]=false;

 bcastAddress=InetAddress.getByName("255.255.255.255");
 sock.setBroadcast(true);
 data[0]=1;
 udpPacket = new DatagramPacket(data, 1, bcastAddress, 9999);
 sock.send(udpPacket);

 Thread udpReceiver = new Thread(new UdpChatReceive(sock));
 udpReceiver.start();

 while(true)
  {
  frase=in.readLine();
  if(frase.compareTo("EXIT")==0) break;
  if(frase.compareTo("LIST")==0)
	{
	System.out.print("Active peers:");
	changeLock.acquire();
	for(i=0; i<MAXCLI; i++) 
		if(peerActive[i]) System.out.print(" " + peerAddress[i].getHostAddress());
	changeLock.release();
	System.out.println("");
	}
  else
	{
	frase="(" + nick + ") " + frase;
	fraseData = frase.getBytes();
	udpPacket.setData(fraseData);
	udpPacket.setLength(frase.length());
	changeLock.acquire();
	for(i=0; i<MAXCLI; i++) 
		if(peerActive[i])
			{
			udpPacket.setAddress(peerAddress[i]);
			sock.send(udpPacket);
			}
	changeLock.release();
	}
  }

 data[0]=0;
 udpPacket.setData(data);
 udpPacket.setLength(1);
 for(i=0; i<MAXCLI; i++) 
	if(peerActive[i])
		{
		udpPacket.setAddress(peerAddress[i]);
		sock.send(udpPacket);
		}
 sock.close();
 udpReceiver.join();
 } 
}



class UdpChatReceive implements Runnable 
{    
private DatagramSocket s;

public UdpChatReceive(DatagramSocket udp_s) { s=udp_s;}

public void run() {
 int i;
 byte[] data = new byte[300];
 String frase;
 DatagramPacket p;
 InetAddress currPeerAddress;

 p=new DatagramPacket(data, data.length);

 while(true)
   {
   p.setLength(data.length);
   try { s.receive(p); } catch(IOException ex) { return; }
   currPeerAddress=p.getAddress();
   if(data[0]==1)
	{
	try { UdpChat.changeLock.acquire(); } catch(InterruptedException ex) { System.out.println("Interrupted"); }
	for(i=0; i<UdpChat.MAXCLI; i++)
           if(UdpChat.peerActive[i])
             if(currPeerAddress.equals(UdpChat.peerAddress[i])) break;
	if(i==UdpChat.MAXCLI) // new peer
	   {
	   for(i=0; i<UdpChat.MAXCLI; i++) if(!UdpChat.peerActive[i]) break;
	   if(i==UdpChat.MAXCLI) System.out.println("Sorry, no room for more peers");
	   else
		{
		UdpChat.peerActive[i]=true;
		UdpChat.peerAddress[i]=currPeerAddress;
		}
	   }
	UdpChat.changeLock.release();
	data[0]=1;
	p.setAddress(currPeerAddress); // send back an announcement
	p.setLength(1);
	try { s.send(p); } catch(IOException ex) { return; }
	}
   else
   if(data[0]==0)
	{
	try { UdpChat.changeLock.acquire(); } catch(InterruptedException ex) { System.out.println("Interrupted"); }
	for(i=0; i<UdpChat.MAXCLI; i++)
           if(UdpChat.peerActive[i])
             if(currPeerAddress.equals(UdpChat.peerAddress[i])) break;
	if(i<UdpChat.MAXCLI) UdpChat.peerActive[i]=false;
	UdpChat.changeLock.release();
	}
   else
	{
        frase = new String( p.getData(), 0, p.getLength());
   	System.out.println(frase);
	}
   }
 }
}
