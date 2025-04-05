#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>



void main( int argc, char *av[] )
{

    char dir[500];
    double z_src,R;
    char outfile[500];
    char infile_1[500];
    char command[500];

    strcpy(outfile,av[1]);
    strcpy(dir,av[2]);
    R=atof(av[3]);
    z_src=atof(av[4]);


    sprintf(infile_1,"%s/cs%03dd%03d.disp",dir,(int)floor(R),(int)floor(z_src));
    sprintf(command,"cp  %s %s",infile_1,outfile);
    printf("%s\n",command);
    system(command);
   
}

