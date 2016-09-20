import java.io.*; 
import java.net.*; 

class TcpChatSrvST
{    
public static int MAXCLI=100;
public static Socket[] cliSock = new Socket[MAXCLI];
public static Socket newCli;
public static DataOutputStream[] sOut = new DataOutputStream[MAXCLI];
public static DataInputStream[] sIn = new DataInputStream[MAXCLI];
public static Boolean[] inUse= new Boolean[MAXCLI];

static ServerSocket sock;

public static void main(String args[]) throws Exception    
{       
int i, j, nChars;
byte[] data = new byte[300];

try { sock = new ServerSocket(9999); }
catch(IOException ex) {
	System.out.println("Local port number not available.");
	System.exit(1); }

for(i=0; i<MAXCLI; i++) inUse[i]=false;
sock.setSoTimeout(100); // set the socket timeout
while(true)
	{
	try { 					// is there a new client connection request
		newCli=sock.accept();
		newCli.setSoTimeout(100); // set the connected socket timeout
		for(i=0; i<MAXCLI; i++) if(!inUse[i]) break; // find a free slot
		inUse[i]=true;
		cliSock[i]=newCli;
		sOut[i]=new DataOutputStream(cliSock[i].getOutputStream());
		sIn[i]=new DataInputStream(cliSock[i].getInputStream());
		}
	catch(SocketTimeoutException to) {}

	for(i=0; i<MAXCLI; i++) 
		if(inUse[i]) {
   			try {				// try reading the line size
        		nChars=sIn[i].read();
   			if(nChars==0) { 		// empty line - client wants to exit
				sOut[i].write(nChars);  // send back an empty line
				cliSock[i].close();
				inUse[i]=false;
				} 
			else
				{
				sIn[i].read(data,0,nChars);  // read the line
				for(j=0; j<MAXCLI; j++)
				   if(inUse[j])
					{
					sOut[j].write(nChars);
                			sOut[j].write(data,0,nChars);
					}
				}
			   }
			catch(SocketTimeoutException to) {}
			}
		}

	}
} 



