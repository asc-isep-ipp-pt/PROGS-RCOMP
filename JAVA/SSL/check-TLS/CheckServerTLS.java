
import java.io.*;
import java.net.*;

import javax.net.ssl.*;
import javax.security.cert.X509Certificate;

/**
 *
 * @author asc@isep.ipp.pt
 */
public class CheckServerTLS {
	static private SSLSocket sock;
	static private InetAddress serverIP;
	static private int serverPort;
	static private DataOutputStream sOut;
	static private DataInputStream sIn;

	// THE NUMBER OF VOTES TO CAST ON THE FIRST CANDIDATE
	static private final int VOTES_TO_CAST = 200;

	public static void main(String args[]) throws Exception {

		if(args.length<1||args.length>2) {
            		System.out.println("Usage: java CheckServerTLS {SERVER-ADDRESS} [{SERVER-PORT-NUMBER}]");
            		System.out.println("                  The default {SERVER-PORT-NUMBER} is 443 (HHTPS)." );
            		System.exit(1);
            		}

		try { serverIP = InetAddress.getByName(args[0]); }
		catch(UnknownHostException ex) {
			System.out.println("Invalid SERVER-ADDRESS.");
			System.exit(1);
			}

		if(args.length==2) {
			try { serverPort = Integer.parseInt(args[1]); }
			catch(NumberFormatException ex) {
				System.out.println("Invalid SERVER-PORT.");
				System.exit(1);
			}
		} else {
			serverPort=443;
		}
 
		// Don't set the trust store, use the default: {JREHOME}/lib/security/cacerts
		// In Linux, it's also: /etc/ssl/certs/java/cacerts
		// System.setProperty("javax.net.ssl.trustStore", "cacerts.jks");
		// System.setProperty("javax.net.ssl.trustStorePassword","public");
		
		SSLSocketFactory sf = (SSLSocketFactory) SSLSocketFactory.getDefault();
		
		try {
    			sock = (SSLSocket) sf.createSocket(serverIP,serverPort);
		}
		catch(IOException ex) {
            		System.out.println("Failed to connect to: " + args[0] + ":" + serverPort);
            		System.out.println("Application aborted.");
            		System.exit(1);
            	}

            	System.out.println("Connected to server: " + args[0] + ":" + serverPort);

		try {
			sock.startHandshake();
			SSLSession ssl = sock.getSession();
			System.out.println("------------------------------------------------------");
                	System.out.println("SSL/TLS version: " + ssl.getProtocol() + 
					"         Cypher suite: " + ssl.getCipherSuite());

			X509Certificate[] chain=ssl.getPeerCertificateChain();
			System.out.println("------------------------------------------------------");
			System.out.println("Certificate subject: " + chain[0].getSubjectDN());
			System.out.println("------------------------------------------------------");
			System.out.println("Certificate issuer: " + chain[0].getIssuerDN());
			System.out.println("------------------------------------------------------");
			System.out.println("Not before: " + chain[0].getNotBefore());
			System.out.println("------------------------------------------------------");
			System.out.println("Not after:  " + chain[0].getNotAfter());
			System.out.println("------------------------------------------------------");


		}
		catch (SSLException tlsE) {
                        System.out.println("SSL/TLS handshake has failed:\r\n" + tlsE.getCause() );
			try { sock.close(); } catch(IOException ex2) { System.out.println("Error closing socket."); }
			System.exit(1);
                }
		try { sock.close(); } catch(IOException ex2) { System.out.println("Error closing socket."); }

        	} // MAIN METHOD
    	} // CLASS
    
