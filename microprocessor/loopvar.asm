LOAD V0 R0
ADD R2 R2 #1

DOWHILE < R0 #50
	ADD R5 R1 R2

	STORE R2 V2
	LOAD V2 R1

	STORE R5 V5
	LOAD V5 R2

	PRINT R5
	SPACE
	ADD R0 R0 #1
ENDDO
NEWLINE
