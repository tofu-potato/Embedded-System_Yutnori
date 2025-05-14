#pragma once

#include "Variable.h"
#include <time.h>

// Yut No Ri Board
/*

10*			9			8			7			6			5*


		21											20
11															4

				23							22

12															3

							   24*

13															2

				26							25

14															1

		28											27

15*			16			17			18			19			0
*/



void init_Player(Players* p) {	// resets the player status
	p->Horse = 0x44; // Computer = 1, Player = 0		
	p->TimeOver= 0;
	p->Yoot= 0; 
	p->Ate= 0;
	p->Stack= 0;
	p->DataReceived= 0;
	p->Data= 0;
	p->Location[8]= 0;
	p->Options[20]= 0;


	p->Throw_Again= 0;

	p->shortCut= 0;
	p->backDo= 0;
}
void Role(Players* p) {		// throws yoot --> random value 
	
}		

void Move(Players* p) {		// player move the figure

}