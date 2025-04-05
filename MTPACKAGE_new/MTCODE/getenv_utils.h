#ifndef lint
static char sccsid[] = "%W% %G% %U%";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getenv_lf ( char *var,double *pval);

int getenv_f ( char *var,float *pval);

int getenv_d ( char *var,int *pval);

int getenv_s ( char *var,char **pval);

