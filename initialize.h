//initialize.h -- Initializes all hardware and SFRs

void initialize(void) {

	//Set up the ports
	DDRA = 0x00;    				//A.0-7 Main LED
	PORTA = 0x00;
	DDRD = 0x00;    				//D.0 Main LED
	PORTD = 0x00;   				//D.5 Yut trigger
	DDRC = 0xFF;					//C.0-7 Sub LED
	PORTC = 0x00;
	DDRB = 0x00;					//B.0-1 Button

	//Set up timer0 for 1 ms ticks
	TIMSK0 = 2;						//Turn on timer0 compare match ISR 
	OCR0A = 250;  					//Set the compare reg to 250 time ticks
	TCCR0A = 0b00000010; 			//Turn on clear-on-match
	TCCR0B = 0b00000011;			//Set clock prescalar to 64

	//Initialize the time counters
	time1 = t1;
	time2 = t2;
	time3 = t3;
	time4 = t4;
	time5 = t5;
	time6 = t6;
	time7 = t7;

	//Initialize stations
	for (char y = 0; y < 30; y++) {
		station[y] = EMPTY;
	}

	//Initialize all mals
	for (char j = 0; j < numMal; j++) {
		loc[j] = 0;					//Mal location
		route[j] = 0;				//Mal route
		routePos[j] = 0;			//Mal route position
		status[j] = SINGLE;			//Mal status
	}

	//Initialize the flags and variables
	flag = 0;
	total = 0;
	dest = 0;
	ready = OFF;
	change = ON;
	leave = OFF;
	sim = OFF;
	playMode = TOSS;
	malSelected = P11;
	curP = P1;
	sel_delay = OFF;
	move_delay = OFF;
	toss_delay = ON;

	//Seed the randomizer
	srand(SEED);

	//Crank up the ISRs
	sei();
}