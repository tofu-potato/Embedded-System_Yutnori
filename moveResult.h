//moveResult.h -- Checks for special cases after the move

//Change mal conditions according to station occupancy
void checkStation(void) {

	if (curP == P1) {

		//Check own station
		if (malSelected == P11 && station[loc[malSelected]] == P12) {
			status[P11] = DOUBLE;			//Ub the mal(s)		
			status[P12] = DOUBLE;
			station[loc[malSelected]] = DOUBLE1;
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}
		else if (malSelected == P12 && station[loc[malSelected]] == P11) {
			status[P11] = DOUBLE;			//Ub the mal(s)		
			status[P12] = DOUBLE;
			station[loc[malSelected]] = DOUBLE1;
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}

		//Check enemy station
		else if (station[loc[malSelected]] == P21) {
			loc[P21] = 0;					//Reinitialize captured mal
			route[P21] = 0;
			routePos[P21] = 0;
			status[P21] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE1;
			}
			continueTurn();
		}
		else if (station[loc[malSelected]] == P22) {
			loc[P22] = 0;					//Reinitialize captured mal
			route[P22] = 0;
			routePos[P22] = 0;
			status[P22] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE1;
			}
			continueTurn();
		}
		else if (station[loc[malSelected]] == DOUBLE2) {
			loc[P21] = 0;					//Reinitialize captured mals
			loc[P22] = 0;
			route[P21] = 0;
			route[P22] = 0;
			routePos[P21] = 0;
			routePos[P22] = 0;
			status[P21] = SINGLE;
			status[P22] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE1;
			}
			continueTurn();
		}
		//Check empty station
		else if (station[loc[malSelected]] == EMPTY) {
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE1;
			}
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}
	}

	else if (curP == P2) {

		//Check own station
		if (malSelected == P21 && station[loc[malSelected]] == P22) {
			status[P21] = DOUBLE;				//Ub the mal(s)		
			status[P22] = DOUBLE;
			station[loc[malSelected]] = DOUBLE2;
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}
		else if (malSelected == P22 && station[loc[malSelected]] == P21) {
			status[P21] = DOUBLE;			//Ub the mal(s)		
			status[P22] = DOUBLE;
			station[loc[malSelected]] = DOUBLE2;
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}

		//Check enemy station
		else if (station[loc[malSelected]] == P11) {
			loc[P11] = 0;					//Reinitialize captured mal
			route[P11] = 0;
			routePos[P11] = 0;
			status[P11] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE2;
			}
			continueTurn();
		}
		else if (station[loc[malSelected]] == P12) {
			loc[P12] = 0;					//Reinitialize captured mal
			route[P12] = 0;
			routePos[P12] = 0;
			status[P12] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE2;
			}
			continueTurn();
		}
		else if (station[loc[malSelected]] == DOUBLE1) {
			loc[P11] = 0;					//Reinitialize captured mals
			loc[P12] = 0;
			route[P11] = 0;
			route[P12] = 0;
			routePos[P11] = 0;
			routePos[P12] = 0;
			status[P11] = SINGLE;
			status[P12] = SINGLE;
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE2;
			}
			continueTurn();
		}

		//Check empty station
		else if (station[loc[malSelected]] == EMPTY) {
			if (status[malSelected] == SINGLE) {
				station[loc[malSelected]] = malSelected;
			}
			else if (status[malSelected] == DOUBLE) {
				station[loc[malSelected]] = DOUBLE2;
			}
			if (total == YUT || total == MO) {	//Continue if YUT or MO
				continueTurn();
			}
			else {
				endTurn();
			}
		}
	}
}


//**********************************************************   

//End current turn
void endTurn(void) {
	playMode = TOSS;
	total = 0;
	leave = OFF;
	ready = ON;

	//Switch current player
	if (curP == P1) {
		curP = P2;
		if (status[P21] == SCORE) {		//Cannot select scored mal
			malSelected = P22;
			change = OFF;
		}
		else if (status[P22] == SCORE) {
			malSelected = P21;
			change = OFF;
		}
		else {							//Default select setting
			malSelected = P21;
			change = ON;
		}
	}
	else if (curP == P2) {
		curP = P1;
		if (status[P11] == SCORE) {		//Cannot select scored mal
			malSelected = P12;
			change = OFF;
		}
		else if (status[P12] == SCORE) {
			malSelected = P11;
			change = OFF;
		}
		else {							//Default select setting
			malSelected = P11;
			change = ON;
		}
	}
	blink2(OFF);
	toss_delay = ON;
	time4 = t4;
}


//**********************************************************   

//Continue current turn
void continueTurn(void) {
	playMode = TOSS;
	total = 0;
	leave = OFF;
	ready = ON;

	if (curP == P1) {
		curP = P1;
		if (status[P11] == SCORE) {		//Cannot select scored mal
			malSelected = P12;
			change = OFF;
		}
		else if (status[P12] == SCORE) {
			malSelected = P11;
			change = OFF;
		}
		else {							//Default select setting
			malSelected = P11;
			change = ON;
		}
	}
	else if (curP == P2) {
		curP = P2;
		if (status[P21] == SCORE) {		//Cannot select scored mal
			malSelected = P22;
			change = OFF;
		}
		else if (status[P22] == SCORE) {
			malSelected = P21;
			change = OFF;
		}
		else {							//Default select setting
			malSelected = P21;
			change = ON;
		}
	}
	blink2(OFF);
	toss_delay = ON;
	time4 = t4;
}


//**********************************************************       

//Check if mal(s) scored
void checkScore(void) {
	if (loc[malSelected] == 30) {
		if (curP == P1 && status[malSelected] == DOUBLE) {
			status[P11] = SCORE;
			status[P12] = SCORE;
		}
		else if (curP == P2 && status[malSelected] == DOUBLE) {
			status[P21] = SCORE;
			status[P22] = SCORE;
		}
		else if (status[malSelected] == SINGLE) {
			status[malSelected] = SCORE;
		}
		checkWin();
		if (total == YUT || total == MO) {	//Continue if YUT or MO
			continueTurn();
		}
		else {
			endTurn();
		}
	}
}


//**********************************************************       

//Check if winning condition is met and end the game
void checkWin(void) {
	//Player 1 wins		
	if (status[P11] == SCORE && status[P12] == SCORE) {
		malSelected = EMPTY;		//No blinking

		while (1) {
			blink();
			blink2(OFF);
		}
	}

	//Player 2 wins
	else if (status[P21] == SCORE && status[P22] == SCORE) {
		malSelected = EMPTY;		//No blinking

		while (1) {
			blink();
			blink2(OFF);
		}
	}
}