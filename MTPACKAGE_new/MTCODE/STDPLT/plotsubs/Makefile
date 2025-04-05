STDLIB = ../

SOURCE = frame.c scale.c table.c err.c plot.c dot.c \
	text.c  polygon.c output.c erase.c symbol.c \
	puth.c fortran.c
FILES=	${SOURCE:%.c=%.o}

STDLIB = ../..

SYM = 	symbolname.h
HEADERS = ../h/igl.h stdplt.h
CFLAGS= -O 

stdplt:	$(FILES) $(SYM)
	ar cr stdplt $(FILES)
	ranlib stdplt 
	\mv stdplt ../../libstd.a

symbol.o: ${SYM}

${SYM}:	../symbol/symboldata
	(cd ../symbol; make )

testload:
	gcc -g dummymain.c stdplt -lm
	@echo "execute test"
	a.out > /dev/null


$(FILES): $(HEADERS)

list:
	imprint  makefile sublist ${HEADERS} $(SOURCE)

install:
	install -c -m 644 stdplt ${STDLIB}/libstd.a
	ranlib ${STDLIB}/libstd.a

cleanup:
	-rm -f $(FILES) stdplt

.c.o:
	gcc -c  -O4 $< -o  $@
