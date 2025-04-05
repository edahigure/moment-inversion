#include<stdio.h>
#include<stdlib.h>

void main( int argc, char *av[] )
{

    double R1,R2,R;
            
    R1   =atof(av[1]);

    R2   =atof(av[2]);


    if(R1>=R2)
    printf("%d\n",1);
    else
    printf("%d\n",0);

}

