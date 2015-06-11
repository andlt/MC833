package server;

import java.io.IOException;
import java.net.ServerSocket;

import constants.Constans;

public class Server {

	public static void main(String[] args) {
		
	    ServerSocket MyService;
	    try {
	    	MyService = new ServerSocket(Constans.PORT_NUMBER);
	    	
	    	PrintStream output;
	        output = new PrintStream(serviceSocket.getOutputStream());
	        
	        output.close();
	        input.close();
	        serviceSocket.close();
	        MyService.close();
	    	
	    }
	    catch (IOException e) {
	    	System.out.println(e);
	    }
		
		System.out.println("Server Running!");
	}
	
}
