CFLAGS= -g
GETPAR= /home/u1/dreger/Sources/libget.a
#PLOTLIB= ../plotsubs/stdplt
PLOTLIB= -lstd

all:	box mtest symbols test fan text

box:	box.c
	cc ${CFLAGS} -o box box.c ${PLOTLIB} ${GETPAR} -lm

mtest:	mtest.c
	cc ${CFLAGS} -o mtest mtest.c ${PLOTLIB} ${GETPAR} -lm
symbols:	symbols.c
	cc ${CFLAGS} -o symbols symbols.c ${PLOTLIB} ${GETPAR} -lm

csymbols:	csymbols.c
	cc ${CFLAGS} -o csymbols csymbols.c ${PLOTLIB} ${GETPAR} -lm

test:	test.c
	cc ${CFLAGS} -o test test.c ${PLOTLIB} ${GETPAR} -lm

fan:	fan.c
	cc ${CFLAGS} -o fan fan.c ${PLOTLIB} ${GETPAR} -lm

text:	text.c
	cc ${CFLAGS} -o text text.c ${PLOTLIB} ${GETPAR} -lm

cleanup:
	-rm -f test text box symbols csymbols fan
