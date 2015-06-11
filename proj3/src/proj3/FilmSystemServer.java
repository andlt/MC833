package proj3;

import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;

public class FilmSystemServer {
	public static void main(String[] args) {

		// You may want a SecurityManager for downloading of classes:
		// System.setSecurityManager(new RMISecurityManager());

		try {
			// Create an instance of the server object
			FilmSystemImpl im = new FilmSystemImpl();

			System.out.println("DateServer starting...");

			LocateRegistry.createRegistry(1099);
			// Publish it in the RMI registry.
			// Of course you have to have rmiregistry or equivalent running!
			Naming.rebind(FilmSystem.LOOKUPNAME, im);

			System.out.println("DateServer ready.");
		} catch (Exception e) {
			System.err.println(e);
			System.exit(1);
		}
	}
}
