package proj3;

import java.util.List;

/* Interface que define os serviços que o sevidor irá prover para o cliente */
public interface FilmSystem extends java.rmi.Remote {

	public final static String LOOKUPNAME = "FilmSystem"; // Nome utilizado no registro de objetos do RMI
	
	public String getFilmes(Integer option, List<String> params) throws java.rmi.RemoteException; 
}