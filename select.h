//select.h -- Enables the player/AI to select which mal to move

void select(void) {
	//End turn if both home for BACKDO
	if (curP == P1 && result == BACKDO && loc[P11] == 0 && loc[P12] == 0) {
		endTurn();
	}
	else if (curP == P2 && result == BACKDO && loc[P21] == 0 && loc[P22] == 0) {
		endTurn();
	}
	//End turn if one home and one score for BACKDO
	else if (curP == P1 && result == BACKDO && status[P11] == SCORE && loc[P12] == 0) {
		endTurn();
	}
	else if (curP == P1 && result == BACKDO && status[P12] == SCORE && loc[P11] == 0) {
		endTurn();
	}
	else if (curP == P2 && result == BACKDO && status[P21] == SCORE && loc[P22] == 0) {
		endTurn();
	}
	else if (curP == P2 && result == BACKDO && status[P22] == SCORE && loc[P21] == 0) {
		endTurn();
	}
	//Cannot select home mal for BACKDO
	else if (curP == P1 && result == BACKDO && loc[P11] == 0) {
		malSelected = P12;
		change = OFF;
	}
	else if (curP == P1 && result == BACKDO && loc[P12] == 0) {
		malSelected = P11;
		change = OFF;
	}
	else if (curP == P2 && result == BACKDO && loc[P21] == 0) {
		malSelected = P22;
		change = OFF;
	}
	else if (curP == P2 && result == BACKDO && loc[P22] == 0) {
		malSelected = P21;
		change = OFF;
	}

	if (curP == AI) {
		ai();
	}
	else if (curP != AI) {

		toggle = ~PINB & 0x02;		//Pin B.1 = SW7 Toggle
		enter = ~PINB & 0x01;		//Pin B.0 = SW6 Enter

		if (time7 == 0 && toggle != 0) {			//Toggle
			//Cannot select home mal for BACKDO
			if (curP == P1 && result == BACKDO && loc[P11] == 0) {
				malSelected = P12;
			}
			else if (curP == P1 && result == BACKDO && loc[P12] == 0) {
				malSelected = P11;
			}
			else if (curP == P2 && result == BACKDO && loc[P21] == 0) {
				malSelected = P22;
			}
			else if (curP == P2 && result == BACKDO && loc[P22] == 0) {
				malSelected = P21;
			}
			//Cannot select scored mal
			else if (curP == P1 && malSelected == P11 && status[P12] == SCORE) {
				malSelected = P11;
			}
			else if (curP == P1 && malSelected == P12 && status[P11] == SCORE) {
				malSelected = P12;
			}
			else if (curP == P2 && malSelected == P21 && status[P22] == SCORE) {
				malSelected = P21;
			}
			else if (curP == P2 && malSelected == P22 && status[P21] == SCORE) {
				malSelected = P22;
			}
			else if (curP == P1 && malSelected == P11) {
				malSelected = P12;
			}
			else if (curP == P1 && malSelected == P12) {
				malSelected = P11;
			}
			else if (curP == P2 && malSelected == P21) {
				malSelected = P22;
			}
			else if (curP == P2 && malSelected == P22) {
				malSelected = P21;
			}
			time7 = t7;
		}
		else if (enter != 0) {		//Enter 
			playMode = MOVE;
			move_delay = ON;
			time5 = t5;
		}
	}
}