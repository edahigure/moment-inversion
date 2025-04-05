#include<sys/file.h>
#include<fcntl.h>
#include<stdio.h>
#include <math.h>
#include <stdlib.h>

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


/*convert SAC binary to headerless binary file (fromHelm.c generated) */
/*or Helmberger ascii */
int main(ac,av)
 int ac;
 char **av;
 {
  int i, npts, ihd[40],fd1, fd2, binflag=1;
  float dt, fhd[70], *tr;
  char **chd, *in, *out, *line;

  in = (char*) malloc((100)*sizeof(char) );
  out = (char*) malloc((100)*sizeof(char) );
  line = (char*) malloc((100)*sizeof(char) );
  chd=char_matrix( 0, 7, 0, 23);

  setpar(ac,av);
  mstpar("in","s",in);
  mstpar("out","s",out);
  getpar("binflag","d",&binflag);
  endpar();

  fd1=open(in,O_RDONLY,0644);
  fd2=open(out,O_WRONLY | O_CREAT | O_TRUNC,0644);


  read(fd1,fhd,70*4);  /*Read Sac Float Field*/
  read(fd1,ihd,40*4);  /*Read Sac Int   Field*/
  read(fd1,chd,24*8);  /*Read Sac Char. Field*/
  npts=ihd[9];
  dt=fhd[0];
  fprintf(stderr,"npts=%d   dt=%f\n",npts,dt);

  tr=(float *)malloc(sizeof(float)*npts);
  read(fd1,tr,npts*sizeof(float));
  



  
  write(fd2,tr,npts*4); /*Write timeseries file*/

  if(binflag != 1)      /*Make Helmberger File*/
  {
     fprintf(stderr,"Making Helmberger Ascii\n");
     sprintf(line,"mkHelm ntr=1 dt=%f nt=%d < %s > %s.helm",dt,npts,out,out);
     system(line);
     sprintf(line,"rm %s",out);
     system(line);
   }
  fprintf(stderr,"Here Ok sac2bin\n");
}


