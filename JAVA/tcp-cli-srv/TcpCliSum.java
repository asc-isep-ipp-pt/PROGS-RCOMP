import java.io.*; 
import java.net.*; 

class TcpCliSum {    
	static InetAddress serverIP;
	static Socket sock;

	public static void main(String args[]) throws Exception {
		if(args.length!=1) {
			System.out.println("Server IPv4/IPv6 address or DNS name is required as argument");
			System.exit(1); }

		try { serverIP = InetAddress.getByName(args[0]); }
		catch(UnknownHostException ex) {
			System.out.println("Invalid server specified: " + args[0]);
			System.exit(1); }

		try { sock = new Socket(serverIP, 9999); }
		catch(IOException ex) {
			System.out.println("Failed to establish TCP connection");
			System.exit(1); }

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

