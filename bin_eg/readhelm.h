/* Routine to read Helmberger Format Seismograms and to return */
/* information about number of traces, number of time points,  */
/* dt, and the data vector                                     */

#ifndef READHELM
#define READHELM


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <sys/file.h>
#include<string.h>


#define FORM1a "%8d\n"
#define FORM1b "%8d      %s\n"
#define FORM2  "%s\n"
#define FORM3  "    %11.4e    %11.4e      0  0  0.00\n"
#define FORM4a "%8d  %8.5f %11.4e\n"
#define TRUE 1
#define FALSE 0


struct GREEN
 {
 int zz,nn;
 int np;
 float dt;
 float *u1; /* Tangential Strikeslip */
 float *u2; /* Tangential Dipslip    */
 float *u3; /* Radial     Strikeslip */
 float *u4; /* Radial     Dipslip    */
 float *u5; /* Radial     45_slip    */
 float *u6; /* Vertical   Strikeslip */
 float *u7; /* Vertical   Dipslip    */
 float *u8; /* Vertical   45_slip    */
 float *u9; /* Radial     Explosion  */
 float *u10; /* Vertical   Explosion    */
 };

static FILE *inf,*of;

static char form[32]     = "(6e12.5)";

void readhelm(char *in,int *N,int *NT,float *DT,float *vec1);
int chkform(char *f_form,char *c_form1,char *c_form2,int *f_width);

   /*Subroutines*/
static char x_form[64];

#endif
