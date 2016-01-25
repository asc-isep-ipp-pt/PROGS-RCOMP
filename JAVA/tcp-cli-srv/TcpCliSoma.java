import java.io.*; 
import java.net.*; 

class TcpCliSoma
{    
static InetAddress IPdestino;
static Socket sock;

public static void main(String args[]) throws Exception    
{       
if(args.length!=1) {
	System.out.println("Deve indicar o host de destino na linha de comando");
	System.exit(1); }

try { IPdestino = InetAddress.getByName(args[0]); }
catch(UnknownHostException ex) {
	System.out.println("O nome de host de destino fornecido (" + args[0] + ") nao foi resolvido");
	System.exit(1); }

try { sock = new Socket(IPdestino, 9999); }
catch(IOException ex) {
	System.out.println("A ligacao TCP falhou.");
	System.exit(1); }

BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
DataOutputStream sOut = new DataOutputStream(sock.getOutputStream());
DataInputStream sIn = new DataInputStream(sock.getInputStream());


String frase;
long f,i,n,num;
do
   {
   do
	{
	num=-1;
	while(num<0) {
	  System.out.print("Numero inteiro positivo a somar (zero para terminar): ");
	  frase = in.readLine();
	  try { num=Integer.parseInt(frase); } catch(NumberFormatException ex) {num=-1;}
	  if(num<0) System.out.println("Formato nao aceite");
	  }
	n=num; for(i=0;i<4;i++) {sOut.write((byte)(n%256)); n=n/256; }
	}
   while(num!=0);
   num=0; f=1; for(i=0;i<4;i++) {num=num+f*sIn.read(); f=f*256;}
   System.out.println("A soma recebida foi: " + num);
   }
while(num!=0);
sock.close();
} 
}

