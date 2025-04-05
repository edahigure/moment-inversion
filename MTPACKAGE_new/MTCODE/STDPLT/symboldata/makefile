# Makefile for symbol subroutines
# note this doesn't work
HEADERS = ../h/igl.h sym.h
SOURCE = getnames.c getsymdata.c
STD_OBJS = getnames.o  
COM_OBJS = getsymdata.o
DATA = createfiles
#CFLAGS = -g -pg
#CFLAGS= -fsingle

dummy : $(STD_OBJS) $(DATA) $(COM_OBJS)
$(STD_OBJS) : $(HEADERS)
$(COM_OBJS) : $(HEADERS)
$(DATA):
	cc -g -o createfiles createfiles.c /home/u1/dreger/Sources/libget.a

list:
	imprint makefile $(SOURCE) createfiles.c textfile sym.h
