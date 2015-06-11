package proj3;

import java.io.IOException;
import java.rmi.Naming;
import java.util.Date;
import java.util.Scanner;

/* A very simple client for the RemoteDate service. */
public class DateClient {

	/** The local proxy for the service. */
	protected static RemoteDate netConn = null;

	public static void main(String[] args) {
		
		System.out.println("Número de repetições desejadas:");
		Scanner scanner = new Scanner(System.in);  
		int repetitions = scanner.nextInt();
		scanner.close();
		
		System.out.println(repetitions);
		
		for(int i = 0; i < repetitions; i++) {
			try {
				long startTime = System.nanoTime();
				netConn = (RemoteDate)Naming.lookup(RemoteDate.LOOKUPNAME);
				Date today = netConn.getRemoteDate();
				long estimatedTime = System.nanoTime() - startTime;
				System.out.println(today.toString());
				System.out.println("Time elapsed: "+estimatedTime/*/1000000*/);
			} catch (Exception e) {
				System.err.println("RemoteDate exception: " + e.getMessage());
				e.printStackTrace();
			}
		}

	}
}