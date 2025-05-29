//locate.h -- Locate and light up mal according to its board address

void locate(char mal, char n) {

	DDRA = 0x00;		//Clear previous state
	PORTA = 0x00;
	DDRD = 0x00;
	PORTD = 0x00;

	if (mal == P11 || mal == P12) {
		switch (n) {
		case 0:			//HOME: 96 or 97
			if (mal == P11) {
				DDRA = 0x20;
				PORTA = 0x00;
				DDRD = 0x01;
				PORTD = 0x01;
			}
			else if (mal == P12) {
				DDRA = 0x40;
				PORTA = 0x00;
				DDRD = 0x01;
				PORTD = 0x01;
			}
			break;

		case 1:			//23
			DDRA = 0x06;
			PORTA = 0x02;
			break;

		case 2:			//26
			DDRA = 0x22;
			PORTA = 0x02;
			break;

		case 3:			//27
			DDRA = 0x42;
			PORTA = 0x02;
			break;

		case 4:			//29
			DDRA = 0x02;
			PORTA = 0x02;
			DDRD = 0x01;
			PORTD = 0x00;

			break;

		case 5:			//18
			DDRA = 0x81;
			PORTA = 0x01;
			break;

		case 6:			//67
			DDRA = 0x60;
			PORTA = 0x20;
			break;

		case 7:			//64
			DDRA = 0x28;
			PORTA = 0x20;
			break;

		case 8:			//62
			DDRA = 0x22;
			PORTA = 0x20;
			break;

		case 9:			//59
			DDRA = 0x10;
			PORTA = 0x10;
			DDRD = 0x01;
			PORTD = 0x00;

			break;

		case 10:			//57
			DDRA = 0x50;
			PORTA = 0x10;
			break;

		case 11:			//45
			DDRA = 0x18;
			PORTA = 0x08;
			break;

		case 12:			//47
			DDRA = 0x48;
			PORTA = 0x08;
			break;

		case 13:			//49
			DDRA = 0x08;
			PORTA = 0x08;
			DDRD = 0x01;
			PORTD = 0x00;
			break;

		case 14:			//51
			DDRA = 0x11;
			PORTA = 0x10;
			break;

		case 15:			//53
			DDRA = 0x14;
			PORTA = 0x10;
			break;

		case 16:			//41
			DDRA = 0x09;
			PORTA = 0x08;
			break;

		case 17:			//39
			DDRA = 0x04;
			PORTA = 0x04;
			DDRD = 0x01;
			PORTD = 0x00;
			break;

		case 18:			//36
			DDRA = 0x24;
			PORTA = 0x04;
			break;

		case 19:			//34
			DDRA = 0x0C;
			PORTA = 0x04;
			break;

		case 20:			//69
			DDRA = 0x20;
			PORTA = 0x20;
			DDRD = 0x01;
			PORTD = 0x00;
			break;

		case 21:			//72
			DDRA = 0x42;
			PORTA = 0x40;
			break;

		case 22:			//74
			DDRA = 0x48;
			PORTA = 0x40;
			break;

		case 23:			//76
			DDRA = 0x60;
			PORTA = 0x40;
			break;

		case 24:			//79
			DDRA = 0x40;
			PORTA = 0x40;
			DDRD = 0x01;
			PORTD = 0x00;
			break;

		case 25:			//81
			DDRA = 0x81;
			PORTA = 0x80;
			break;

		case 26:			//83
			DDRA = 0x84;
			PORTA = 0x80;
			break;

		case 27:			//86
			DDRA = 0xA0;
			PORTA = 0x80;
			break;

		case 28:			//61
			DDRA = 0x21;
			PORTA = 0x20;
			break;

		case 29:			//92
			DDRA = 0x02;
			PORTA = 0x00;
			DDRD = 0x01;
			PORTD = 0x01;
			break;

		case 30:			//SCORE: 14 or 15
			if (mal == P11) {
				DDRA = 0x09;
				PORTA = 0x01;
			}
			else if (mal == P12) {
				DDRA = 0x11;
				PORTA = 0x01;
			}
			break;

		default:			//Turn off
			DDRA = 0x00;
			PORTA = 0x00;
			DDRD = 0x00;
			PORTD = 0x00;
			break;
		}
	}

	else if (mal == P21 || mal == P22) {
		switch (n) {
		case 0:			//HOME: 16 or 17
			if (mal == P21) {
				DDRA = 0x21;
				PORTA = 0x01;
			}
			else if (mal == P22) {
				DDRA = 0x41;
				PORTA = 0x01;
			}
			break;

		case 1:			//21
			DDRA = 0x03;
			PORTA = 0x02;
			break;

		case 2:			//24
			DDRA = 0x0A;
			PORTA = 0x02;
			break;

		case 3:			//25
			DDRA = 0x12;
			PORTA = 0x02;
			break;

		case 4:			//28
			DDRA = 0x82;
			PORTA = 0x02;
			break;

		case 5:			//31
			DDRA = 0x05;
			PORTA = 0x04;
			break;

		case 6:			//65
			DDRA = 0x30;
			PORTA = 0x20;
			break;

		case 7:			//63
			DDRA = 0x24;
			PORTA = 0x20;
			break;

		case 8:			//89
			DDRA = 0x80;
			PORTA = 0x80;
			DDRD = 0x01;
			PORTD = 0x00;
			break;

		case 9:			//95
			DDRA = 0x10;
			PORTA = 0x00;
			DDRD = 0x01;
			PORTD = 0x01;
			break;

		case 10:			//56
			DDRA = 0x30;
			PORTA = 0x10;
			break;

		case 11:			//43
			DDRA = 0x0C;
			PORTA = 0x08;
			break;

		case 12:			//94
			DDRA = 0x08;
			PORTA = 0x00;
			DDRD = 0x01;
			PORTD = 0x01;
			break;

		case 13:			//48
			DDRA = 0x88;
			PORTA = 0x08;
			break;

		case 14:			//52
			DDRA = 0x12;
			PORTA = 0x10;
			break;

		case 15:			//54
			DDRA = 0x18;
			PORTA = 0x10;
			break;

		case 16:			//42
			DDRA = 0x0A;
			PORTA = 0x08;
			break;

		case 17:			//37
			DDRA = 0x44;
			PORTA = 0x04;
			break;

		case 18:			//38
			DDRA = 0x84;
			PORTA = 0x04;
			break;

		case 19:			//35
			DDRA = 0x14;
			PORTA = 0x04;
			break;

		case 20:			//68
			DDRA = 0xA0;
			PORTA = 0x20;
			break;

		case 21:			//71
			DDRA = 0x41;
			PORTA = 0x40;
			break;

		case 22:			//73
			DDRA = 0x44;
			PORTA = 0x40;
			break;

		case 23:			//75
			DDRA = 0x50;
			PORTA = 0x40;
			break;

		case 24:			//78
			DDRA = 0xC0;
			PORTA = 0x40;
			break;

		case 25:			//82
			DDRA = 0x82;
			PORTA = 0x80;
			break;

		case 26:			//84
			DDRA = 0x88;
			PORTA = 0x80;
			break;

		case 27:			//85
			DDRA = 0x90;
			PORTA = 0x80;
			break;

		case 28:			//87
			DDRA = 0xC0;
			PORTA = 0x80;
			break;

		case 29:			//91
			DDRA = 0x01;
			PORTA = 0x00;
			DDRD = 0x01;
			PORTD = 0x01;
			break;

		case 30:			//SCORE: 32 or 19
			if (mal == P21) {
				DDRA = 0x06;
				PORTA = 0x04;
			}
			else if (mal == P22) {
				DDRA = 0x01;
				PORTA = 0x01;
				DDRD = 0x01;
				PORTD = 0x00;
			}
			break;

		default:			//Turn off
			DDRA = 0x00;
			PORTA = 0x00;
			DDRD = 0x00;
			PORTD = 0x00;
			break;
		}
	}
}