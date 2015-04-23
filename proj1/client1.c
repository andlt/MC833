#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
	struct timeval *tv;
	int startTimeInSeconds;
	int startTimeInMicroSeconds;
	int endTimeInSeconds;
	int endTimeInMicroSeconds;

    //basic check of the arguments
    //additional checks can be inserted
    if (argc !=2) {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order

    gettimeofday(tv, NULL);
	startTimeInSeconds = tv->tv_sec;
	startTimeInMicroSeconds = tv->tv_usec;
	printf("StartTime: %d, %d\n", startTimeInSeconds, startTimeInMicroSeconds);

    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    // while (fgets(sendline, MAXLINE, stdin) != NULL) {

    sprintf(sendline, "0 2 1 2\n"); // Listar todos os títulos dos filmes e o ano de lançamento.
    send(sockfd, sendline, strlen(sendline), 0);

    if (recv(sockfd, recvline, MAXLINE,0) == 0){
        //error: server terminated prematurely
        perror("The server terminated prematurely");
        exit(4);
    }
    // printf("%s", "String received from the server: ");
    fputs(recvline, stdout);

    // }

    gettimeofday(tv, NULL);
	endTimeInSeconds = tv->tv_sec;
	endTimeInMicroSeconds = tv->tv_usec;
	printf("EndTime: %d, %d\n", endTimeInSeconds, endTimeInMicroSeconds);

	printf("Diference: %d\n", endTimeInMicroSeconds - startTimeInMicroSeconds);
	FILE *rFile;
    rFile = fopen("results.txt","w");
	if (rFile == NULL) {
		perror("Error opening file");
	} else {
		fprintf(rFile, "%d\n", endTimeInMicroSeconds - startTimeInMicroSeconds);
	}

    exit(0);
}
