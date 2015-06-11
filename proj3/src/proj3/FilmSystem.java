package proj3;

import java.util.List;

/** A statement of what the client & server must agree upon. */
public interface FilmSystem extends java.rmi.Remote {

	/** The name used in the RMI registry service. */
	public final static String LOOKUPNAME = "FilmSystem";
	
	public String getFilmes(Integer option, List<String> params) throws java.rmi.RemoteException; 
}
// END main