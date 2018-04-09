import java.io.*; 
import java.net.*; 

class HTTP {    
	static private final String HTTP_VERSION ="HTTP/1.0";
	static private final String HTTP_CONNECTION_CLOSE ="Connection: close";
	static private final String HTTP_CRLF ="\r\n";
	
	static String readLineCRLF(DataInputStream sIn) {
		byte[] line = new byte[300];
		int p=0;
		try {
  			for(;;) {
				sIn.read(line,p,1);
        			if(line[p]=='\n') return(new String( line, 0, p));
        			else
        				if(line[p]!='\r') p++;
        			}
			}
		catch(IOException ex) { System.out.println("READ IOException"); }
		return(null);
		}

	static void writeLineCRLF(DataOutputStream sOut, String line) {
		try {
			sOut.write(line.getBytes(),0,(byte)line.length());
			sOut.write(HTTP_CRLF.getBytes(),0,2);
			} 
		catch(IOException ex) { System.out.println("WRITE IOException"); }
		}	


	static void sendHttpResponseHeader(DataOutputStream sOut, String status, String contentType, int contentLength) {
		writeLineCRLF(sOut, HTTP_VERSION + " " + status);
		writeLineCRLF(sOut, "Content-Type: " + contentType);
		writeLineCRLF(sOut, "Content-Length: " + contentLength);
		writeLineCRLF(sOut, HTTP_CONNECTION_CLOSE);
		writeLineCRLF(sOut, "");
		}

	static void sendHttpResponse(DataOutputStream sOut, String status, String contentType, 
			byte[] content, int contentLength) {
		sendHttpResponseHeader(sOut,status,contentType,contentLength);
		try {
			sOut.write(content,0,contentLength);
			}
		catch(IOException ex) { System.out.println("IOException"); }
		}


	static void sendHttpStringResponse(DataOutputStream sOut, String status, String contentType, String content) {
		sendHttpResponse(sOut,status,contentType,content.getBytes(),content.length());
		}


	static void sendHttpFileResponse(DataOutputStream sOut, String status, String filePath) {
		String responseStatus="200 Ok";
		String contentType="text/html";
		File f;

		f=new File(filePath);
		if(!f.exists()) {
			sendHttpStringResponse(sOut, "404 Not Found", contentType, 
				"<html><body><h1>404 File not found</h1></body></html>");
			return;
			}
        	if(filePath.endsWith(".pdf")) contentType="application/pdf";
        	else
        	if(filePath.endsWith(".js")) contentType="application/javascript";
        	else
        	if(filePath.endsWith(".txt")) contentType="text/plain";
        	else
        	if(filePath.endsWith(".gif")) contentType="image/gif";
        	else
        	if(filePath.endsWith(".png")) contentType="image/png";

		if(status!=null) responseStatus=status;

		int len= (int) f.length();
		sendHttpResponseHeader(sOut,responseStatus,contentType,len);
		byte[] data = new byte[300];
		int done, readNow;
		InputStream fReader;
		try {
			fReader = new BufferedInputStream(new FileInputStream(f));
			do {
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


	}



