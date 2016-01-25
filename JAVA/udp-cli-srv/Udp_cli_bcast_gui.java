package udp.cli.bcast;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;
import java.io.*; 
import java.net.*; 
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;


public class Udp_cli_bcast_gui extends Application {

static private InetAddress IPdestino;
static private DatagramSocket sock;
TextField textToSend;
TextArea textRec;
    
public void sendAndReceive() {
    byte[] data;
    String frase=textToSend.getText();
    if(frase.isEmpty()) return;
    data = frase.getBytes();
    DatagramPacket request = new DatagramPacket(data, frase.length(), IPdestino, 9999);
    try {
    sock.send(request);
    DatagramPacket reply = new DatagramPacket(data, data.length);
    sock.receive(reply);
    IPdestino=reply.getAddress();
    frase = new String( reply.getData(), 0, reply.getLength());   
    } 
      catch(SocketTimeoutException ex) {frase="!! O servidor não respondeu.";}
      catch(IOException ex) { frase="!! Network IO error."; }
      
    
    textRec.appendText("\n" + frase);
    textToSend.setText("");
    }
    
@Override
    public void start(Stage primaryStage) {
        Button sendB = new Button("ENVIAR");
        Button exitB = new Button("SAIR");
       
        textToSend = new TextField();
        textRec = new TextArea("");
        
        sendB.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) { sendAndReceive();}});
        
        
        
        exitB.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) { Platform.exit();}});
    
        
        textToSend.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent event) { sendAndReceive();}});
 
        
        GridPane grid = new GridPane();
        //grid.setGridLinesVisible(true);
        grid.setAlignment(Pos.CENTER);
        grid.setHgap(5); grid.setVgap(10);
        grid.setPadding(new Insets(5, 5, 5, 5));
       
        Scene scene = new Scene(grid, 400, 300);
        
        textRec.setEditable(false);
        textRec.setFocusTraversable(false);
        
               
        grid.add(textRec,0,0,4,1);
        
        grid.add(textToSend,0,1,4,1);        
        grid.add(sendB,1,2); grid.add(exitB,3,2);
       
                       
        primaryStage.setTitle("Udp_cli_bcast_gui");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    
public static void main(String args[]) throws Exception    
{       
sock = new DatagramSocket();
sock.setBroadcast(true);
sock.setSoTimeout(500);
IPdestino=InetAddress.getByName("255.255.255.255");
launch(args);
sock.close();
} 
}


