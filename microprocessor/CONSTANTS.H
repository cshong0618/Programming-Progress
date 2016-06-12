#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SIZE 32
#define WORD 4
#define PROGLINES_MAX ((1 << 17))
#define BRANCH_MAX (PROGLINES_MAX / 2)
/*
	Instructions
*/
#define LOAD "LOAD"
#define LOAD_I LOAD
#define STORE "STORE"
#define STORE_I STORE
#define ADD "ADD"
#define MULT "MULT"
#define MINUS "MINUS"
#define DIVIDE "DIV"

#define RESET "RESET"
#define SET "SET"

#define OPEN "OPEN"
#define IF "IF"
#define ELSE "ELSE"
#define ENDIF "ENDIF"
#define SETJMP "SETJMP"
#define JMP "JMP"
#define DISPLAY "DISPLAY"
#define PRINT "PRINT"
#define FPRINT "FPRINT"
#define NEWLINE "NEWLINE"
#define SPACE "SPACE"
#define SETVAR "SETVAR"
#define READIN "READINPUT"
#define CLEARALL "CLEARALL"
#define DOWHILE "DOWHILE"
#define ENDDO "ENDDO"

#define EXIT "EXIT"

/*
	Instruction codes
*/
#define B_LOAD 1

// 2
#define B_LOAD_I 1 << 1

// 4
#define B_STORE 1 << 2

// 8
#define B_STORE_I 1 << 3

// 16
#define B_ADD 1 << 4

// 32
#define B_MULT 1 << 5

// 64
#define B_MINUS 1 << 6

// 128
#define B_DIVIDE 1 << 7

// 256
#define B_RESET 1 << 8
#define B_CLEARALL 257

// 512
#define B_SET 1 << 9
#define B_OPEN 513
#define B_IF 514
#define B_ELSE 515
#define B_ENDIF 516
#define B_SETJMP 517
#define B_JMP 518
#define B_DISPLAY 519
#define B_SETVAR 520
#define B_READIN 521
#define B_PRINT 522
#define B_NEWLINE 523
#define B_DOWHILE 525
#define B_ENDDO 526
#define B_SPACE 527
#define B_FPRINT 528

// 1024, 1 << 10


/*
	Identifier
*/
#define R_R (int)'R'
#define R_V (int)'V'
#define R_N (int)'#'
#define R_MORE (int)'>'
#define R_LESS (int)'<'
#define R_EQUALS (int)'='
#define R_NOT (int)'!'
/*
	Identifier codes
*/
#define B_R 1
#define B_V 1 << 4

// 32
#define B_N 1 << 5

// 64
#define B_MORE 1 << 6
#define B_LESS 65
#define B_EQUALS 66
#define B_NOT 67
#define B_MORE_EQUAL ((B_MORE) + B_EQUALS)
#define B_LESS_EQUAL (B_LESS + B_EQUALS)

/*
	Error
*/
#define ERROR "Error: "
#define DIVISION_BY_ZERO ERROR ## "Division by zero"
#define OUT_OF_BOUNDS ERROR ## "Array out of bounds"

/*
	Error codes
*/
#define B_DIVISON_BY_ZERO 1 << 0
#define B_OUT_OF_BOUNDS 1 << 1

/*
	Flags
*/
#define COMPLIER_MODE 1
#define LOAD_SUCCESS 1 << 5
#define SET_SUCCESS 1 << 6

#endif