package proj3;

import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;

/* Classe que implementa o servidor do sistema, que provê os serviços de FimSystem para o cliente */
public class FilmSystemServer {
	public static void main(String[] args) {

		try {
			// Instancia o objeto com as funções providas
			FilmSystemImpl im = new FilmSystemImpl();

			System.out.println("Inicializando servidor...");

			// Coloca o serviço no registro de objetos do RMI
			LocateRegistry.createRegistry(1099); // 1099 é a porta padrão
			Naming.rebind(FilmSystem.LOOKUPNAME, im); // modifica o nome para aquele acordado na interface

			System.out.println("Servidor pronto.");
		} catch (Exception e) {
			System.err.println(e);
			System.exit(1);
		}
	}
}
