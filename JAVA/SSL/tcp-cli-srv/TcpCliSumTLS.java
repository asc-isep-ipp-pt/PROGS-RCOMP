
import java.io.*;
import java.net.*;
import java.security.KeyStore;
import javax.net.*;
import javax.net.ssl.*;
import javax.security.cert.X509Certificate;

/**
 *
 * @author asc@isep.ipp.pt
 */

class TcpCliSumTLS {    

	static final int SERVER_PORT=9999;
	static final String KEYSTORE_PASS="forgotten";

	static InetAddress serverIP;
	static SSLSocket sock;

	public static void main(String args[]) throws Exception {
		if(args.length!=2) {
			System.out.println("Server IPv4/IPv6 address/DNS name is required as first argument");
			System.out.println("Client name is required as second argument (a matching keystore must exist)");
			System.exit(1); }
		
		// Trust these certificates provided by servers
		System.setProperty("javax.net.ssl.trustStore", args[1]+".jks");
		System.setProperty("javax.net.ssl.trustStorePassword",KEYSTORE_PASS);

		// Use this certificate and private key for client certificate when requested by the server
		System.setProperty("javax.net.ssl.keyStore",args[1]+".jks");
    		System.setProperty("javax.net.ssl.keyStorePassword",KEYSTORE_PASS);

		SSLSocketFactory sf = (SSLSocketFactory) SSLSocketFactory.getDefault();

		try { serverIP = InetAddress.getByName(args[0]); }
		catch(UnknownHostException ex) {
			System.out.println("Invalid server specified: " + args[0]);
			System.exit(1); }


		try {
    			sock = (SSLSocket) sf.createSocket(serverIP,SERVER_PORT);
		}
		catch(IOException ex) {
            		System.out.println("Failed to connect to: " + args[0] + ":" + SERVER_PORT);
            		System.out.println("Application aborted.");
            		System.exit(1);
            	}

		System.out.println("Connected to: " + args[0] + ":" + SERVER_PORT);


		sock.startHandshake();

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		DataOutputStream sOut = new DataOutputStream(sock.getOutputStream());
		DataInputStream sIn = new DataInputStream(sock.getInputStream());


		String frase;
		long f,i,n,num;
		do {
   			do {
				num=-1;
				while(num<0) {
	  				System.out.print("Enter a positive integer to SUM (zero to terminate): ");
	  				frase = in.readLine();
	 				try { num=Integer.parseInt(frase); }
					catch(NumberFormatException ex) {num=-1;}
	  				if(num<0) System.out.println("Invalid number");
	  				}
				n=num; for(i=0;i<4;i++) {sOut.write((byte)(n%256)); n=n/256; }
				}
   			while(num!=0);
   			num=0; f=1;
			for(i=0;i<4;i++) {num=num+f*sIn.read(); f=f*256;}
   			System.out.println("SUM RESULT = " + num);
   			}
		while(num!=0);
		sock.close();
		} 
	}

