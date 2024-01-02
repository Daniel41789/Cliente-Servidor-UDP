#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 1024
#define PORT 12345

void process_command(char *command, char *response)
{
    char cmd, *data;
    int nums[5];
    int sum;

    if (sscanf(command, "<SYN>%c,%d,%d,%d,%d,%d<STX>", &cmd, &nums[0], &nums[1], &nums[2], &nums[3], &nums[4]) == 6)
    {
        switch (cmd)
        {
        case 'A':
            // Ordenar ascendentemente
            
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4 - i; j++)
                {
                    if (nums[j] > nums[j + 1])
                    {
                        int temp = nums[j];
                        nums[j] = nums[j + 1];
                        nums[j + 1] = temp;
                    }
                }
            }
            sprintf(response, "(%d,%d,%d,%d,%d),<ACK>,<SYN>%c,DAT<STX>", nums[0], nums[1], nums[2], nums[3], nums[4], cmd);
            break;

        case 'B':
            // Ordenar descendentemente
            
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4 - i; j++)
                {
                    if (nums[j] < nums[j + 1])
                    {
                        int temp = nums[j];
                        nums[j] = nums[j + 1];
                        nums[j + 1] = temp;
                    }
                }
            }
            sprintf(response, "(%d,%d,%d,%d,%d),<ACK>,<SYN>%c,DAT<STX>", nums[0], nums[1], nums[2], nums[3], nums[4], cmd);
            break;

        case 'C':
            // Sumar los números
            
            sum = nums[0] + nums[1] + nums[2] + nums[3] + nums[4];
            sprintf(response, "(%d),<ACK>,<SYN>%c,DAT<STX>", sum, cmd);
            break;

        case 'D':
            // Retardo de mensaje
            sleep(3);
            sprintf(response, "Retardo de mensaje,<ACK>,<SYN>%c,DAT<STX>", cmd);
            break;

        default:
            // Comando no reconocido
            sprintf(response, "Comando no reconocido,<NAK>,%s", command);
        }
    }
    else
    {
        // Error en el formato del mensaje
        sprintf(response, "Error en el formato del mensaje,<NAK>,%s", command);
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    char response[MAXLINE];
    socklen_t len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);

    while (1)
    {
        recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[strlen(buffer)] = '\0';

        printf("Client: %s\n", buffer);

        process_command(buffer, response);

        sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Response sent.\n");
    }

    close(sockfd);
    return 0;
}
