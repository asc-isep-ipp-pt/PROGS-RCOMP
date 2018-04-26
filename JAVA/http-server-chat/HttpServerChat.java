import java.io.*;
import java.net.*;
import java.util.*;

/**
 *
 * @author ANDRE MOREIRA (asc@isep.ipp.pt)
 */
public class HttpServerChat {
    static private final String BASE_FOLDER="www";
    static private ServerSocket sock;

    public static void main(String args[]) throws Exception {
        Socket cliSock;
        
        if(args.length!=1) {
            System.out.println("Local port number required at the command line.");
            System.exit(1);
        }
        try { sock = new ServerSocket(Integer.parseInt(args[0])); }
        catch(IOException ex) {
            System.out.println("Server failed to open local port " + args[0]);
            System.exit(1);
        }
        System.out.println("Server ready, listening on port number " + args[0]);
	addMsg("HTTP Chat Server is ready ...");
        while(true) { 
            cliSock=sock.accept();
            HttpChatRequest req=new HttpChatRequest(cliSock, BASE_FOLDER);
            req.start();
        }
    } 
	
    // MESSAGES ARE ACCESSED BY THREADS - LOCKING REQUIRED
    private static int nextMsgNum = 0;
    private static final ArrayList<String> MSG_LIST = new ArrayList<>();

    public static String getMsg(int msgNumber) {
        synchronized(MSG_LIST) {
            while(msgNumber>=nextMsgNum) {   
                try { MSG_LIST.wait(); }    // wait for a notification on MSG_LIST's monitor
                                            // while waiting MSG_LIST's intr lock is released
                catch(InterruptedException ex) {
                    System.out.println("Thread error: interrupted");
                    return null;
                }
            }
            return MSG_LIST.get(msgNumber);
        }
    }
    
    public static void addMsg(String msg) {
        synchronized(MSG_LIST) {
            MSG_LIST.add(nextMsgNum, msg);
            nextMsgNum++;
            MSG_LIST.notifyAll(); // notify all threads waiting on MSG_LIST's monitor
        }
    }
    
    
}

