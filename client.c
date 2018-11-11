#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// sudo netstat -tnlp | grep :8002, to check if the server is listening at port 8002.
// ifconfig to find ipaddr.

void error(char* msg) {
    perror(msg);
    exit(0);
}

// argc is argument count (automatic).

int main(int argc, char* argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]); // atoi() converts from string to integer.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Note that ./client counts as an argument.
    printf("%d\n", argc);
    printf("%s\n", argv[0]);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    // server = gethostbyname(argv[1]);
    struct in_addr ipv4addr;
    inet_pton(AF_INET, argv[1], &ipv4addr);
    server = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    printf("Please enter the message: ");
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);
    int i;
    char *letter;
    unsigned usecs = 1000000;
    for (i=0; i<strlen(buffer); i++) {
        *letter = buffer[i];
        n = write(sockfd, letter, 1);
        fflush(stdout);
        usleep(usecs);
    }

    if (n < 0) {
        error("ERROR writing to socket");
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 256);

    printf("Here is the message: %s\n", buffer);

    if (n < 0) {
        error("ERROR reading from socket");
    }

    printf("%s\n", buffer);

    return 0;
}