//move.h -- Moves the selected mal according to the toss result

void move(void) {

	if (leave == OFF) {
		station[loc[malSelected]] = EMPTY;
		leave = ON;
		total = result;
	}
	else if (leave == ON) {
		if (time6 == 0 && result > 0) {
			if (curP == P1 && status[malSelected] == DOUBLE) {
				routePos[P11]++;				//If ub, move both mals
				routePos[P12]++;
				moveMal(P11);
				moveMal(P12);
			}
			else if (curP == P2 && status[malSelected] == DOUBLE) {
				routePos[P21]++;				//If ub, move both mals
				routePos[P22]++;
				moveMal(P21);
				moveMal(P22);
			}
			else if (status[malSelected] == SINGLE) {
				routePos[malSelected]++;		//If not, move selected mal
				moveMal(malSelected);
			}
			checkScore();						//Terminate if scored
			result--;
			time6 = t6;
		}
		else if (time6 == 0 && result < 0) {	//BACKDO
			if (curP == P1 && status[malSelected] == DOUBLE) {
				routePos[P11]--;				//If ub, move both mals
				routePos[P12]--;
				moveMal(P11);
				moveMal(P12);
			}
			else if (curP == P2 && status[malSelected] == DOUBLE) {
				routePos[P21]--;				//If ub, move both mals
				routePos[P22]--;
				moveMal(P21);
				moveMal(P22);
			}
			else if (status[malSelected] == SINGLE) {
				routePos[malSelected]--;		//If not, move selected mal
				moveMal(malSelected);
			}
			checkScore();						//Terminate if scored
			result++;
			time6 = t6;
		}
		else if (time6 == 0 && result == 0) {	//Reached dest
			if (curP == P1 && status[malSelected] == DOUBLE) {
				updateRoute(P11);				//If ub, update both mals
				updateRoute(P12);
			}
			else if (curP == P2 && status[malSelected] == DOUBLE) {
				updateRoute(P21);				//If ub, update both mals
				updateRoute(P22);
			}
			else if (status[malSelected] == SINGLE) {
				updateRoute(malSelected);		//If not, update selected mal
			}
			checkScore();						//Terminate if scored
			checkStation();
		}
	}
}