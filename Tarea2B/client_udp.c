#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 12345

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    char message[MAXLINE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {
        printf("Enter message in the format <SYN>CMD,N1,N2,N3,N4,N5<STX>: ");
        fgets(message, sizeof(message), stdin);

        sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("Message sent.\n");

        recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, sizeof(servaddr));
        buffer[strlen(buffer)] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
