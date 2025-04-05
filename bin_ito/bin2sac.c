#include<sys/file.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>


char **char_matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
    long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
    char **m;
    int NR_END=1;
    /* allocate pointers to rows */
    m=(char **) malloc((size_t)((nrow+NR_END)*sizeof(char*)));
    if (!m) printf("allocation failure 1 in matrix()");
    m += NR_END;
    m -= nrl;

    /* allocate rows and set pointers to them */
    m[nrl]=(char *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(char)));
    if (!m[nrl]) printf("allocation failure 2 in matrix()");
    m[nrl] += NR_END;
    m[nrl] -= ncl;

    for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

    /* return pointer to array of pointers to rows */
    return m;
}

/*convert headerless binary file (fromHelm.c generated) to SAC binary file */
int main(ac,av)
 int  ac;
 char **av;
 {
 int i, npts, year, jday, hour, min, sec, msec, ihd[40];
 float dt, B=0.0, E, fhd[70], *tr, evla, evlo, stla, stlo, dist;
 float azi, bazi, cmpaz, cmpinc;
 char **chd, *ename, *sname;
//[8][24]


ename = (char*) malloc((16)*sizeof(char) );
sname = (char*) malloc((8)*sizeof(char) );
chd=char_matrix( 0, 7, 0, 23);


/* Initialize Header */
for(i=0;i<40;i++) ihd[i]=-12345;

for(i=0;i<70;i++) fhd[i]=-12345.00;


for(i=0;i<8;i++)  sprintf(chd[i],"-12345  -12345  -12345  ");




sprintf(ename,"-12345  -12345  ");

sprintf(sname,"-12345  ");


/* Set Essential Parameters */
ihd[35]=1;                  /*Sets file to evenly spaced*/
ihd[15]=1;                  /*Sets file type to Timeseries*/
ihd[6]=6;                   /*Variable Name Internal */


setpar(ac,av);
mstpar("npts","d",&npts);
  ihd[9]=npts;
  
mstpar("dt","f",&dt);
  fhd[0]=dt;
if(getpar("stime","f",&B))
  fhd[5]=B;
if(getpar("year","d",&year))
   ihd[0]=year;
if(getpar("jday","d",&jday))
   ihd[1]=jday;
if(getpar("hour","d",&hour))
   ihd[2]=hour;
if(getpar("min","d",&min))
   ihd[3]=min;
if(getpar("sec","d",&sec))
   ihd[4]=sec;
if(getpar("msec","d",&msec))
   ihd[5]=msec;



getpar("ename","s",ename);         /*Event   name*/
getpar("sname","s",sname);         /*Station name*/



if(getpar("cmpaz","f",&cmpaz))
   fhd[57]=cmpaz;
if(getpar("cmpinc","f",&cmpinc))
   fhd[58]=cmpinc;


if(getpar("stla","f",&stla))       /*Station lat and lon.*/
   {
   fhd[31]=stla;
   fprintf(stderr,"%f\n",fhd[31]);
   }
   
   
if(getpar("stlo","f",&stlo))
   fhd[32]=stlo;
if(getpar("evla","f",&evla))          /*Event lat and lon.*/
   fhd[35]=evla;
if(getpar("evlo","f",&evlo))
   fhd[36]=evlo;
if(getpar("dist","f",&dist))
   fhd[50]=dist;
if(getpar("azi","f",&azi))
   fhd[51]=azi;
if(getpar("bazi","f",&bazi))
   fhd[52]=bazi;
endpar();


sprintf(chd[0],"%-8s%-16s",sname,ename);

fprintf(stderr,"bin2sac: %s  %s   chd[0]: %s \n",ename,sname,chd[0]);
  
fhd[6]=B+fhd[0]*(ihd[9]-1);        /*Set E variable*/
tr=(float *)malloc(sizeof(float)*npts);
read(0,tr,npts*sizeof(float));

for(int k=1;k<10;k++)
fprintf(stderr,"%d %e \n",npts,tr[k]);

write(1,fhd,70*4);  /*Write Sac Float Field*/
write(1,ihd,40*4);  /*Write Sac Int   Field*/
write(1,chd,24*8);  /*Write Sac Char. Field*/
write(1,tr,ihd[9]*4); /*Write timeseries file*/


}


