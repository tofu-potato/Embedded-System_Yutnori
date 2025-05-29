//timerISR.h -- Runs ISR

ISR(TIMER0_COMPA_vect) {
	//Decrement the four timers for 250 ms
	if (time1 > 0)  --time1;		//Refresh rate
	if (time2 > 0)  --time2;		//Blink rate
	if (time3 > 0)  --time3;		//Extender

	//Decrement further
	if (time3 == 0) {
		if (time4 > 0) 	--time4;	//Delay rate (2000 ms)
		if (time5 > 0) 	--time5;	//Delay rate (1000 ms)
		if (time6 > 0) 	--time6;	//Move rate (1000 ms)
		if (time7 > 0)  --time7;	//Debounce rate (500 ms)
		time3 = t3;
	}
}