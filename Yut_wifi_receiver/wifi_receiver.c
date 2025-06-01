#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 9999
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 모든 IP에서 수신
    server_addr.sin_port = htons(SERVER_PORT);

    // 3. 바인드
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. 리슨 (연결 대기)
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("서버 대기 중... 포트 %d\n", SERVER_PORT);

    while (1) {
        // 5. 클라이언트 연결 수락
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // 6. 데이터 수신
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("▶ 수신된 윷 결과: %s\n", buffer);
        }

        // 7. 클라이언트 소켓 닫기
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
