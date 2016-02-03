import java.io.*; 
import java.net.*; 

class HttpRequest implements Runnable 
{    
private Socket s;
private DataOutputStream sOut;
private DataInputStream sIn;
private String CRLF="\r\n";


String readLineCRLF()
{
byte[] line = new byte[300];
int p=0;
try {
  for(;;) {
	sIn.read(line,p,1);
        if(line[p]=='\n')
                return(new String( line, 0, p));
        else
        if(line[p]!='\r') p++;
        }
  } catch(IOException ex) { System.out.println("READ IOException"); }
return(null);
}



void writeLineCRLF(String line)
{
try {
	sOut.write(line.getBytes(),0,(byte)line.length());
	sOut.write(CRLF.getBytes(),0,2);
  } catch(IOException ex) { System.out.println("WRITE IOException"); }
}






public HttpRequest(Socket cli_s) { s=cli_s;}

public void run()
{
long f,i,num,soma;
String request;
String line;
try {
sOut = new DataOutputStream(s.getOutputStream());
sIn = new DataInputStream(s.getInputStream());

} catch(IOException ex) { System.out.println("IOException"); }

request=readLineCRLF();
System.out.println("Request: " + request);

if(request.startsWith("POST /upload")) processPostUpload();
else
if(request.startsWith("POST /list")) processPostList();
else processGet(request);
	
try { s.close();} catch(IOException ex) { System.out.println("CLOSE IOException"); }

}



void processGet(String req)
{
byte[] data = new byte[300];
int done, readNow;
long len;
String line;
String fileName, filePath;
String reply;
String type;
File f;
InputStream fReader;

do { // READ AND IGNORE THE HEADER LINES
	line=readLineCRLF();
 	}
while(line.length()>0);
fileName=req.split(" ")[1];
if(fileName.compareTo("/")==0) fileName="/index.html";
filePath="www"+fileName;

f=new File(filePath);
reply="200 OK";
type="text/html";
if(!f.exists())
	{
	reply="404 Not Found";
	f=new File("www/not-found.html");
	}
else
	{
        if(filePath.endsWith(".pdf")) type="application/pdf";
        else
        if(filePath.endsWith(".txt")) type="text/plain";
        else
        if(filePath.endsWith(".gif")) type="image/gif";
        else
        if(filePath.endsWith(".png")) type="image/png";
	}

len=f.length();
writeLineCRLF("HTTP/1.0 " + reply);
writeLineCRLF("Content-Type: " + type);
writeLineCRLF("Content-Length: " + len);
writeLineCRLF("Connection: close");
writeLineCRLF("");

try {
fReader = new BufferedInputStream(new FileInputStream(f));

do
	{
	if(len>300) readNow=300; else readNow=(int)len;
	try {
	done=fReader.read(data,0,readNow);
	len=len-done;
	sOut.write(data,0,done);
		}
		catch(IOException ex) { System.out.println("IOException"); }
	}
while(len>0);

}
	catch(FileNotFoundException ex)  { System.out.println("FILE OPEN FileNotFoundException"); }

}


void processPostUpload()
{
String line, boundary, filename, filePath;
int done,readNow,len;
String cDisp="Content-Disposition: form-data; name=\"filename\"; filename=\"";
File f;
FileOutputStream fOut;
byte[] data = new byte[300];

len=0;
boundary=null;

do {	// FIRSH HEADER
	line=readLineCRLF();
	if(line.startsWith("Content-Length: "))
		{
		len=Integer.parseInt(line.split(" ")[1]);
		}
	else
	if(line.startsWith("Content-Type: multipart/form-data; boundary="))
		{
		boundary=line.split("=")[1];
		}
 	}
while(line.length()>0);


if(len==0) {replyPostError("Content-Length: expected and not found"); return;}
if(boundary==null) {replyPostError("Content-Type: multipart/form-data; expected and not found"); return;}

line=readLineCRLF();
if(!line.endsWith(boundary)) {replyPostError("Multipart separator expected and not found"); return;}
len=len-line.length()-2;
filename="";
do // SECOND HEADER
	{
	line=readLineCRLF();
	len=len-line.length()-2;
	if(line.startsWith(cDisp))
		{
		filename=line.split("=")[2];
		filename=filename.substring(1,filename.length()-1);
		}
	}
while(line.length()>0);

try {
if(filename.length()==0)
	{
	do	{done=sIn.read(data,0,300);len=len-done;} while(len>0);

	replyPostError("Content-Disposition: form-data; expected and not found (NO FILENAME)"); return;
	}
filePath="www/"+filename;
f = new File(filePath);
fOut = new FileOutputStream(f);

len=len-boundary.length()-6;

do 
	{
	if(len>300) readNow=300; else readNow=len;
	done=sIn.read(data,0,readNow);
	fOut.write(data,0,done);
	len=len-done;
	}
while(len>0);
fOut.close();
line=readLineCRLF();



} catch(IOException ex) { System.out.println("IOException"); }




replyPostList();

}


void processPostList()
{
String line;
do { // READ AND IGNORE THE HEADER LINES
	line=readLineCRLF();
 	}
while(line.length()>0);
replyPostList();
}


void replyPostList()
{
String s1="<html><head><title>File List</title></head><body><h1>File List:</h1><big><ul>";
String s2="</ul></big><hr><p><a href=/>BACK</a></body></html>";
String list;
int len;
File d;


d= new File("www/");
list="";
File[] filesList = d.listFiles();
for(File f : filesList)
        if(f.isFile())
		list=list + "<li><a href=/" + f.getName() + ">" + f.getName() + "</a>";


writeLineCRLF("HTTP/1.0 200 OK");
writeLineCRLF("Content-type: text/html");
len=s1.length() + s2.length() + list.length();
writeLineCRLF("Content-length: " + len);
writeLineCRLF("Connection: close");
writeLineCRLF("");
try {
sOut.write(s1.getBytes(),0,s1.length());
sOut.write(list.getBytes(),0,list.length());
sOut.write(s2.getBytes(),0,s2.length());
	}
	catch(IOException ex) { System.out.println("WRITE IOException"); }
}

void replyPostError(String error)
{
String s1="<html><head><title>Server Error</title></head><body><center><img src=500.png><br>(500.png)</center><h1>Server error on POST</h1><p>ERROR: ";
String s2="<hr><p><a href=/>BACK</a></body></html>";
int len;

writeLineCRLF("HTTP/1.0 500 Internal Server Error");
writeLineCRLF("Content-type: text/html");
len=s1.length() + s2.length() + error.length();
writeLineCRLF("Content-length: " + len);
writeLineCRLF("Connection: close");
writeLineCRLF("");
try {
sOut.write(s1.getBytes(),0,s1.length());
sOut.write(error.getBytes(),0,error.length());
sOut.write(s2.getBytes(),0,s2.length());
	}
	catch(IOException ex) { System.out.println("WRITE IOException"); }
}


} // END OF CLASS
