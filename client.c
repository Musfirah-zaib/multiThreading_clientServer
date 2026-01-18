#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    printf("Enter string: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    send(sockfd, buffer, strlen(buffer), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);

    printf("Result: %s\n", buffer);
    close(sockfd);
    return 0;
}
