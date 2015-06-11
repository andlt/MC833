package proj3;

import java.rmi.Naming;
import java.util.Scanner;

/* A very simple client for the RemoteDate service. */
public class FilmSystemClient {

	/** The local proxy for the service. */
	protected static FilmSystem netConn = null;

	public static void main(String[] args) {
		
		System.out.println("Opção:");
		Scanner scanner = new Scanner(System.in);  
		int options = scanner.nextInt();
		System.out.println("Número de repetições desejadas:");
		int repetitions = scanner.nextInt();
		scanner.close();
		
		System.out.println(repetitions);
		
		for(int i = 0; i < repetitions; i++) {
			try {
				long startTime = System.nanoTime();
				netConn = (FilmSystem)Naming.lookup(FilmSystem.LOOKUPNAME);
				String ret = netConn.getFilmes(options, null);
				long estimatedTime = System.nanoTime() - startTime;
//				System.out.println(ret);
				System.out.println(estimatedTime/*/1000000*/);
			} catch (Exception e) {
				System.err.println("RemoteDate exception: " + e.getMessage());
				e.printStackTrace();
			}
		}

	}
}