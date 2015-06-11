package server;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class ServerImpl extends UnicastRemoteObject implements Server {

	private static final long serialVersionUID = 8026991899928164298L;

	public ServerImpl() throws RemoteException {
		super(); 
	}
	
	public String Hello() {
		System.out.println("Invocation to Hello was succesful!");
		return "Hello World from RMI server!"; 
	}
	
	public static void main(String args[]) {
		try { 
			// Creates an object of the HelloServer class.
			ServerImpl obj = new ServerImpl(); 
			// Bind this object instance to the name "HelloServer". 
			Naming.rebind("HelloServer", obj); 
			System.out.println("Ligado no registro"); 
		} catch (Exception ex) { 
			System.out.println("error: " + ex.getMessage()); 
			ex.printStackTrace();
		} 
	}
}
