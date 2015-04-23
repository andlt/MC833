#include <stdio.h>
#include <netinet/in.h> // sockaddr_in
#include <string.h> // memset()
// TODO: checar se realmente não é necessário importar #include <sys/time.h>

#define PROTOCOL_FMLY AF_INET // família de protocolo (AF_INET: IPv4) // TODO: trocar AF_INET
#define SOCK_TYPE SOCK_STREAM // tipo de socket (stream) // TODO

#define SERV_PORT 3000 // porta do servidor // TODO trocar para outra porta
#define MAX_LINE 4096 // tamaho máximo de uma linha

#define RESULTS_FILE "results.txt"

#define PRINTS_ON 1 // 1 para printar o menu, 0 para desliga-lo

void getCurrentTime (int* seconds, int* microSeconds)
{
	/* Guarda em seconds e microSeconds os tempos decorridos em segundos e microsegundos desde o Epoch 1970-01-01 00:00:00 UTC */
	struct timeval *tv;
	gettimeofday(tv, NULL);
	*seconds = tv->tv_sec; // guarda os segundos desde o Epoch (1970)
	*microSeconds = tv->tv_usec; // guarda os microsegundos desde o Epoch (1970)
	//printf("EndTime: %d, %d\n", endTimeInSeconds, endTimeInMicroSeconds);
}

void saveConnectionTimes (int startMS, int endMS)
{
	/* Salva a diferença entre startMS e endMS (ambos em microsegundos) no arquivo RESULTS_FILE */
	FILE *rFile;
    rFile = fopen(RESULTS_FILE,"w");
	if (rFile == NULL) {
		perror("Error opening file");
	} else {
		fprintf(rFile, "%d\n", endMS - startMS);
	}
}

int main(int argc, char **argv)
{
	/* A função deve ser chamada recebendo o IP do servidor como argumento */

	int sockfd = -1; // código de retorno da criação do socket
    struct sockaddr_in servaddr; // estrutura para endereço (IPv4)
	struct timeval *tv;
	int startTimeInSeconds; // TODO: inicializar tempos com -1
	int startTimeInMicroSeconds;
	int endTimeInSeconds;
	int endTimeInMicroSeconds;
    char sendline[MAX_LINE], recvline[MAX_LINE];
	int menuOption = -1;

	if (argc != 2) { // confere se recebemos extamente 1 argumento
		printf("Número de argumentos incorreto. %d argumentos recebidos.\n", argc-1);
		return 1;		
	}

	// TODO: checar se argumento recebido é um endereço de IP

	// Cria o socket (AF_INET: IPv4; SOCK_STREAM: stream socket)
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) { // se sockfd retornar negativo, houve um erro
        printf("Problema na criação do socket. sockfd retornou %d.\n", sockfd);
        return 2;
    }

	// Inicializa o servaddr
	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // seta família de protocolo
    servaddr.sin_addr.s_addr= inet_addr(argv[1]); // seta endereço do host na Internet. inet_addr converte do formato padrão para binário
    servaddr.sin_port = htons(SERV_PORT);	// seta a porta. htons converte para Big Endian

	// TODO: Requisitar uma lista de serviços disponíveis noservidor, e imprimi-la
	// Solicita ao usuário que entre com a requisição
	// TODO: modificar opções do menu para letras
	if (PRINTS_ON == 1) {
		printf("Selecione uma opção: \n\n");
		printf("0 - Listar todos os títulos dos filmes e o ano de lançamento \n");
		printf("1 - Listar todas as informações de todos os filmes \n");
		printf("2 - Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado \n");
		printf("3 - Dado o identificador de um filme, retornar a sinopse do filme \n");
		printf("4 - Dado o identificador de um filme, retornar todas as informações deste filme \n");
		printf("5 - Dado o identificador de um filme, retornar o número de exemplares em estoque \n");
		printf("6 - Alterar o número de exemplares em estoque (apenas cliente locadora) \n");
		printf("9 - Sair \n");
	}

	scanf("%d", &menuOption);
	// Prepara a requisição a ser enviada ao servidor de acordo com a opção selecionada
	switch (menuOption){
		case 0:
			if (PRINTS_ON == 1)
				printf("Opção 0 - 'Listar todos os títulos dos filmes e o ano de lançamento' selecionada.\n");
			sprintf(sendline, "0 2 1 2\n"); // Listar todos os títulos dos filmes e o ano de lançamento.
			break;
		case 1:
			if (PRINTS_ON == 1)
				printf("Opção 1 - 'Listar todas as informações de todos os filmes' selecionada.\n");
			sprintf(sendline, "0 9 0 1 2 3 4 5 6 7 8"); // Listar todas as informações de todos os filmes.
			break;
		case 2:
			if (PRINTS_ON == 1)
				printf("Opção 2 - 'Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado' selecionada.\n");
			sprintf(sendline, "1 3 Drama 2 1 2"); // Listar todos os títulos dos filmes e o ano de lançamento de um gênero determinado.
			break;
		case 3:
			if (PRINTS_ON == 1)
				printf("Opção 3 - 'Dado o identificador de um filme, retornar a sinopse do filme' selecionada.\n");
			sprintf(sendline, "1 0 2 1 5"); // Dado o identificador de um filme, retornar a sinopse do filme.
			break;
		case 4:
			if (PRINTS_ON == 1)
				printf("Opção 4 - 'Dado o identificador de um filme, retornar todas as informações deste filme' selecionada.\n");
			sprintf(sendline, "1 0 2 9 0 1 2 3 4 5 6 7 8"); // Dado o identificador de um filme, retornar todas as informações deste filme.
			break;
		case 5:
			if (PRINTS_ON == 1)
				printf("Opção 5 - 'Dado o identificador de um filme, retornar o número de exemplares em estoque' selecionada.\n");
			sprintf(sendline, "1 0 2 1 8"); // Dado o identificador de um filme, retornar o número de exemplares em estoque.
			break;
		case 6:
			if (PRINTS_ON == 1)
				printf("Opção 6 - 'Alterar o número de exemplares em estoque (apenas cliente locadora)' selecionada.\n");
			sprintf(sendline, "2 3 15"); // Alterar o número de exemplares em estoque (apenas cliente locadora)
			break;
		case 9:
			if (PRINTS_ON == 1)
				printf("Opção 9 - 'Sair' selecionada.\n");
			return 0;
		default:
			printf("Erro: %d selecionado. Essa opção não existe!\n", menuOption);
			return 3;
	}

	// Guarda o tempo de abertura da conexão // TODO: substituir por getCurrentTime()
	gettimeofday(tv, NULL);
	startTimeInSeconds = tv->tv_sec; // guarda os segundos desde o Epoch (1970)
	startTimeInMicroSeconds = tv->tv_usec; // guarda os microsegundos desde o Epoch (1970)
	// printf("StartTime: %d, %d\n", startTimeInSeconds, startTimeInMicroSeconds);
	// getCurrentTime(&startTimeInSeconds, &startTimeInMicroSeconds);

	// Conecta o cliente ao socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) { // se o conect retornar negativo, ocorreu erro na conexão
        return 4;
        printf("Problema na conexão com o servidor.");
    }

	// Envia a requisição ao servidor
	send(sockfd, sendline, strlen(sendline), 0);

	// Recebe a mensagem do socket
	if (recv(sockfd, recvline, MAX_LINE, 0) == 0){ // se recv retornar 0, ocorreu na recepção
        printf("O servidor terminou prematuramente");
        return 5;
    }
	// string recebida do socket
    fputs(recvline, stdout);
	
	// Guarda o tempo de fechamento da conexão // TODO: substituir por getCurrentTime()
	gettimeofday(tv, NULL);
	endTimeInSeconds = tv->tv_sec;
	endTimeInMicroSeconds = tv->tv_usec;
	//printf("EndTime: %d, %d\n", endTimeInSeconds, endTimeInMicroSeconds);

	printf("Diference: %d\n", endTimeInMicroSeconds - startTimeInMicroSeconds);

	// Salva os tempos de conexão
	saveConnectionTimes(endTimeInMicroSeconds, startTimeInMicroSeconds);

    return 0;
}
