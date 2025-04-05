#include<sys/file.h>
#include<stdio.h>
#include<string.h>

FILE *fopen(), *in, *out;
int getenv_s();

struct comp
  {
  float re;
  float im;
  };

main(ac,av)
 int *ac;
 char **av;
  {
  int i, n, nzero, npole,env;
  float gain, junk;
  int year1,jday1,hrmin1, year2,jday2,hrmin2;
  struct comp zero[20], pole[20];
  char line[120], tmp[50], tmp1[20], tmp2[20], tmp3[20], name[5], comp[5];
  char name1[5], comp1[5], padding_character='\0';
  char temp1[13], temp2[13];
  char *resp_name;

  resp_name=(char *)malloc(sizeof(char)*120);

  setpar(ac,av);
  mstpar("name","s",name);
  mstpar("comp","s",comp);
  endpar();

  
  if(! getenv_s("REDI_MT_RESP", &resp_name)) {
    env=1;
    }
  if(env==1)
     {
     fprintf(stderr,"Missing environment variable (source tdmt.config)\n");
     exit(-1);
     }
  if((in=fopen(resp_name,"r")) == NULL)
      {
      fprintf(stderr,"Error opening: %s\n",resp_name);
      exit(-1);
      }

  sprintf(tmp,"%s_%s.zp",name,comp);
  fprintf(stderr,"name=%s\n",tmp);

  if((out=fopen(tmp,"w")) == NULL)
      {
      fprintf(stderr,"Error opening %s\n",tmp);
      exit(-1);
      }


while(fgets(line,120,in) != NULL)
    {
    sscanf(line,"%4s %4s %d.%d.%d %d.%d.%d",
       name1,comp1,&year1,&jday1,&hrmin1,&year2,&jday2,&hrmin2);


    if (!strcmp(name,name1)  && !strcmp(comp,comp1))
     {
       fscanf(in,"%g %d %d\n",&gain,&nzero,&npole);
       fprintf(out,"CONSTANT %g\n",gain);

	  for(i=0 ; i < nzero ; i++)
	     fscanf(in,"%f %f\n",&zero[i].re,&zero[i].im);

       fprintf(out,"ZEROS %d\n",nzero);
	  for(i=0 ; i < nzero ; i++)
	     fprintf(out,"%.4e  %.4e\n",zero[i].re,zero[i].im);


	  for(i=0 ; i < npole ; i++)
	     {
	     if(i % 2 == 0)
	     fscanf(in,"%f %f\n",&pole[i].re,&pole[i].im);
	     else
	     fscanf(in,"%f %f",&pole[i].re,&pole[i].im);
	     }

       fprintf(out,"POLES %d\n",npole);
	  for(i=0 ; i < npole ; i++)
	     fprintf(out,"%.4e  %.4e\n",pole[i].re,pole[i].im);

       fclose(in);
       fclose(out);
       exit(-1);
       }

    } /*End While*/
  } /*END*/
