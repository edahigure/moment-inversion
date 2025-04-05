# CFLAGS=-O
CFLAGS=-c -O -DENVIRONMENT
#CFLAGS=-O

SOURCE = getpar.c getarg.c
CFILES = getpar.o getarg.o
FFILES = fgetpar.o fgetarg.o

ALL=get libget

all:	$(ALL)

get:
	gcc $(CFLAGS) $(SOURCE)
	ar r get $(CFILES)
	ranlib get
	ar tv get

libget: get
	\mv get ../libget.a

cleanup:
	rm *.o
list:
	imprint -2 $(SOURCE) Makefile

doc:
	itroff -man getpar.3
	itroff -man getarg.3
