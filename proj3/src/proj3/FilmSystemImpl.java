package proj3;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;

/* Classe que implementa os serviços que o servidor provê para o cliente */
public class FilmSystemImpl extends UnicastRemoteObject implements FilmSystem {

	private static final long serialVersionUID = 4357486579370675619L;

	/* Consultas SQL para as possíveis operações. */
//	Opção 0 - 'Listar todos os títulos dos filmes e o ano de lançamento'
	private static String SQL_0 = "SELECT titulo, anoLancamento FROM filme;";

//	Opção 1 - 'Listar todas as informações de todos os filmes'
	private static String SQL_1 = "SELECT * FROM filme;";

//	Opção 2 - 'Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado'
	private static String SQL_2 = "SELECT titulo, anoLancamento FROM filme WHERE genero ILIKE '%@GENERO@%';";
	
//	Opção 3 - 'Dado o identificador de um filme, retornar a sinopse do filme'
	private static String SQL_3 = "SELECT sinopse FROM filme WHERE id = @ID@;";
	
//	Opção 4 - 'Dado o identificador de um filme, retornar todas as informações deste filme'
	private static String SQL_4 = "SELECT * FROM filme WHERE id = @ID@;";
	
//	Opção 5 - 'Dado o identificador de um filme, retornar o número de exemplares em estoque'
	private static String SQL_5 = "SELECT numExemplares FROM filme WHERE id = @ID@;";
	
//	Opção 6 - 'Alterar o número de exemplares em estoque (apenas cliente locadora)'
	private static String SQL_6 = "UPDATE filme SET numExemplares = @NEW_NUM_EXEMPLARES@ WHERE id = @ID@;";
	private static String SQL_6_VIEW = "SELECT numExemplares FROM filme WHERE id = @ID@;";

	// Construtor
	public FilmSystemImpl() throws RemoteException {
		super();
	}

	// Recebe a opção do cliente e os parâmetros, retorna  o resultado desejado
	public String getFilmes(Integer option, List<String> params) throws java.rmi.RemoteException {
		String ret = "";
		Connection conn = null;
        try {
        	// Faz a conexão com o banco de dados local.
        	Class.forName("org.postgresql.Driver");
        	String url = "jdbc:postgresql://localhost:5433/mc833"; // porta do banco
        	conn = DriverManager.getConnection(url,"postgres", "postgres"); // usuário e senha

            Statement st = conn.createStatement();
            ResultSet rs = null;

            // Escolhe a consulta a ser executada a partir da opção selecionada pelo cliente.
            switch (option) {
				case 0: 
					rs = st.executeQuery(SQL_0);
					break;
				case 1:
					rs = st.executeQuery(SQL_1);
					break;
				case 2:
					rs = st.executeQuery(SQL_2.replace("@GENERO@", params.get(0)));
					break;
				case 3:
					rs = st.executeQuery(SQL_3.replace("@ID@", params.get(0)));
					break;
				case 4:
					rs = st.executeQuery(SQL_4.replace("@ID@", params.get(0)));
					break;
				case 5:
					rs = st.executeQuery(SQL_5.replace("@ID@", params.get(0)));
					break;
				case 6:
					st.executeUpdate(SQL_6.replace("@ID@", params.get(0)).replace("@NEW_NUM_EXEMPLARES@", params.get(1)));
					rs = st.executeQuery(SQL_6_VIEW.replace("@ID@", params.get(0)));
					break;
				default:
					ret = "Opção selecionada inválida.\n";
					break;
			}

            // Prepara a resposta
            if (option >= 0 && option <= 6) {
	            while (rs.next()) {
	            	try {
	            		if (rs.getInt("id") != 0)				ret += "ID: " + rs.getInt("id") + "\n";
					} catch (Exception e) {}
	            	try {
	            		if (rs.getString("titulo") != null) 	ret += "Título: " + rs.getString("titulo") + "\n";;
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getInt("anoLancamento") != 0) 	ret += "Ano de lançamento: " + rs.getInt("anoLancamento") + "\n";
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getString("genero") != null) 	ret += "Gênero: " + rs.getString("genero") + "\n";;
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getInt("duracao") != 0) 			ret += "Duração: " + rs.getInt("duracao") + "\n";
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getString("sinopse") != null) 	ret += "Sinopse: " + rs.getString("sinopse") + "\n";;
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getString("diretor") != null) 	ret += "Diretor: " + rs.getString("diretor") + "\n";;
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getString("elenco") != null) 	ret += "Elenco: " + rs.getString("elenco") + "\n";;
	            	} catch (Exception e) {}
	            	try {
	            		if (rs.getInt("numExemplares") != 0) 	ret += "Número de Exemplares: " + rs.getInt("numExemplares") + "\n";
	            	} catch (Exception e) {}
	            	ret += "\n";
	            }
            }
            // Fecha as conexões com o banco.
            rs.close();
            st.close();
        } catch (ClassNotFoundException e) {
        	e.printStackTrace();
        	System.exit(1);
        } catch (SQLException e) {
        	e.printStackTrace();
        	System.exit(2);
        }
        return ret;
	}
}
