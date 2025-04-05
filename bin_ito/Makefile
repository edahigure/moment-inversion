CFLAGS	= -std=c99
FFLAGS 	= 
CXXFLAGS= -std=c++0x

ALL	=   bin2sac FKRPROG makeascii sac2bin waveint window  

all:$(ALL)


bin2sac: bin2sac.c getpar.c
	gcc  -c getpar.c -o getpar.o
	gcc   bin2sac.c getpar.o -o bin2sac


FKRPROG: FKRPROG.f
	gfortran  -O3 FKRPROG.f -o FKRPROG


makeascii: makeascii.c getpar.c
	gcc  -c getpar.c -o getpar.o
	gcc   makeascii.c getpar.o -o makeascii



sac2bin: sac2bin.c getpar.c
	gcc  -c getpar.c -o getpar.o
	gcc   sac2bin.c getpar.o -o sac2bin


waveint: wvint9.f cwrite.o
	cc -c cwrite.c
	gfortran wvint9.f cwrite.o -o waveint


window: window.c  getpar.c
	gcc -o window window.c getpar.c 



