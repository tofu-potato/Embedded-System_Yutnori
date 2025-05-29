//toss.h -- Simulates toss result 

void toss(void) {
	
	if (curP == AI) {				//AI skips Yut toss
		sim = ON;
	}	
	else if (curP != AI) {
		if (ready == ON) {			
			if (PIND & 0x10) {			//Pin D.4 = Yut stick trigger
				sim = ON;				//Comparator turns HIGH
				ready = OFF;	
			}
		}
	}

	if (sim == ON) {				//Ready to simulate toss
				
		//Simulate yut toss with q = 0.46
		for (char i = 0; i < 4; i++) {
			int num = rand() % 100;
			if (num < 46)  yut[i] = UP;
			else if (num >= 46)  yut[i] = DOWN;
		}

		//Yield toss result
		result = yut[0] + yut[1] + yut[2] + yut[3];
		if (result == 0)  result = MO;
		else if (yut[0] == UP && result == DO)  result = BACKDO;
		sim = OFF;					//Terminate toss
								
		blink2(result);
		playMode = SELECT;
		sel_delay = ON;
		time5 = t5;
	}
}