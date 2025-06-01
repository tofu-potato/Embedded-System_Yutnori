#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "AccelMagGyro.h"
#include "Yut_logic.h"

const char* get_yut_result(void) {
    int accel1, accel2, accel3;
    int accel_avg;
    int max_avg;

    Accel(&accel1, &accel2, &accel3);
    accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;

    if (accel_avg < 100)
        return NULL;

    max_avg = accel_avg;
    for (int i = 0; i < 10; i++) {
        Accel(&accel1, &accel2, &accel3);
        accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;
        if (accel_avg > max_avg)
            max_avg = accel_avg;
        usleep(100000);
    }

    srand(max_avg);
    int r = rand() % 1000;

    if (r < 53) return "빽도";
    else if (r < 211) return "도";
    else if (r < 581) return "개";
    else if (r < 871) return "걸";
    else if (r < 956) return "윷";
    else return "모";
}
