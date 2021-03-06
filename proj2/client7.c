#include <stdio.h>
#include <netinet/in.h> // sockaddr_in
#include <string.h> // memset()
#include <arpa/inet.h> // memset()
#include <sys/time.h> // gettimeofday()
#include <strings.h> // bzero()

#define PROTOCOL_FMLY AF_INET // família de protocolo (AF_INET: IPv4)
#define SOCK_TYPE SOCK_DGRAM // tipo de socket (datagram)
#define SERV_PORT 4000 // porta do servidor
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
    rFile = fopen(RESULTS_FILE,"a");
	if (rFile == NULL) {
		perror("Error opening file");
	} else {
		fprintf(rFile, "%d\n", endMS - startMS);
	}
}

int main(int argc, char **argv)
{
	/*/* A função deve ser chamada recebendo o IP do servidor como argumento */

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

	// Zera o buffer
	bzero(&servaddr, sizeof(servaddr));

	// TODO: checar se argumento recebido é um endereço de IP

	// Cria o socket
	if ((sockfd = socket (PROTOCOL_FMLY, SOCK_TYPE, 0)) < 0) { // se sockfd retornar negativo, houve um erro
        printf("Problema na criação do socket. sockfd retornou %d.\n", sockfd);
        return 2;
    }

	// Prepara o endereço do socket
	/*memset(&servaddr, 0, sizeof(servaddr));*/
    servaddr.sin_family = PROTOCOL_FMLY; // seta família de protocolo
    /*servaddr.sin_addr.s_addr= inet_addr(argv[1]); // seta endereço do host na Internet. inet_addr converte do formato padrão para binário*/
    servaddr.sin_port = htons(SERV_PORT);	// seta a porta. htons converte para Big Endian
	inet_pton(PROTOCOL_FMLY, argv[1], &servaddr.sin_addr);

	sprintf(sendline, "2 3 15"); // Alterar o número de exemplares em estoque (apenas cliente locadora)

	// Guarda o tempo de abertura da conexão // TODO: substituir por getCurrentTime()
	gettimeofday(tv, NULL);
	startTimeInSeconds = tv->tv_sec; // guarda os segundos desde o Epoch (1970)
	startTimeInMicroSeconds = tv->tv_usec; // guarda os microsegundos desde o Epoch (1970)
	// printf("StartTime: %d, %d\n", startTimeInSeconds, startTimeInMicroSeconds);
	// getCurrentTime(&startTimeInSeconds, &startTimeInMicroSeconds);*/

	// Envia a requisição ao servidor
	sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));

	// Recebe a mensagem do socket
	if (recvfrom(sockfd, recvline, MAX_LINE, 0, NULL, NULL) == 0){ // se recv retornar 0, ocorreu erro na recepção
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
	saveConnectionTimes(startTimeInMicroSeconds, endTimeInMicroSeconds);

    return 0;
}
