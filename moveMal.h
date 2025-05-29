//moveMal.h -- Processes the move and updates the routes

//Move mal(s) along the route
void moveMal(char mal) {
	switch (route[mal]) {
	case 0:					//route0
		loc[mal] = route0[routePos[mal]];
		break;
	case 1:					//route1
		loc[mal] = route1[routePos[mal]];
		break;
	case 2:					//route2
		loc[mal] = route2[routePos[mal]];
		break;
	case 3:					//route3
		loc[mal] = route3[routePos[mal]];
		break;
	case 4:					//route4
		loc[mal] = route4[routePos[mal]];
		break;
	default:
		break;
	}
}


//**********************************************************  

//Perform route changes
void updateRoute(char mal) {

	switch (route[mal]) {
	case 0:					//route0
		//Landing on "5": route0 -> route1
		if (loc[mal] == 5) {
			route[mal] = 1;
			routePos[mal] = 1;
		}
		//Landing on "10": route0 -> route2
		else if (loc[mal] == 10) {
			route[mal] = 2;
			routePos[mal] = 1;
		}
		//Tossing BACK-DO from "1": route0 -> route4
		else if (loc[mal] == 20) {
			route[mal] = 4;
			routePos[mal] = 1;
		}
		break;
	case 1:					//route1
		//Landing on "23": route1 -> route3
		if (loc[mal] == 23) {
			route[mal] = 3;
			routePos[mal] = 1;
		}
		//Tossing BACK-DO from "5": route1 -> route0
		else if (loc[mal] == 4) {
			route[mal] = 0;
			routePos[mal] = 4;
		}
		break;
	case 2:					//route2
		//Tossing BACK-DO from "10": route2 -> route0
		if (loc[mal] == 9) {
			route[mal] = 0;
			routePos[mal] = 9;
		}
		break;
	case 3:					//route3
		//Tossing BACK-DO from "23": route3 -> route1
		if (loc[mal] == 22) {
			route[mal] = 1;
			routePos[mal] = 3;
		}
		break;
	case 4:					//route4
		//Tossing BACK-DO from "20": route4 -> route0
		if (loc[mal] == 1) {
			route[mal] = 0;
			routePos[mal] = 1;
		}
		break;
	default:
		break;
	}
}


//**********************************************************  

//Check the destination of the selected mal (for AI)
void checkDest(char mal, signed char space) {
	switch (route[mal]) {
	case 0:					//route0
		if (route0[routePos[mal] + space] > 0) {
			dest = route0[routePos[mal] + space];
		}
		else {
			dest = 0;
		}
		break;
	case 1:					//route1
		if (route1[routePos[mal] + space] > 0) {
			dest = route1[routePos[mal] + space];
		}
		else {
			dest = 0;
		}
		break;
	case 2:					//route2
		if (route2[routePos[mal] + space] > 0) {
			dest = route2[routePos[mal] + space];
		}
		else {
			dest = 0;
		}
		break;
	case 3:					//route3
		if (route3[routePos[mal] + space] > 0) {
			dest = route3[routePos[mal] + space];
		}
		else {
			dest = 0;
		}
		break;
	case 4:					//route4
		if (route4[routePos[mal] + space] > 0) {
			dest = route4[routePos[mal] + space];
		}
		else {
			dest = 0;
		}
		break;
	default:
		break;
	}
	return(dest);
}