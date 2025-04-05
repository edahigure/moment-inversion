/* Routine to read Helmberger Format Seismograms and to return */
/* information about number of traces, number of time points,  */
/* dt, and the data vector                                     */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define FORM1a "%8d\n"
#define FORM1b "%8d      %s\n"
#define FORM2  "%s\n"
#define FORM3  "    %11.4e    %11.4e      0  0  0.00\n"
#define FORM4a "%8d  %8.5f %11.4e\n"
#define TRUE 1
#define FALSE 0

static char form[32]     = "(6e12.5)";
FILE *fopen(),*inf,*of;
int fclose();


void readhelm(char *in,int *N,int *NT,float *DT,float *vec1);
int chkform(char *f_form,char *c_form1,char *c_form2,int *f_width);
