package proj3;

import java.rmi.Naming;
import java.util.Scanner;

/* Classe que implementa o cliente do sistema, responsável pelo contato com o servidor */
public class FilmSystemClient {

	// Proxy local para o FilmSystem 
	protected static FilmSystem netConn = null;

	public static void main(String[] args) {
		
		// Solicita ao usuário que entre com a requisição e número de repetições
		Scanner scanner = new Scanner(System.in);  
		System.out.println("Opção:");
		int options = scanner.nextInt();
		System.out.println("Número de repetições desejadas:");
		int repetitions = scanner.nextInt();
		scanner.close();
		
		// Repete a chamada reptition vezes, para fins de medição do tempo
		for(int i = 0; i < repetitions; i++) {
			try {
				long startTime = System.nanoTime(); // Inicia a contagem do tempo
				netConn = (FilmSystem)Naming.lookup(FilmSystem.LOOKUPNAME); // Abre a conexão
				String ret = netConn.getFilmes(options, null); // Guarda a resposta da chamada RMI
				long estimatedTime = System.nanoTime() - startTime; // Finaliza a contagem do tempo
//				System.out.println(ret); // Imprime a resposta
				System.out.println(estimatedTime/1000); // Imprime o tempo decorrido (medido em microsegundos)
			} catch (Exception e) {
				System.err.println("Exceção encontrada: " + e.getMessage());
				e.printStackTrace();
			}
		}

	}
}