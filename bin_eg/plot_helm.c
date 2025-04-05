/* Moment Tensor Inversion */
/* Uses previously determined Green's functions in D. Helmbergers format*/

#include"readhelm.h"
#include<stdlib.h>


float **fmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;
   int NR_END=1;
	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	if (!m) printf("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) printf("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}


void main( int argc, char *argv[] )
{
   int i,j,l,N,Np,Z,k,ntr,vsize,nn,*npts;

   float  *tmp_2,*dt,junk;

   char infile[50],line[50];

   int  N_BIG;
   char gf_plot[500][50],command[50];
   float *u1,*u2,*u3,*u4,*u5,*u6,*u7,*u8,*u9,*u10;
   float **u;
   FILE *fp;
   static FILE  *fd, *fd1, *fd2, *fd3,  *out_gf;
   char str[500];

   strcpy( infile, argv[1] ) ;


   npts=(int *)malloc(sizeof(int)*10);
   dt  =(float *)malloc(sizeof(float)*10);


   N_BIG=10000;
   tmp_2=(float *)malloc((size_t) ( 8*N_BIG*sizeof(float) ) );

   fd=fopen(infile,"r");
   fgets(line,100,fd);
   sscanf(line,"%d",&N);
   fgets(line,100,fd);
   fgets(line,100,fd);
   fgets(line,100,fd);
   sscanf(line,"%d %f",&vsize,&junk);
   fclose(fd);


   readhelm(infile,&ntr,npts,dt,tmp_2);    /*Open and Read Green's Func*/
   nn=npts[0];


   u=fmatrix(0, N_BIG, 0, ntr);

   for(int k=0;k<ntr;k++){
   for(j=0 ; j < nn ; j++){
      u[j][k]=tmp_2[j+k*nn];
//    printf("%e\n",u[j][k]);
    }}


	for(i=0 ; i < ntr ; i++){
	sprintf( gf_plot[i], "gf%d.xy", i ) ;
//	fprintf(stderr,"%s\n",gf_plot[i]); 
   }


   for(int k=0;k<ntr;k++){
		out_gf=fopen(gf_plot[k],"w");  
		for(j=0 ; j < nn ; j++){
			fprintf(out_gf,"%f %e\n", dt[0]*j,u[j][k]); 
		}
      fclose(out_gf);
   }

   fp=fopen("Plot.gnu","w");
   fprintf(fp,"set terminal qt persist title \"GnuPlot\"\n");

   fprintf(fp,"unset key\n");
   fprintf(fp,"set grid\n");
   fprintf(fp,"set pointsize .3\n");
   strcpy(str,"");

   for(int k=0;k<ntr-1;k++){ 
    sprintf(str,"%s \"gf%d.xy\" ,",str,k);
   }
   sprintf(str,"%s \"gf%d.xy\"",str,ntr-1);
//   printf("%s",str);
   fprintf(fp,"plot %s using 1:2 with lines linecolor rgb \"blue\" ",str);

   fclose(fp);
   system("gnuplot Plot.gnu");

   for(int k=0;k<ntr;k++){ 
//    sprintf(str,"rm gf%d.xy",k);
//    system(str);
   }


//   system("rm Plot.gnu");

   return;


}











