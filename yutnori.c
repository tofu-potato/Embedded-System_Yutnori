//lab5.c -- Main program

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h> 
#include "defVar.h"
#include "initialize.h"
#include "timerISR.h"
#include "toss.h"
#include "select.h"
#include "move.h"
#include "ai.h"
#include "blink.h"
#include "locate.h"
#include "moveMal.h"
#include "moveResult.h"


//**********************************************************       

int main(void) {
	initialize();
	while (1) {
		blink();

		if (toss_delay == ON) {
			while (time4 > 0) {		//1500 ms delay
				blink();
			}
			toss_delay = OFF;
		}

		//Enter TOSS mode
		if (playMode == TOSS) {
			toss();
		}
		if (sel_delay == ON) {		//500 ms delay
			while (time5 > 0) {
				blink();
			}
			sel_delay = OFF;
		}

		//Enter SELECT mode
		if (playMode == SELECT) {
			select();
		}
		if (move_delay == ON) {		//500 ms delay
			while (time5 > 0) {
				blink();
			}
			move_delay = OFF;
			time6 = t6;
		}

		//Enter MOVE mode
		if (playMode == MOVE) {
			move();
		}
	}

}