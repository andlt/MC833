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

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

char banco[9][255];
char buf[MAXLINE];
char ret[MAXLINE];

void process(char *params);
void list(char *params);
void filter(char *params);
void edit();

int main (int argc, char **argv)
{
    int listenfd, connfd, n;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    //Create a socket for the soclet
    //If sockfd<0 there was an error in the creation of the socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    //bind the socket
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen(listenfd, LISTENQ);

    printf("%s\n","Server running...waiting for connections.");

    for ( ; ; ) {

        clilen = sizeof(cliaddr);
        //accept a connection
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);

        printf("%s\n","Received request...");

        if ( (childpid = fork ()) == 0 ) {//if it’s 0, it’s child process

            printf ("%s\n","Child created for dealing with client requests");

            //close listening socket
            close (listenfd);

            while ( (n = recv(connfd, buf, MAXLINE, 0)) > 0)  {
                printf("String received from and resent to the client: %s\n", buf);
                char *params = strtok(buf, " ");
                process(params);
                // printf("processAfter\n");
                // printf("ret:\n%s\n", ret);
                send(connfd, ret, strlen(ret), 0);
            }

            if (n < 0)
            printf("%s\n", "Read error");
            exit(0);
        }
        //close socket of the server
        close(connfd);
    }
}

void process(char *params){
    int valido = 1;

    // TODO: Fazer a validação dos campos passados
    // for (int i = 2; i < strlen(params); i++) {
    //     int index = strtol(&params[i], NULL, 10);
    //     if (index < 0 || index > 8) {
    //         printf("Campo não existe.\n");
    //         valido = 0;
    //         break;
    //     }
    // }
    // if (strcmp(&params[1], "0") != 0 && strcmp(&params[1], "1") != 0 && strcmp(&params[1], "2") != 0) {
    //     printf("Função não existe.\n"); 
    //     valido = 0;
    // }

    if (valido == 1) {
        FILE *pFile, *newFile;
        char c;
        int campo = 0;
        int text = 0;
        int gambi = 0;

        pFile = fopen("bd.txt","r");
        newFile = fopen("bd2.txt","w");

        if (strcmp(&params[0], "2") == 0)
            fprintf(newFile, "ID\tTítulo\tAno de lançamento\tGênero\tDuração (min)\tSinopse\tDiretor\tElenco\tNúmero de exemplares em estoque\n");

        if (pFile == NULL) {
            perror("Error opening file");
        } else {
            do {
                c = fgetc(pFile);
                if (gambi == 0) {
                    if (c == '\n') gambi++;
                } else if (c == '\t') {
                    text = 0;
                    campo++;
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

                    campo = 0;
                    text = 0;
                    memset(banco, 0, sizeof banco);
                } else {
                    banco[campo][text] = c;
                    text++;
                }
            } while (c != EOF);
            fclose(pFile);
            fclose(newFile);
        }

        if (strcmp(&params[0], "2") == 0) {
            rename("bd2.txt", "bd.txt");
            strcat(ret, "Número de exemplares alterado com sucesso.\n");
        } else {
            remove("bd2.txt");
        }
    }
}

/*
- Listar todos os títulos dos filmes e o ano de lançamento;
- Listar todas as informações de todos os filmes;
Parâmetros: 0 [quantidade de parâmetros seguintes] [sequência de campos a serem exibidos]
*/
void list(char *params) {
    int indexInitial = 4;
    for (int i = indexInitial; i < (strtol(&params[2], NULL, 10) * 2) + indexInitial; i += 2) {
        int index = strtol(&params[i], NULL, 10);
        strcat(ret, banco[index]);
        strcat(ret, "\t");
        // printf("%s\t", banco[index]);
    }
    strcat(ret, "\n");
    // printf("\n");
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
        int idFilmeParam = strtol(&params[4], NULL, 10);
        int idFilmeBanco = strtol(banco[0], NULL, 10);
        if (idFilmeParam == idFilmeBanco) {
            int indexInitial = 8;
            for (int i = indexInitial; i < (strtol(&params[6], NULL, 10) * 2) + indexInitial; i += 2) {
                int index = strtol(&params[i], NULL, 10);
                strcat(ret, banco[index]);
                strcat(ret, "\t");
                // printf("%s\t", banco[index]);
            }
            strcat(ret, "\n");
            // printf("\n");
        }
    } else if (campo == 3) { // Filtro pela sinopse
        if (strstr(banco[3], &params[4]) != NULL) { // FIXME: Não tá funfando porque no &params[4] não traz apenas a string, mas também o que vem depois.
			printf("%s", &params[4]);
            int indexInitial = 8;
            for (int i = indexInitial; i < (strtol(&params[6], NULL, 10) * 2) + indexInitial; i += 2) {
                int index = strtol(&params[i], NULL, 10);
                strcat(ret, banco[index]);
                strcat(ret, "\t");
                // printf("%s\t", banco[index]);
            }
            strcat(ret, "\n");
            // printf("\n");
        }
    }
}

/*
- Alterar o número de exemplares em estoque (apenas cliente locadora);
Parâmetros: 2 [ID a ser alterado] [nova quantidade de exemplares em estoque]
*/
void edit(char *params, FILE * newFile) {
    int idFilmeParam = strtol(&params[2], NULL, 10);
    int idFilmeBanco = strtol(banco[0], NULL, 10);
    if (idFilmeParam == idFilmeBanco) {
        fprintf(newFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", banco[0], banco[1], banco[2], banco[3], banco[4], banco[5], banco[6], banco[7], &params[4]);
    } else {
        fprintf(newFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", banco[0], banco[1], banco[2], banco[3], banco[4], banco[5], banco[6], banco[7], banco[8]);
    }
}
