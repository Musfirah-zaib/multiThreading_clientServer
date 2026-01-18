#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void reverse(char str[]) {
    int i, j;
    char temp;
    for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void* handle_client(void* arg) {
    int client_sock = *(int*)arg;
    char buffer[1024];

    recv(client_sock, buffer, sizeof(buffer), 0);
    reverse(buffer);
    send(client_sock, buffer, strlen(buffer), 0);

    close(client_sock);
    pthread_exit(NULL);
}

int main() {
    int sockfd, client_sock;
    struct sockaddr_in server, client;
    socklen_t len;
    pthread_t tid;

    // 1. Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Initialize server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(sockfd, (struct sockaddr*)&server, sizeof(server));

    // 4. Listen
    listen(sockfd, 5);
    printf("Multi-threaded TCP Server waiting...\n");

    while (1) {
        len = sizeof(client);
        client_sock = accept(sockfd, (struct sockaddr*)&client, &len);

        // 5. Create thread
        pthread_create(&tid, NULL, handle_client, &client_sock);
        pthread_detach(tid);   // avoid zombie threads
    }

    close(sockfd);
    return 0;
}
