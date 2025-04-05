#include<sys/file.h>
#include<fcntl.h>
#include<stdio.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>
#include "sacio.h"
//#include "libmseed.h"
#include "distaz.h"

float **matrixf( long nrh,  long nch);

#define N_MAX 500000
#define N_MAX_ST 1000

#define FALSE   0
#define TRUE    1

int main( int ac, char *av[] )
{

   int n_tr[1];

   float beg[1],del,del_cp;
   float **yarray;
   

   int  nerr, max ;
   char str_file[500];


   float stla,stlo,evla,evlo; 

   float eepi[2],spos[2]  ;
   float delt,deltdg,deltkm,azes,azesdg,azse,azsedg ;
   int geograph=0 ;
   float pi;

   pi=4.0*atan(1);
    
   
 
   yarray=matrixf(  1,  N_MAX);
 

    strcpy(str_file,av[1]);

    evlo=atof(av[2]);
    evla=atof(av[3]);


    rsac1( str_file, yarray[0], &n_tr[0], &beg[0], &del_cp, &max, &nerr, strlen( str_file ) );


    getfhv("STLA",   &stla,     &nerr, strlen("STLA"));

    getfhv("STLO",   &stlo,     &nerr, strlen("STLO"));

//   printf("evlo: %f evla: %f stlo: %f stla: %f   error:%d\n",evlo,evla,stlo,stla, nerr);




    eepi[0]=evlo;
    eepi[1]=evla;
    spos[0]=stlo;
    spos[1]=stla;




    geograph=0 ;
    delaz5_(&eepi[1],&eepi[0],&spos[1],&spos[0],&delt,&deltdg,
	  &deltkm,&azes,&azesdg,&azse,&azsedg,&geograph);
//    printf( "%f\n", deltkm) ;
    printf( "%d\n", (int)deltkm) ;


    return 1;
    
    

}




float **matrixf( long nrh,  long nch)
/* allocate a float matrix with subscript range m[0..nrh][0..nch] */
{
	long i, nrow=nrh+1,ncol=nch+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow)*sizeof(float*)));
	if (!m) fprintf(stderr,"allocation failure");

	/* allocate rows and set pointers to them */
	m[0]=(float *) malloc((size_t)((nrow*ncol)*sizeof(float)));
	if (!m[0]) fprintf(stderr,"allocation failure");

	for(i=1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}



