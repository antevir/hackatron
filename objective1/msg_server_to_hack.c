
#include <stdio.h>
#include <stdbool.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <unistd.h> // read(), write(), close()

#define PORT 8080
#define SA struct sockaddr

static void set_so_linger(int socket)
{
    int z; /* Status code */
    struct linger so_linger;
    so_linger.l_onoff = 0;
    so_linger.l_linger = 00;
    z = setsockopt(socket,
                SOL_SOCKET,
                SO_LINGER,
                &so_linger,
                sizeof so_linger);
    if ( z ) {
       perror("setsockopt(2)");
    }
}

void client_handler(int connfd)
{
    char message[] = "Hello world!";
    char rxBuffer[16];

    do {
        ssize_t len = read(connfd, rxBuffer, 32);
        if (len <= 0) {
            break;
        }
        rxBuffer[len] = 0;

        size_t msgLen = strlen(message);
        if (msgLen > 0) {
            write(connfd, message, strlen(message));
        }
    } while (strcmp("exit", rxBuffer) != 0);

    close(connfd);
}

// Entrypoint
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    const int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded..\n");
    }

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening..\n");
    }
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    } else {
        printf("server accept the client...\n");
    }

    set_so_linger(connfd);

    // Function that handles communication between client and server
    client_handler(connfd);

    // Close socket when we're done
    close(sockfd);
}
