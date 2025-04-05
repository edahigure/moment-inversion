#include <stdio.h>
#include <sys/file.h>
#include <string.h>
#define ABS(x) ((x) < 0.0 ? -1.0*(x) : (x))
#define IS_LEAP(yr)     ( yr%400==0 || (yr%4==0 && yr%100!=0) )

static char months_days[2][13] = {
         {0,31,28,31,30,31,30,31,31,30,31,30,31},
	 {0,31,29,31,30,31,30,31,31,30,31,30,31}
};

/*Program to read redi files extracted from mail and create a event file*/

FILE *fopen(), *in,*out;
int jdayest();
void mdy2ydoy();

main()
  {
  int redi_id, hr, mn, mo, dy, year=1999, jday, cnt;
  int Eflag, Rerror;
  float mag,lat,latmin,lon,lonmin,depth,slat,slon,tmp,zmin, sec;
  char line[120], line1[120],line2[120], time[10],date[10];
  char *prgmpath, *datapath;

  getenv_s ("REDI_MT_BINDIR", &prgmpath);
  getenv_s ("REDI_MT_DATDIR", &datapath);

  sprintf(line,"%s/redi_trigger",datapath);
  if((in=fopen(line,"r")) == NULL)
     {
     fprintf(stdout," is no redi_trigger file: %s\n",line);
     exit(-1);
     }

  Rerror=0;                       /*Flag to signal appropriate message type*/
  while(fgets(line,120,in) != NULL)
     {
     sscanf(line,"%s %s",line1, line2);
     if(!strcmp(line1,"REDI") && !strcmp(line2,"Preliminary"))
	  {
	  fprintf(stdout,"%s\n %s %s\n",line,line1,line2);
	  fgets(line,120,in);
	  fgets(line,120,in);
	  fgets(line,120,in);
	  fprintf(stdout,"%s\n",line);
	  mag=0.0;
	  redi_id=-1;
	  Eflag=sscanf(line,"%f %s %s %f %f %f %f %f %d",&mag,time,date,
	    &lat,&latmin,&lon,&lonmin,&depth,&redi_id);

/*BLOCK OF CODE ADDED TO GENERATE TDMT_REDI_SCHED.IN FILE*/
          lat += latmin/60.0;
          lon += lonmin/60.0;
	  lon *= -1.0;
	  sscanf(time,"%d:%d",&hr,&mn);
	  sscanf(date,"%d/%d",&mo,&dy);
          jday=jdayest(mo,dy,year);
          sprintf(line,"%s %d %d %d %d 0.0 %f %f %f %f %d","tdmt.config",year,jday,
		      hr,mn,lat,lon,depth,mag,redi_id);


	  if(mag > 3.49 && redi_id > 0)
	   {
           sprintf(line1,"%s/GUIDE/%04d",datapath,redi_id);
           out=fopen(line1,"w");
	   fprintf(out,"%s\n",line);
           Rerror=1;
	   strcpy(line1,"RESET");
	   strcpy(line2,"RESET");
	   }
	  else if(mag > 0.0 && redi_id > 0)
	   {
	   fprintf(stdout,"Redi event %04d is too small to proceed\n",redi_id);
           Rerror=1;
	   strcpy(line1,"RESET");
	   strcpy(line2,"RESET");
	   }
	  else if(redi_id < 0)
	   {
	   fprintf(stdout,"Not a redi event message\n");
           Rerror=1;
	   strcpy(line1,"RESET");
	   strcpy(line2,"RESET");
	   }
	    fprintf(stdout,"HERE\n");
	    

	  }
     } /*End While*/

}

int jdayest(mo,dy,year)
int mo,dy,year;
{
int doy;

mdy2ydoy(mo,dy,year,&doy);
return(doy);
}

/* function mdy2ydoy -- calculate day of year from year, month, and day */
void mdy2ydoy(month,day,year,doy)
int month,day,year,*doy;
{
int i;

*doy = day;
for (i=1;i<month;i++)
	*doy += months_days[IS_LEAP(year)][i];
}
/* end function mdy2ydoy */

ydoy2mdy(year,doy,month,day)
int *month,*day,year,doy;
    {
    int i,yearday;

    yearday = doy;
    for (i=1;yearday > months_days[IS_LEAP(year)][i];i++)
    yearday -= months_days[IS_LEAP(year)][i];
    *month = i;
    *day = yearday;
     }
/* end function ydoy2mdy */
