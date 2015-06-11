package client;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.Socket;

import constants.Constans;

public class Client {

	public static void main(String[] args) {
		Socket MyClient;
	    try {
           MyClient = new Socket("Machine name", Constans.PORT_NUMBER);
	           
           System.out.println("Client Running!");
			
		    DataInputStream input;
		    try {
		    	input = new DataInputStream(MyClient.getInputStream());
		    }
		    catch (IOException e) {
		       System.out.println(e);
		    }
	    }
	    catch (IOException e) {
	        System.out.println(e);
	    }
		

	}
	
}
