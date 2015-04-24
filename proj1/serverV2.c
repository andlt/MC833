/*
- Campos: 
    0 - ID
    1 - Título
    2 - Ano de lançamento
    3 - Gênero
    4 - Duração (min)
    5 - Sinopse
    6 - Diretor
    7 - Elenco
    8 - Número de exemplares em estoque
- Funções:
    0 - List;
    1 - Filter;
    2 - Edit;
*/


#include <stdio.h>
#include <netinet/in.h> // sockaddr_in
#include <string.h> // memset()

#define PROTOCOL_FMLY AF_INET // família de protocolo (AF_INET: IPv4)
#define SOCK_TYPE SOCK_STREAM // tipo de socket (stream)
#define SERV_PORT 4000 // porta do servidor
#define LISTENQ 8 // Número máximo de conexões de clientes
#define MAX_LINE 4096 // tamaho máximo de uma linha

char dataBase[9][255];
char strBuffer[MAX_LINE];
char resp[MAX_LINE];

/*
- Listar todos os títulos dos filmes e o ano de lançamento;
- Listar todas as informações de todos os filmes;
Parâmetros: 0 [quantidade de parâmetros seguintes] [sequência de campos a serem exibidos]
*/
void list(char *params) {
    int indexInitial = 4;
	int i;
    for (i = indexInitial; i < (strtol(&params[2], NULL, 10) * 2) + indexInitial; i += 2) {
        int index = strtol(&params[i], NULL, 10);
        strcat(resp, dataBase[index]);
        strcat(resp, "\t");
    }
    strcat(resp, "\n");
}

/*
- Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado;
- Dado o identificador de um filme, retornar a sinopse do filme;
- Dado o identificador de um filme, retornar todas as informações deste filme;
- Dado o identificador de um filme, retornar o número de exemplares em estoque;
Parâmetros: 1 [campo a ser filtrado (ID/Gênero)] [valor do campo a ser filtrado] [quantidade de parâmetros seguintes] [sequência de campos a serem exibidos]
*/
void filter(char *params) {
    int campo = strtol(&params[2], NULL, 10);
    if (campo == 0) { // Filtro pelo ID
        int filmIdParam = strtol(&params[4], NULL, 10);
        int dbFilmId = strtol(dataBase[0], NULL, 10);
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
        while (params[indexParam] != ' ') {
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

/*
- Alterar o número de exemplares em estoque (apenas cliente locadora);
Parâmetros: 2 [ID a ser alterado] [nova quantidade de exemplares em estoque]
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
        int gambi = 0;

		// Abre o arquivo do dataBase e cria um novo
        pFile = fopen("db.txt","r");
        newFile = fopen("db2.txt","w");

		// TODO comment
        if (strcmp(&params[0], "2") == 0)
            fprintf(newFile, "ID\tTítulo\tAno de lançamento\tGênero\tDuração (min)\tSinopse\tDiretor\tElenco\tNúmero de exemplares em estoque\n");

		// TODO comment
        if (pFile == NULL) {
            perror("Error opening file");
        } else {
            do {
                c = fgetc(pFile);
                if (gambi == 0) {
                    if (c == '\n') gambi++;
                } else if (c == '\t') {
                    text = 0;
                    field++;
                } else if (c == '\n') {

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
                    memset(dataBase, 0, sizeof dataBase);
                } else {
                    dataBase[field][text] = c;
                    text++;
                }
            } while (c != EOF);
            fclose(pFile);
            fclose(newFile);
        }

        if (strcmp(&params[0], "2") == 0) {
            rename("db2.txt", "db.txt");
            strcat(resp, "Número de exemplares alterado com sucesso.\n");
        } else {
            remove("db2.txt");
        }
    }
}

int main (int argc, char **argv)
{
	int sockfd = -1; // código de retorno da criação do socket
	int recvCode = -1; // código de retorno da recv()
    struct sockaddr_in servaddr; // estrutura para endereço (IPv4)
    struct sockaddr_in cliaddr; // estrutura para endereço (IPv4)
    socklen_t clilen; // tamamnho do endereço cli
	int connfd; // TODO inicializar
    pid_t childpid; // IDs dos processos filhos

	// Cria o socket
	if ((sockfd = socket (PROTOCOL_FMLY, SOCK_TYPE, 0)) <0) { // se sockfd retornar negativo, houve um erro
        printf("Problema na criação do socket. sockfd retornou %d.\n", sockfd);
        return 2;
    }

	// Prepara o endereço do socket
	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = PROTOCOL_FMLY; // seta família de protocolo
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // seta endereço do host na Internet
    servaddr.sin_port = htons(SERV_PORT);	// seta a porta. htons converte para Big Endian

    // Vincula o socket
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	// Ouve o socket criando uma fila de conexão e esperando por clientes
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
	}

	return 0;
}
