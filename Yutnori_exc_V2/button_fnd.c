#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "buttondrv.h"
#include "fnd_control.h"
#include "textlcd.h"  // attempt 관련 함수

#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"  // struct MyMsgBuf 정의 포함

#define BUTTON_DEVICE "/dev/input/event7"

int selected_fnd_num = -1;  // 선택된 숫자 (기본값 -1)
int selected_mal_idx = -1;

#define BUTTON_CODE_SELECT_MAL1 KEY_SEARCH
#define BUTTON_CODE_SELECT_MAL2 KEY_MENU

// 점(.) 이동
void move_cursor_left() {
    do {
        cursor = (cursor - 1 + 6) % 6;
    } while (fnd_digits[cursor] < 0);
    update_fnd_display();
}

// 선택된 숫자 삭제 및 왼쪽으로 shift
void delete_and_shift() {
    if (fnd_digits[cursor] < 0) return;

    selected_fnd_num = fnd_digits[cursor];

    for (int i = cursor; i > 0; i--) {
        fnd_digits[i] = fnd_digits[i - 1];
    }
    fnd_digits[0] = -1;

    for (int i = cursor; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            cursor = i;
            break;
        }
    }

    update_fnd_display();
}

void run_button_handler_once(int fd) {
    struct input_event ev;

    if (read(fd, &ev, sizeof(ev)) != sizeof(ev)) return;

    printf("EV: type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);

    if (ev.type == EV_KEY && ev.value == 1) {
        if (ev.code == KEY_VOLUMEUP) {
            move_cursor_left();
        }
        else if (ev.code == KEY_BACK) {
            // 현재 커서 위치에 있는 숫자를 가져와 선택
            if (fnd_digits[cursor] >= 0) {
                selected_fnd_num = fnd_digits[cursor];
                delete_and_shift();

                // IPC 메시지 전송
                key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
                if (key == -1) {
                    perror("ftok 실패");
                    return;
                }

                int msqid = msgget(key, IPC_CREAT | 0666);
                if (msqid == -1) {
                    perror("msgget 실패");
                    return;
                }

                struct MyMsgBuf msg;
                msg.mtype = MSG_TYPE_YUT;
                msg.yut_result = selected_fnd_num;

                if (msgsnd(msqid, &msg, sizeof(msg.yut_result), 0) == -1) {
                    perror("msgsnd 실패");
                } else {
                    printf("▶ 선택된 숫자 %d → 메시지 큐 전송 완료\n", selected_fnd_num);
                }

                selected_fnd_num = -1;
            }
        }
        else if (ev.code == KEY_VOLUMEDOWN) {
            increment_attempt();
        }
    }

    else if (ev.code == BUTTON_CODE_SELECT_MAL1 || ev.code == BUTTON_CODE_SELECT_MAL2) {
        if (selected_fnd_num >= 0) {
            selected_mal_idx = (ev.code == BUTTON_CODE_SELECT_MAL1) ? 0 : 1;

            // 메시지 전송
            key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
            if (key == -1) {
                perror("ftok 실패");
                return;
            }

            int msqid = msgget(key, IPC_CREAT | 0666);
            if (msqid == -1) {
                perror("msgget 실패");
                return;
            }

            struct MoveMalMsgBuf msg;
            msg.mtype = MSG_TYPE_MOVE_MAL;
            msg.selected_mal_idx = selected_mal_idx;
            msg.steps_to_move = selected_fnd_num;

            if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                perror("말 이동 msgsnd 실패");
            } else {
                printf("▶ 말 %d → %d칸 이동 메시지 전송 완료\n", selected_mal_idx, selected_fnd_num);
            }

            // 초기화
            selected_fnd_num = -1;
            selected_mal_idx = -1;
        }
    }
}