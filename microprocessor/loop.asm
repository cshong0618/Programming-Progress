SET V0 #1
SET V15 #2
LOAD V15 R15
SET V14 #8
LOAD V14 R14

MULT R15 R15 R14

ADD R13 R13 #4
MULT R13 R13 R14
ADD R13 R13 #1

SETJMP A
LOAD V0 R0
DOWHILE <= R0 R15
	LOAD V1 R1
	DOWHILE < R1 R0
		SPACE
		ADD R1 R1 #1
	ENDDO

	MULT R12 R0 #2
	MINUS R11 R13 R12

	LOAD V2 R2
	DOWHILE < R2 R11
		DISPLAY *
		ADD R2 R2 #1
	ENDDO
	NEWLINE
	ADD R0 R0 #1
ENDDO
ADD R5 R5 #1
NEWLINE
IF < R5 #10
	JMP A
ENDIF