#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// IPC 관련 헤더 추가
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"
#include "defvar.h"
#include "key_codes.h"

// 기존 디바이스 헤더
#include "fnd_control.h"
#include "button_fnd.h"
#include "textlcd.h"


// 메시지 구조체 및 키 정의 (main.c에 직접 추가하거나, 별도의 IPC_MSG.h 포함)
//struct my_msgbuf {
//    long mtype;
//    int yut_result;
//};

//#define MSG_KEY_PATH "/tmp"
//#define MSG_KEY_ID 'A'
//#define MSG_TYPE_YUT 1

#define BUTTON_DEVICE "/dev/input/event7"
#define SERVER_PORT 9999

//extern int attempt; // textlcd.c에 정의된 전역 변수


// FND에서 선택된 숫자 (말 이동 칸 수)를 저장할 변수
static int selected_fnd_num = -1;

// FND에 숫자가 남아있는지 확인하는 함수 (fnd_control.c의 fnd_digits 배열 활용)
//extern int fnd_digits[6]; // fnd_control.c에서 extern으로 가져옴

static int is_fnd_empty() {
    for (int i = 0; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            return 0; // 숫자가 하나라도 있으면 비어있지 않음
        }
    }
    return 1; // 모든 자리가 비어 있음
}

int main() {
    int button_fd = -1;
    int server_fd = -1;
    int client_fd = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    key_t msg_queue_key; // 메시지 큐 키
    int msg_queue_id;    // 메시지 큐 ID

    printf("▶ 시스템 초기화 중...\n");

    // FND 및 LCD 초기화
    init_fnd_digits();
    update_fnd_display();
    update_textlcd_display();
    curP = P1;

    // ----------------------------------------------------
    // 메시지 큐 생성 또는 가져오기
    // ftok() 함수는 경로와 ID를 기반으로 고유한 키를 생성합니다.
    msg_queue_key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
    if (msg_queue_key == -1) {
        perror("ftok 실패");
        goto cleanup;
    }

    // msgget() 함수로 메시지 큐를 얻습니다. IPC_CREAT | 0666은 없으면 생성하고, 접근 권한을 0666으로 설정합니다.
    msg_queue_id = msgget(msg_queue_key, IPC_CREAT | 0666);
    if (msg_queue_id == -1) {
        perror("msgget 실패");
        goto cleanup;
    }
    printf("▶ 메시지 큐 (ID: %d) 생성/연결 완료.\n", msg_queue_id);
    // ----------------------------------------------------

    // 버튼 디바이스 열기
    button_fd = open(BUTTON_DEVICE, O_RDONLY | O_NONBLOCK);
    if (button_fd < 0) {
        perror("버튼 디바이스 열기 실패");
        goto cleanup;
    }

    // WiFi 수신 소켓 초기화
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket 생성 실패");
        goto cleanup;
    }

    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt(SO_REUSEADDR) 실패");
        goto cleanup;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind 실패");
        goto cleanup;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen 실패");
        goto cleanup;
    }

    printf("▶ 시스템 실행 중 - WiFi 수신 대기 (포트 %d)...\n", SERVER_PORT);

    while (1) {
        // 버튼 이벤트 처리 (이전 코드와 동일)
        struct input_event ev;
        if (read(button_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_KEY && ev.value == 1) {
                if (ev.code == KEY_VOLUMEUP) {
                    move_cursor_left();
                } else if (ev.code == KEY_BACK) {
                    if (fnd_digits[cursor] >= 0) { // 현재 커서 위치에 숫자가 있다면
                        selected_fnd_num = fnd_digits[cursor]; // 선택된 숫자를 저장
                        printf("▶ FND에서 %d 선택됨 (이동 칸 수).\n", selected_fnd_num);
                        delete_and_shift(); // FND에서 해당 숫자 삭제 및 시프트
                    } else {
                        printf("▶ 커서 위치에 숫자가 없습니다.\n");
                    }
                }
                else if (ev.code == KEY_PROG1) { // 버튼 3 (말 1 선택)
                    // 말 이동 IPC 메시지 전송 시도
                    if (selected_fnd_num != -1) {
                        int mal_to_move = (curP == P1 ? P11 : P21); // 현재 플레이어의 첫 번째 말
                        printf("▶ 말 %d (플레이어 %d의 말1) 선택됨. %d 칸 이동 요청.\n", mal_to_move, (curP == P1 ? 1 : 2), selected_fnd_num);
                        
                        struct MoveMalMsgBuf msg_to_send;
                        msg_to_send.mtype = MSG_TYPE_MOVE_MAL;
                        msg_to_send.selected_mal_idx = mal_to_move;
                        msg_to_send.steps_to_move = selected_fnd_num;

                        if (msgsnd(msg_queue_id, &msg_to_send, sizeof(struct MoveMalMsgBuf) - sizeof(long), IPC_NOWAIT) == -1) {
                            perror("msgsnd (말 이동) 실패");
                        } else {
                            printf("▶ IPC 메시지 (말 이동: 말%d, %d칸) 전송 성공.\n", mal_to_move, selected_fnd_num);
                            selected_fnd_num = -1; // 사용했으니 초기화
                            // 말 이동 요청 후 FND에 아직 남은 숫자가 있다면 계속 SELECT 모드 유지
                            // 아니면 다음 윷 결과 대기 또는 턴 종료 로직으로
                        }
                    } else {
                        printf("▶ 먼저 FND에서 이동할 칸 수를 선택해주세요.\n");
                    }
                }
                else if (ev.code == KEY_PROG2) { // 버튼 4 (말 2 선택)
                    // 말 이동 IPC 메시지 전송 시도
                    if (selected_fnd_num != -1) {
                        int mal_to_move = (curP == P1 ? P12 : P22); // 현재 플레이어의 두 번째 말
                        printf("▶ 말 %d (플레이어 %d의 말2) 선택됨. %d 칸 이동 요청.\n", mal_to_move, (curP == P1 ? 1 : 2), selected_fnd_num);

                        struct MoveMalMsgBuf msg_to_send;
                        msg_to_send.mtype = MSG_TYPE_MOVE_MAL;
                        msg_to_send.selected_mal_idx = mal_to_move;
                        msg_to_send.steps_to_move = selected_fnd_num;

                        if (msgsnd(msg_queue_id, &msg_to_send, sizeof(struct MoveMalMsgBuf) - sizeof(long), IPC_NOWAIT) == -1) {
                            perror("msgsnd (말 이동) 실패");
                        } else {
                            printf("▶ IPC 메시지 (말 이동: 말%d, %d칸) 전송 성공.\n", mal_to_move, selected_fnd_num);
                            selected_fnd_num = -1; // 사용했으니 초기화
                            // 말 이동 요청 후 FND에 아직 남은 숫자가 있다면 계속 SELECT 모드 유지
                            // 아니면 다음 윷 결과 대기 또는 턴 종료 로직으로
                        }
                    } else {
                        printf("▶ 먼저 FND에서 이동할 칸 수를 선택해주세요.\n");
                    }
                }
                else if (ev.code == KEY_VOLUMEDOWN) { // 버튼 5 (attempt 증가)
                    increment_attempt();
                }
            }
        }

        // WiFi 수신 처리 (논블로킹)
        struct timeval timeout = {0, 100000}; // 100ms
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        int max_fd = server_fd;
        
        int sel_ret = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

        if (sel_ret < 0) {
            printf("check1");
            perror("select 실패");
            goto cleanup;
        } else if (sel_ret > 0 && FD_ISSET(server_fd, &readfds)) {
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            printf("check2");
            if (client_fd < 0) {
                perror("accept 실패");
                printf("check3");
                goto cleanup;
            }

            printf("▶ 클라이언트 (%s:%d) 연결됨\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            int net_result;
            int recv_len = recv(client_fd, &net_result, sizeof(net_result), 0);
            
            if (recv_len == sizeof(net_result)) {
                int received_yut_result = ntohl(net_result); // WiFi로 수신한 최종 윷 결과

                if (attempt > 0) {
                    if (received_yut_result == 1 || received_yut_result == 2 || received_yut_result == 3 ||
                        received_yut_result == 4 || received_yut_result == 5 || received_yut_result == 8) {
                        
                        // ----------------------------------------------------
                        // 수신된 윷 결과를 메시지 큐로 전송 (msgsnd)
                         // struct my_msgbuf msg_to_send;
                         struct MyMsgBuf msg_to_send_yut; // 변수명 변경
                        //msg_to_send.mtype = MSG_TYPE_YUT;        // 정의한 메시지 타입
                        //msg_to_send.yut_result = received_yut_result; // 수신한 윷 결과 값
                        msg_to_send_yut.mtype = MSG_TYPE_YUT;
                        msg_to_send_yut.yut_result = received_yut_result;


                        // msgsnd(메시지 큐 ID, 메시지 구조체 포인터, 메시지 데이터 크기, 플래그)
                        //if (msgsnd(msg_queue_id, &msg_to_send, sizeof(struct my_msgbuf) - sizeof(long), IPC_NOWAIT) == -1) {
                         if (msgsnd(msg_queue_id, &msg_to_send_yut, sizeof(struct MyMsgBuf) - sizeof(long), IPC_NOWAIT) == -1) {
                            perror("msgsnd 실패 (메시지 큐 가득 참 또는 다른 오류)");
                            // 메시지 큐 전송 실패 시에도 시스템 종료 여부는 판단 필요
                            // 여기서는 일단 에러만 출력하고 계속 진행합니다.
                            // 만약 IPC 통신이 필수적이라면, goto cleanup; 할 수 있습니다.
                        } else {
                            printf("▶ 메시지 큐에 윷 결과 (%d) 전송 성공.\n", received_yut_result);
                        }
                        // ----------------------------------------------------

                        insert_fnd_number(received_yut_result); // FND에 표시

                        if (received_yut_result == 4 || received_yut_result == 5)
                        {
                            decrement_attempt();
                            increment_attempt();
                        }
                        else
                            decrement_attempt();
                    } else {
                        fprintf(stderr, "▶ 유효하지 않은 윷 결과 값 수신: %d\n", received_yut_result);
                    }
                } else {
                    printf("▶ attempt 0: 수신된 값 무시됨 (%d)\n", received_yut_result);
                }
            } else if (recv_len == 0) {
                printf("▶ 클라이언트 (%s:%d) 연결 종료.\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                goto cleanup;
            } else {
                perror("recv 오류 또는 불완전한 데이터 수신");
                goto cleanup;
            }
            close(client_fd);
            client_fd = -1;
        }

        //check_and_toggle_player();
        // 턴 전환 로직 유지 (attempt 0이고 FND에 숫자가 없으면)
        if (attempt == 0 && is_fnd_empty()) {
            printf("▶ 턴 종료: attempt 0 및 FND 비어있음. 플레이어 전환.\n");
            curP = (curP == P1) ? P2 : P1; // 플레이어 전환
            printf("▶ 현재 플레이어: P%d\n", (curP == P1 ? 1 : 2));
            // 필요하다면 이곳에서 Qt GUI에 플레이어 전환 메시지를 보낼 수도 있음
            // update_textlcd_display(); // LCD 갱신 (선택 사항, textlcd.c에서 플레이어 표시하도록 구현되어 있다면)
            // 다시 attempt 초기화 등 필요한 초기화 수행 (rollyut() 함수에서 초기화하는 경우 이 부분은 생략)
        }
        usleep(10000);
    }

cleanup:
    printf("▶ 시스템 종료 중...\n");
    if (client_fd != -1) close(client_fd);
    if (server_fd != -1) close(server_fd);
    if (button_fd != -1) close(button_fd);

    // 메시지 큐 제거 (선택 사항이지만, 깔끔한 종료를 위해 권장)
    // 이 프로세스가 메시지 큐를 생성한 유일한 프로세스이고, 더 이상 사용되지 않을 때 제거합니다.
    // 다른 프로세스에서 메시지 큐를 계속 사용해야 한다면 제거하지 마세요.
    if (msg_queue_id != -1) {
        if (msgctl(msg_queue_id, IPC_RMID, NULL) == -1) {
            perror("메시지 큐 제거 실패");
        } else {
            printf("▶ 메시지 큐 (ID: %d) 제거 완료.\n", msg_queue_id);
        }
    }

    return 1;
}