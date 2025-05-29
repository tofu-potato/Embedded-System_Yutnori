//blink.h -- Turns on and blinks LED's

//Blink selected mal and display remaining mals
void blink(void) {
	//Time division multiple access (TDMA) with flag
	if (time1 == 0 && flag == 0) {			//Dedicated to P1-1
		flag = 1;
		time1 = t1;
		if (curP == P1 && malSelected == P11) {
			if (time2 >= 125) {				//Blink if selected
				locate(P11, loc[P11]);
			}
			else if (time2 < 125) {
				locate(P11, 44);
			}
		}
		else {								//Remain turned on otherwise
			locate(P11, loc[P11]);
		}
		if (time2 == 0)  time2 = t2;
	}

	else if (time1 == 0 && flag == 1) {		//Dedicated to P1-2
		flag = 2;
		time1 = t1;
		if (curP == P1 && malSelected == P12) {
			if (time2 >= 125) {
				locate(P12, loc[P12]);
			}
			else if (time2 < 125) {
				locate(P12, 44);
			}
		}
		else {
			locate(P12, loc[P12]);
		}
		if (time2 == 0)  time2 = t2;
	}

	else if (time1 == 0 && flag == 2) {		//Dedicated to P2-1
		flag = 3;
		time1 = t1;
		if (curP == P2 && malSelected == P21) {
			if (time2 >= 125) {
				locate(P21, loc[P21]);
			}
			else if (time2 < 125) {
				locate(P21, 44);
			}
		}
		else {
			locate(P21, loc[P21]);
		}
		if (time2 == 0)  time2 = t2;
	}

	else if (time1 == 0 && flag == 3) {		//Dedicated to P2-2
		flag = 0;
		time1 = t1;
		if (curP == P2 && malSelected == P22) {

			if (time2 >= 125) {
				locate(P22, loc[P22]);
			}
			else if (time2 < 125) {
				locate(P22, 44);
			}
		}
		else {
			locate(P22, loc[P22]);
		}
		if (time2 == 0)  time2 = t2;
	}
}


//**********************************************************  

//Display player indicator and toss result
void blink2(signed char n) {
	if (curP == P1) {
		switch (n) {
		case BACKDO:		//10100000
			PORTC = 0xA0;
			break;

		case DO:			//10010000
			PORTC = 0x90;
			break;

		case GAE:			//10011000
			PORTC = 0x98;
			break;

		case GEOL:			//10011100
			PORTC = 0x9C;
			break;

		case YUT:			//10011110
			PORTC = 0x9E;
			break;

		case MO:			//10011111
			PORTC = 0x9F;
			break;

		case OFF:			//10000000
			PORTC = 0x80;
			break;

		default:			//Turn off
			PORTC = 0x00;
			break;
		}
	}
	else if (curP == P2) {
		switch (n) {
		case BACKDO:		//01100000
			PORTC = 0x60;
			break;

		case DO:			//01010000
			PORTC = 0x50;
			break;

		case GAE:			//01011000
			PORTC = 0x58;
			break;

		case GEOL:			//01011100
			PORTC = 0x5C;
			break;

		case YUT:			//01011110
			PORTC = 0x5E;
			break;

		case MO:			//01011111
			PORTC = 0x5F;
			break;

		case OFF:			//01000000
			PORTC = 0x40;
			break;

		default:			//Turn off
			PORTC = 0x00;
			break;
		}
	}
}