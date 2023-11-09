#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <unistd.h> // read(), write(), close()
#define MAX_STRING_LENGTH 80
#define PORT 8080
#define SA struct sockaddr

static const char *getStringFromUser()
{
    static char buf[MAX_STRING_LENGTH + 1];

    bzero(buf, sizeof(buf));
    printf("Enter the string : ");
    for (int i = 0; i < MAX_STRING_LENGTH; i++) {
        char ch = getchar();
        if (ch == '\n') {
            break;
        }
        buf[i] = ch;
    }

    return buf;
}

static ssize_t readTcpData(int sockfd, char *pBuf, size_t len)
{
    ssize_t ret;
    bzero(pBuf, len);

    ret = read(sockfd, pBuf, len - 1);
    if (ret < 0) {
        if (errno != EWOULDBLOCK) {
            printf("Read failed: %s\n", strerror(errno));
            exit(0);
        } else {
            printf("No data received\n");
            ret = 0;
        }
    } else if (ret == 0) {
        printf("Connection closed\n");
        exit(0);
    }

    return ret;
}

void client(int sockfd)
{
    for (;;) {
        const char *pString = getStringFromUser();
        size_t strLen = strlen(pString);

        if (strLen == 0) {
            continue;
        }

        ssize_t ret = write(sockfd, pString, strLen);
        if (ret <= 0) {
            break;
        }

        char rxBuffer[64];
        ret = readTcpData(sockfd, rxBuffer, sizeof(rxBuffer));
        if (ret > 0) {
            printf("From Server : \"%s\"\n", rxBuffer);
        }
    }
}

int main()
{
    int sockfd, connfd;
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

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    } else {
        printf("connected to the server..\n");
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &(int){1}, sizeof(int));

    // function for chat
    client(sockfd);

    // close the socket
    close(sockfd);
}