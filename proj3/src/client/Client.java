package client;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;

import constants.Constans;

public class Client {

	public static void main(String[] args) {
		Socket MyClient;
	    try {
           MyClient = new Socket("Machine name", Constans.PORT_NUMBER);
	           
           System.out.println("Client Running!");
			
		    DataInputStream input;
		    input = new DataInputStream(MyClient.getInputStream());
		    
		    PrintStream output;
		    output = new PrintStream(serviceSocket.getOutputStream());
		    
		    output.close();
		    input.close();
		    MyClient.close();

	    }
	    catch (IOException e) {
	        System.out.println(e);
	    }
		
	    

	}
	
}
