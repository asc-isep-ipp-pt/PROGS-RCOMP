import java.io.*; 
import java.net.*; 

import java.net.ServerSocket;
import java.net.Socket;

import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;

class TcpSrvSumTLS {    

	static final int SERVER_PORT=9999;
	static final String TRUSTED_STORE="server_J.jks";
	static final String KEYSTORE_PASS="forgotten";


	public static void main(String args[]) throws Exception {       
		SSLServerSocket sock=null;
		Socket cliSock;


		// Trust these certificates provided by authorized clients
                System.setProperty("javax.net.ssl.trustStore", TRUSTED_STORE);
                System.setProperty("javax.net.ssl.trustStorePassword",KEYSTORE_PASS);

                // Use this certificate and private key as server certificate
                System.setProperty("javax.net.ssl.keyStore",TRUSTED_STORE);
                System.setProperty("javax.net.ssl.keyStorePassword",KEYSTORE_PASS);

		SSLServerSocketFactory sslF = (SSLServerSocketFactory) SSLServerSocketFactory.getDefault();
		try {
    			sock = (SSLServerSocket) sslF.createServerSocket(SERVER_PORT);
			sock.setNeedClientAuth(true);
			}
		catch(IOException ex) {
            		System.out.println("Server failed to open local port " + SERVER_PORT);
            		System.exit(1);
            	}


		while(true) {
			cliSock=sock.accept();
			new Thread(new TcpSrvSumTLSThread(cliSock)).start();
			}
		} 
	}



class TcpSrvSumTLSThread implements Runnable {    
	private Socket s;
	private DataOutputStream sOut;
	private DataInputStream sIn;

	public TcpSrvSumTLSThread(Socket cli_s) { s=cli_s;}

	public void run() {
		long f,i,num,sum;
		InetAddress clientIP;

		clientIP=s.getInetAddress();
		System.out.println("New client connection from " + clientIP.getHostAddress() + 
			", port number " + s.getPort());
		try {
			sOut = new DataOutputStream(s.getOutputStream());
			sIn = new DataInputStream(s.getInputStream());
			do {
   				sum=0;
				do {
   					num=0; f=1; for(i=0;i<4;i++) {num=num+f*sIn.read(); f=f*256;}
					sum=sum+num;
					}
   				while(num>0);
   				num=sum; for(i=0;i<4;i++) {sOut.write((byte)(num%256)); num=num/256; }
   				}
			while(sum>0);

			System.out.println("Client " + clientIP.getHostAddress() + ", port number: " + s.getPort() + 
				" disconnected");
			s.close();
			}
		catch(IOException ex) { System.out.println("IOException"); }
		}
	}



