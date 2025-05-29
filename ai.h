//ai.h -- Selects the mal to move based on the board situation

void ai(void) {
	if (change == OFF) {				//No changes needed
		playMode = MOVE;
		move_delay == ON;
		time5 = t5;
	}
	else if (change == ON) {			//Changes available
		if (curP == P1) {

			//Check for captures
			if (station[checkDest(P11, result)] == DOUBLE2) {			//Capture ub
				malSelected = P11;
			}
			else if (station[checkDest(P12, result)] == DOUBLE2) {		//Capture ub
				malSelected = P12;
			}
			else if (station[checkDest(P11, result)] == P21 && station[checkDest(P12, result)] == P22) {
				if (checkDest(P11, result) > checkDest(P12, result)) {	//Capture further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, result)] == P22 && station[checkDest(P12, result)] == P21) {
				if (checkDest(P11, result) > checkDest(P12, result)) {	//Capture further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, result)] == P21 || station[checkDest(P11, result)] == P22) {
				malSelected = P11;
			}
			else if (station[checkDest(P12, result)] == P21 || station[checkDest(P12, result)] == P22) {
				malSelected = P12;
			}

			//Check for threats											//One space behind
			else if (station[checkDest(P11, -1)] == P21 && station[checkDest(P12, -1)] == P22) {
				if (checkDest(P11, -1) > checkDest(P12, -1)) {			//Run further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, -1)] == P22 && station[checkDest(P12, -1)] == P21) {
				if (checkDest(P11, -1) > checkDest(P12, -1)) {			//Run further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, -1)] == P21 || station[checkDest(P11, -1)] == P22) {
				malSelected = P11;
			}
			else if (station[checkDest(P12, -1)] == P21 || station[checkDest(P12, -1)] == P22) {
				malSelected = P12;
			}															//Two spaces behind
			else if (station[checkDest(P11, -2)] == P21 && station[checkDest(P12, -2)] == P22) {
				if (checkDest(P11, -2) > checkDest(P12, -2)) {			//Run further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, -2)] == P22 && station[checkDest(P12, -2)] == P21) {
				if (checkDest(P11, -2) > checkDest(P12, -2)) {			//Run further mal
					malSelected = P11;
				}
				else {
					malSelected = P12;
				}
			}
			else if (station[checkDest(P11, -2)] == P21 || station[checkDest(P11, -2)] == P22) {
				malSelected = P11;
			}
			else if (station[checkDest(P12, -2)] == P21 || station[checkDest(P12, -2)] == P22) {
				malSelected = P12;
			}

			//Check for ub
			else if (station[checkDest(P11, result)] == P12) {
				malSelected = P11;
			}
			else if (station[checkDest(P12, result)] == P11) {
				malSelected = P12;
			}

			//Check for route changes
			else if (checkDest(P11, result) == 20) {
				malSelected = P11;
			}
			else if (checkDest(P12, result) == 20) {
				malSelected = P12;
			}
			else if (checkDest(P11, result) == 23) {
				malSelected = P11;
			}
			else if (checkDest(P12, result) == 23) {
				malSelected = P12;
			}
			else if (checkDest(P11, result) == 10) {
				malSelected = P11;
			}
			else if (checkDest(P12, result) == 10) {
				malSelected = P12;
			}
			else if (checkDest(P11, result) == 5) {
				malSelected = P11;
			}
			else if (checkDest(P12, result) == 5) {
				malSelected = P12;
			}

			//50:50 otherwise
			else {
				int num = rand() % 100;
				if (num < 50)  malSelected = P12;
				else if (num >= 50)  malSelected = P11;
			}
		}

		else if (curP == P2) {

			//Check for captures
			if (station[checkDest(P21, result)] == DOUBLE1) {			//Capture ub
				malSelected = P21;
			}
			else if (station[checkDest(P22, result)] == DOUBLE1) {		//Capture ub
				malSelected = P22;
			}
			else if (station[checkDest(P21, result)] == P11 && station[checkDest(P22, result)] == P12) {
				if (checkDest(P21, result) > checkDest(P22, result)) {	//Capture further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, result)] == P12 && station[checkDest(P22, result)] == P11) {
				if (checkDest(P21, result) > checkDest(P22, result)) {	//Capture further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, result)] == P11 || station[checkDest(P21, result)] == P12) {
				malSelected = P21;
			}
			else if (station[checkDest(P22, result)] == P11 || station[checkDest(P22, result)] == P12) {
				malSelected = P22;
			}

			//Check for threats											//One space behind
			else if (station[checkDest(P21, -1)] == P11 && station[checkDest(P22, -1)] == P12) {
				if (checkDest(P21, -1) > checkDest(P22, -1)) {			//Run further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, -1)] == P12 && station[checkDest(P22, -1)] == P11) {
				if (checkDest(P21, -1) > checkDest(P22, -1)) {			//Run further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, -1)] == P11 || station[checkDest(P21, -1)] == P12) {
				malSelected = P21;
			}
			else if (station[checkDest(P22, -1)] == P11 || station[checkDest(P22, -1)] == P12) {
				malSelected = P22;
			}															//Two spaces behind
			else if (station[checkDest(P21, -2)] == P11 && station[checkDest(P22, -2)] == P12) {
				if (checkDest(P21, -2) > checkDest(P22, -2)) {			//Run further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, -2)] == P12 && station[checkDest(P22, -2)] == P11) {
				if (checkDest(P21, -2) > checkDest(P22, -2)) {			//Run further mal
					malSelected = P21;
				}
				else {
					malSelected = P22;
				}
			}
			else if (station[checkDest(P21, -2)] == P11 || station[checkDest(P21, -2)] == P12) {
				malSelected = P21;
			}
			else if (station[checkDest(P22, -2)] == P11 || station[checkDest(P22, -2)] == P12) {
				malSelected = P22;
			}

			//Check for ub
			else if (station[checkDest(P21, result)] == P22) {
				malSelected = P21;
			}
			else if (station[checkDest(P22, result)] == P21) {
				malSelected = P22;
			}

			//Check for route changes
			else if (checkDest(P21, result) == 20) {
				malSelected = P21;
			}
			else if (checkDest(P22, result) == 20) {
				malSelected = P22;
			}
			else if (checkDest(P21, result) == 23) {
				malSelected = P21;
			}
			else if (checkDest(P22, result) == 23) {
				malSelected = P22;
			}
			else if (checkDest(P21, result) == 10) {
				malSelected = P21;
			}
			else if (checkDest(P22, result) == 10) {
				malSelected = P22;
			}
			else if (checkDest(P21, result) == 5) {
				malSelected = P21;
			}
			else if (checkDest(P22, result) == 5) {
				malSelected = P22;
			}

			//50:50 otherwise
			else {
				int num = rand() % 100;
				if (num < 50)  malSelected = P22;
				else if (num >= 50)  malSelected = P21;
			}
		}
		playMode = MOVE;
		move_delay == ON;
		time5 = t5;
	}
}