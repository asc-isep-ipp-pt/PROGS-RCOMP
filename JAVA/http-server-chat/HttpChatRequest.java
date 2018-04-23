import java.io.*;
import java.net.*;

/**
 *
 * @author ANDRE MOREIRA (asc@isep.ipp.pt)
 */
public class HttpChatRequest extends Thread {
    String baseFolder;
    Socket sock;
    DataInputStream inS;
    DataOutputStream outS;

    public HttpChatRequest(Socket s, String f) {
        baseFolder=f; sock=s;
    }
    
    @Override
    public void run() {
        try {
            outS = new DataOutputStream(sock.getOutputStream());
            inS = new DataInputStream(sock.getInputStream());
        }
        catch(IOException ex) { System.out.println("Thread error on data streams creation"); }
        try {
            HTTPmessage request = new HTTPmessage(inS);
            HTTPmessage response = new HTTPmessage();
            response.setResponseStatus("200 Ok");
                    
            if(request.getMethod().equals("GET")) {
                if(request.getURI().startsWith("/messages/")) {
                    int msgNum;
                    try { msgNum=Integer.parseInt(request.getURI().substring(10)); }
                    catch(NumberFormatException ne) { msgNum=-1; }
                    if(msgNum<0) {
                        response.setContentFromString(
                            "<html><body><h1>ERROR: 405 Method Not Allowed</h1></body></html>",
                            "text/html");
                        response.setResponseStatus("405 Method Not Allowed");                        
                    }
                    else {
                        String msg=HttpServerChat.getMsg(msgNum);
                        // if msgNum doesn't yet exist, the getMsg() method waits
                        // until it does. So the HTTP request was received, but the
                        // HTTP response is sent only when there's a message
                        response.setContentFromString(msg, "text/plain");                       
                    }
                }
                else { // NOT GET /messages/ , THEN IT MUST BE A FILE
                    String fullname=baseFolder + "/";
                    if(request.getURI().equals("/")) fullname=fullname+"index.html";
                    else fullname=fullname+request.getURI();
                    if(!response.setContentFromFile(fullname)) {
                        response.setContentFromString(
                            "<html><body><h1>404 File not found</h1></body></html>",
                            "text/html");
                        response.setResponseStatus("404 Not Found"); 
                        }                        
                }
            }
            else { // NOT GET, must be POST
                if(request.getMethod().equals("POST") 
                        && request.getURI().equals("/messages")) {
                    HttpServerChat.addMsg(request.getContentAsString());
                    response.setResponseStatus("200 Ok");                    
                }
                else {
                    response.setContentFromString(
                         "<html><body><h1>ERROR: 405 Method Not Allowed</h1></body></html>",
                        "text/html");
                    response.setResponseStatus("405 Method Not Allowed");
                }
            }
            response.send(outS); // SEND THE HTTP RESPONSE
        }
        catch(IOException ex) { System.out.println("Thread I/O error on request/response"); }
        try { sock.close();} catch(IOException ex) { System.out.println("CLOSE IOException"); }
    }
}


