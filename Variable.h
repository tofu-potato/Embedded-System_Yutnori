#pragma once

// Yut No Ri Board
/*

10*			9			8			7			6			5*


		21											20
11															4

				23							22

12															3

							   24*

13															2

				26							25

14															1

		28											27

15*			16			17			18			19			0
*/

#define		do		1
#define		ge		2
#define		gul		3
#define		yoot	4
#define		mo		5

//for shortcut
#define		S1		5
#define		S2		10
#define		S3		24
#define		S4		15

// for game staus
#define		MaxPlayer	2
#define		Player1		0
#define		C1			1	

typedef struct {
	unsigned int Turn; // ==> if Turn = 0 ==> player, 1 ==> Computer

	unsigned int Horse;

	// for Individual status
	unsigned int TimeOver;
	unsigned int YootResult; // initial value is 0b0000; ==> there are 4 figures at start

	// 1110 => yoot >> 3
	unsigned int Ate;
	unsigned int UpEotNya;
	unsigned int DataReceived;
	unsigned int Data;
	int Location[8];	// for individual value of figure - 0,2,4,6 : player 1,3,5,7 : Computer
	int Options[20];	// even : Player1's option, odd : C1's option

	unsigned int Throw_Again;

	// when the player meets 2 ways
	unsigned int shortCut;
	unsigned int backDo;

}Players;

typedef struct {
	unsigned int numbers;

}Board;


extern void init_Player(Players* p);	// resets the player status

extern void Role(Players* p);		// throws yoot

extern void Move(Players* p);		// player move the figure