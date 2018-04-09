import java.io.*; 
import java.net.*; 

class HttpRequest implements Runnable {    
	private Socket s;
	private DataOutputStream sOut;
	private DataInputStream sIn;
	private String baseFolder;

	public HttpRequest(Socket cli_s, String folder) { s=cli_s; baseFolder=folder;}

	public void run() {
		try {
			sOut = new DataOutputStream(s.getOutputStream());
			sIn = new DataInputStream(s.getInputStream());
			}
		catch(IOException ex) { System.out.println("Data Stream IOException"); }
		String request=HTTP.readLineCRLF(sIn);
		// System.out.println("Request: " + request);
		if(request.startsWith("POST /upload")) processPostUpload();
		else
		if(request.startsWith("POST /list")) processPostList();
		else processGet(request);
	
		try { s.close();}
		catch(IOException ex) { System.out.println("CLOSE IOException"); }
		}


	void processGet(String req) {
		String line, fileName, filePath;

		do { // READ AND IGNORE THE HEADER LINES
			line=HTTP.readLineCRLF(sIn);
 			}	
		while(line.length()>0);

		fileName=req.split(" ")[1];
		if(fileName.compareTo("/")==0) fileName="/index.html";
		filePath=baseFolder+fileName;
		HTTP.sendHttpFileResponse(sOut, null, filePath);
		}


	void processPostUpload() {
		String line, boundary, filename, filePath;
		int done,readNow,len;
		String cDisp="Content-Disposition: form-data; name=\"filename\"; filename=\"";
		File f;
		FileOutputStream fOut;
		byte[] data = new byte[300];

		len=0;
		boundary=null;
		do {	// FIRSH HEADER
			line=HTTP.readLineCRLF(sIn);
			if(line.startsWith("Content-Length: ")) {
				len=Integer.parseInt(line.split(" ")[1]);
				}
			else
				if(line.startsWith("Content-Type: multipart/form-data; boundary=")) {
					boundary=line.split("=")[1];
					}
 			}
		while(line.length()>0);

		if(len==0) {replyPostError("Content-Length: expected and not found"); return;}
		if(boundary==null) {replyPostError("Content-Type: multipart/form-data; expected and not found"); return;}

		line=HTTP.readLineCRLF(sIn);
		if(!line.endsWith(boundary)) {replyPostError("Multipart separator expected and not found"); return;}
		len=len-line.length()-2;
		filename="";
		do { 	// SECOND HEADER
			line=HTTP.readLineCRLF(sIn);
			len=len-line.length()-2;
			if(line.startsWith(cDisp)) {
				filename=line.split("=")[2];
				filename=filename.substring(1,filename.length()-1);
				}
			}
		while(line.length()>0);

		try {
			if(filename.length()==0) {
				do { done=sIn.read(data,0,300);len=len-done;} while(len>0);
				replyPostError("Content-Disposition: form-data; expected and not found (NO FILENAME)");
				return;
				}
			filePath=baseFolder+"/"+filename;
			f = new File(filePath);
			fOut = new FileOutputStream(f);

			len=len-boundary.length()-6;

			do {
				if(len>300) readNow=300; else readNow=len;
				done=sIn.read(data,0,readNow);
				fOut.write(data,0,done);
				len=len-done;
				}
			while(len>0);
			fOut.close();
			line=HTTP.readLineCRLF(sIn);
			}
		catch(IOException ex) { System.out.println("IOException"); }
		replyPostList();
		}


	void processPostList() 	{
		String line;
		do { // READ AND IGNORE THE HEADER LINES
			line=HTTP.readLineCRLF(sIn);
 			}
		while(line.length()>0);
		replyPostList();
		}


	void replyPostList() {
		String s1="<html><head><title>File List</title></head><body><h1>File List:</h1><big><ul>";
		String s2="</ul></big><hr><p><a href=/>BACK</a></body></html>";
		String list;
		File d;

		d= new File("www/");
		list=s1;
		File[] filesList = d.listFiles();
		for(File f : filesList)
        		if(f.isFile())
				list=list + "<li><a href=/" + f.getName() + ">" + f.getName() + "</a>";
		list=list+s2;
		HTTP.sendHttpStringResponse(sOut, "200 Ok", "text/html", list);
		}


	void replyPostError(String error) {
		HTTP.sendHttpStringResponse(sOut, "500 Internal Server Error", "text/html", 
			"<html><head><title>Server Error</title></head><body><center><img src=500.png><br>(500.png)</center>"
			+ "<h1>Server error on POST</h1><p>ERROR: " + error +
			"<hr><p><a href=/>BACK</a></body></html>");
		}


	} // END OF CLASS


