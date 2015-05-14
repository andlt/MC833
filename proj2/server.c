/*
- Identificadores dos Campos: 
    0 - ID
    1 - Título
    2 - Ano de lançamento
    3 - Gênero
    4 - Duração (min)
    5 - Sinopse
    6 - Diretor
    7 - Elenco
    8 - Número de exemplares em estoque
- Identificadores das Funções:
    0 - List;
    1 - Filter;
    2 - Edit;
*/

#include <stdio.h>
#include <stdlib.h> // strtol()
#include <netinet/in.h> // sockaddr_in
#include <string.h> // memset()
#include <strings.h> // bzero()

#define PROTOCOL_FMLY AF_INET // família de protocolo (AF_INET: IPv4)
#define SOCK_TYPE SOCK_DGRAM // tipo de socket (datagram)
#define SERV_PORT 4000 // porta do servidor
#define LISTENQ 8 // Número máximo de conexões de clientes
#define MAX_LINE 4096 // tamaho máximo de uma linha

char dataBase[9][255];
char strBuffer[MAX_LINE];
char resp[MAX_LINE];

/*       Função: list(char *params)
 *    Descrição: Lista os campos passados pelo parâmetro "params" (que chega através de códigos).
 *      Entrada: params (char *) - String com os parâmetros a serem utilizados no método. Esses parâmetros seguem o seguinte formato:
 *               0 [quantidade de parâmetros seguintes] [sequência de campos a serem exibidos]
 *               Ex: 0 2 1 2, onde:
 *               - 0 - Função list a ser executada;
 *               - 2 - Dois campos que serão listados;
 *               - 1 - Campo "Título" que será listado;
 *               - 2 - Campo "Ano de lançamento" que será listado.
 *        Saída: (void)
 * Casos de uso: - Listar todos os títulos dos filmes e o ano de lançamento;
 *               - Listar todas as informações de todos os filmes.
 */
void list(char *params) {
    int indexInitial = 4;
	int i;
    for (i = indexInitial; i < (strtol(&params[2], NULL, 10) * 2) + indexInitial; i += 2) { // Percorre os campos a serem listados.
        int index = strtol(&params[i], NULL, 10); // Pega o índice do campo a ser listado.
        strcat(resp, dataBase[index]); // Concatena na resposta da requisição o campo.
        strcat(resp, "\t");
    }
    strcat(resp, "\n");
}

/*       Função: filter(char *params)
 *    Descrição: Lista os campos passados pelo parâmetro "params" (que chega através de códigos), filtrados por um dos seguintes campos: ID ou Gênero.
 *      Entrada: params (char *) - String com os parâmetros a serem utilizados no método. Esses parâmetros seguem o seguinte formato:
 *               1 [campo a ser filtrado (ID/Gênero)] [valor do campo a ser filtrado] [quantidade de parâmetros seguintes] [sequência de campos a serem exibidos]
 *               Ex: 1 3 Drama 2 1 2, onde:
 *               - 1 - Função filter a ser executada;
 *               - 3 - Será filtrado pelo campo com identificador 3, ou seja, "Gênero";
 *               - Drama - Valor do campo "Gênero" que será usado na filtragem;
 *               - 2 - Dois campos que serão listados;
 *               - 1 - Campo "Título" que será listado;
 *               - 2 - Campo "Ano de lançamento" que será listado.
 *        Saída: (void)
 * Casos de uso: - Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado;
 *               - Dado o identificador de um filme, retornar a sinopse do filme;
 *               - Dado o identificador de um filme, retornar todas as informações deste filme;
 *               - Dado o identificador de um filme, retornar o número de exemplares em estoque;
 */
void filter(char *params) {
    int campo = strtol(&params[2], NULL, 10);
    if (campo == 0) { // Filtro pelo ID
        int filmIdParam = strtol(&params[4], NULL, 10); // Pega o ID do filme passado por parâmetro
        int dbFilmId = strtol(dataBase[0], NULL, 10); // Pega o ID do filme do banco
        if (filmIdParam == dbFilmId) {
            int indexInitial = 8;
			int i;
            for (i = indexInitial; i < (strtol(&params[6], NULL, 10) * 2) + indexInitial; i += 2) {
                int index = strtol(&params[i], NULL, 10);
                strcat(resp, dataBase[index]);
                strcat(resp, "\t");
            }
            strcat(resp, "\n");
        }
    } else if (campo == 3) { // Filtro pela sinopse
        char genero[255];
        int index = 0;
        int indexParam = 4;
        while (params[indexParam] != ' ') { // Vai montando a string com o gênero a ser filtrado
            genero[index++] = params[indexParam++];
        }
        if (strstr(dataBase[3], genero) != NULL) {
            int indexInitial = indexParam + 3;
			int i;
            for (i = indexInitial; i < (strtol(&params[indexParam + 1], NULL, 10) * 2) + indexInitial; i += 2) {
                int index = strtol(&params[i], NULL, 10);
                strcat(resp, dataBase[index]);
                strcat(resp, "\t");
            }
            strcat(resp, "\n");
        }
    }
}

/*       Função: edit(char *params, FILE * newFile)
 *    Descrição: Edita o número de exemplares em estoque de um determinado filme.
 *      Entrada: params (char *) - String com os parâmetros a serem utilizados no método. Esses parâmetros seguem o seguinte formato:
 *               2 [ID a ser alterado] [nova quantidade de exemplares em estoque]
 *               Ex: 2 3 15, onde:
 *               - 2 - Função edit a ser executada;
 *               - 3 - Identificador do filme a ser editado;
 *               - 15 - Novo valor do campo "número de exemplares em estoque".
 *               newFile (FILE *) - arquivo auxiliar onde serão escritos os novos valores, e depois substituirá o arquivo original.
 *        Saída: (void)
 * Casos de uso: - Alterar o número de exemplares em estoque (apenas cliente locadora).
 */
void edit(char *params, FILE * newFile) {
    int filmIdParam = strtol(&params[2], NULL, 10);
    int dbFilmId = strtol(dataBase[0], NULL, 10);
    if (filmIdParam == dbFilmId) {
        fprintf(newFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", dataBase[0], dataBase[1], dataBase[2], dataBase[3], dataBase[4], dataBase[5], dataBase[6], dataBase[7], &params[4]);
    } else {
        fprintf(newFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", dataBase[0], dataBase[1], dataBase[2], dataBase[3], dataBase[4], dataBase[5], dataBase[6], dataBase[7], dataBase[8]);
    }
}

void process(char *params)
{
	/* Recebe os parâmetros do cliente e chama as funções adequaddas */

    int valid = 1; // 1 se os campos recebidos forem válidos, 0 se não

    // TODO: Fazer a validação dos campos passados
    // for (int i = 2; i < strlen(params); i++) {
    //     int index = strtol(&params[i], NULL, 10);
    //     if (index < 0 || index > 8) {
    //         printf("Campo não existe.\n");
    //         valid = 0;
    //         break;
    //     }
    // }
    // if (strcmp(&params[1], "0") != 0 && strcmp(&params[1], "1") != 0 && strcmp(&params[1], "2") != 0) {
    //     printf("Função não existe.\n"); 
    //     valid = 0;
    // }

    if (valid == 1) {
        FILE *pFile, *newFile;
        char c;
        int field = 0;
        int text = 0;
        int firstLine = 0;

		// Abre o arquivo do dataBase e cria um novo que será auxiliar caso seja feita a operação de edição.
        pFile = fopen("db.txt","r");
        newFile = fopen("db2.txt","w");

		// Caso seja uma edição, será escrito um novo arquivo, então coloca o cabeçalho dos campos nele.
        if (strcmp(&params[0], "2") == 0)
            fprintf(newFile, "ID\tTítulo\tAno de lançamento\tGênero\tDuração (min)\tSinopse\tDiretor\tElenco\tNúmero de exemplares em estoque\n");

        if (pFile == NULL) {
            perror("Erro ao abrir o arquivo");
        } else {
            do {
                c = fgetc(pFile);
                if (firstLine == 0) { // Ignora a primeira linha, que é o cabeçalho do banco.
                    if (c == '\n') firstLine++;
                } else if (c == '\t') { // Tabulação indica que terminou um campo e virá o próximo.
                    text = 0;
                    field++;
                } else if (c == '\n') { // Quebra de linha indica que terminou um registro e virá o próximo.

                    // Nesse ponto já tem um registro a ser analisado, então realiza a função desejada sobre ele.
                    if (strcmp(&params[0], "0") == 0) {
                        list(params);
                    } else if (strcmp(&params[0], "1") == 0) {
                        filter(params);
                    } else if (strcmp(&params[0], "2") == 0) {
                        edit(params, newFile);
                    } else {
                        printf("Função não existe.\n"); 
                    }

                    field = 0;
                    text = 0;
                    memset(dataBase, 0, sizeof dataBase); // Após realizar a função, limpa a variável local com o registro, para armazenar o próximo.
                } else {
                    dataBase[field][text] = c; // Vai concatenando o próximo caracter do campo.
                    text++;
                }
            } while (c != EOF);
            fclose(pFile);
            fclose(newFile);
        }

        // Caso a função realizada tenha sido a de edição, então um novo arquivo foi criado.
        // Este está com os valores atualizados, então este substitui o antigo.
        if (strcmp(&params[0], "2") == 0) {
            rename("db2.txt", "db.txt");
            strcat(resp, "Número de exemplares alterado com sucesso.\n");
        } else {
            remove("db2.txt");
        }
    }
}

void dg_echo(int sockfd, struct sockaddr * pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAX_LINE];

	for ( ; ; ) {
		len = clilen;
		n = recvfrom(sockfd, mesg, MAX_LINE, 0, pcliaddr, &len);

		printf("%s", mesg);

		sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}

int main (int argc, char **argv)
{
	int sockfd = -1; // código de retorno da criação do socket
	/*int recvCode = -1; // código de retorno da recv() */
    struct sockaddr_in servaddr; // estrutura para endereço (IPv4)
    struct sockaddr_in cliaddr; // estrutura para endereço (IPv4)
    /*socklen_t clilen; // tamamnho do endereço cli
	int connfd; // TODO inicializar
    pid_t childpid; // IDs dos processos filhos*/

	// Zera o buffer
	bzero(&servaddr, sizeof(servaddr));

	// Cria o socket
	if ((sockfd = socket (PROTOCOL_FMLY, SOCK_TYPE, 0)) < 0) { // se sockfd retornar negativo, houve um erro
        printf("Problema na criação do socket. sockfd retornou %d.\n", sockfd);
        return 2;
    }

	// Prepara o endereço do socket
	/*memset(&servaddr, 0, sizeof(servaddr));*/
    servaddr.sin_family = PROTOCOL_FMLY; // seta família de protocolo
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // seta endereço do host na Internet
    servaddr.sin_port = htons(SERV_PORT);	// seta a porta. htons converte para Big Endian

    // Vincula o socket
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	dg_echo(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));

	/*// Ouve o socket criando uma fila de conexão e esperando por clientes
	listen(sockfd, LISTENQ);
    printf("%s\n","Servidor rodando. Esperando conexões.");

	for ( ; ; ) { // loop infinito (enquanto o servidor estiver rodando)

		clilen = sizeof(cliaddr);

        // Aceita uma conexão
        connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);

        printf("%s\n","Request recebido...");

		// Cria filhos para lidar com cada processos
		if ( (childpid = fork ()) == 0 ) { // 0: filho

			printf ("Filho criado para receber request\n");

			close (sockfd); // fecha o socket

			// Recebe a mensagem da conexão
			while ( (recvCode = recv(connfd, strBuffer, MAX_LINE, 0)) > 0)  {

				// Lê os parâmetros
                printf("String recebida do cliente: %s\n", strBuffer);
                char *params = strtok(strBuffer, " ");

				// Processa a requisição do cliente
                process(params);

				// Envia a resposta ao cliente
                send(connfd, resp, strlen(resp), 0);
            }

            if (recvCode < 0) {
            	printf("Erro de leitura\n");
			}
            return 0;
		}

		// Fecha o socket do servidor
        close(connfd);		
	}*/

	return 0;
}
