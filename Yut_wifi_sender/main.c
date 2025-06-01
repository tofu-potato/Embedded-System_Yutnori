#include <stdio.h>
#include <unistd.h>
#include "Yut_logic.h"
#include "wifi_sender.h"

int main(void) {
    while (1) {
        const char* result = get_yut_result();
        if (result != NULL) {
            printf("윷 결과: %s\n", result);
            send_result_over_wifi(result);
            usleep(500000);
        }
        usleep(100000);
    }

    return 0;
}
