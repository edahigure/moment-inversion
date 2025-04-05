#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define ABS(x) ((x) < 0.0 ? -1.0*(x) : (x))
#define	IS_LEAP(yr)	( yr%400==0 || (yr%4==0 && yr%100!=0) )

int getenv_d ();

static char months_days[2][13] = {
	{0,31,28,31,30,31,30,31,31,30,31,30,31},
	{0,31,29,31,30,31,30,31,31,30,31,30,31}
};


FILE *fopen(), *in, *stin, *mtout, *mtoutin, *pagefile;
int jdayest();
struct station {
  char name[5];
  float lat;
  float lon;
  float dist;
  float az;
  int   flag;
  };

main()
  {
  int redi_id, hr, mn, mo, dy, year, jday, statcnt, statmax, slen,cnt,ccnt;
  int READTEST,page=0;
  int i,j,geograph=0,dist,z,min1f,min2f,min3f, Eflag, Rerror;
  int sacfilelen, fd,plotflag;
  float min1,min2,min3;
  float lowcorner, highcorner;
  float mag,lat,lon,depth,slat,slon,tmp,zmin,sec;
  float delt, deltdg, deltkm, azes, azesdg, azse, azsedg;
  char CONFIGFILE[120],line[120], line1[12], line2[12], time[10], date[10],gftype[6];
  char *stationfile, *statuselist,**stations,*prgmpath,*dstream;
/*SAC HEADER*/
  int   ihd[40];
  float fhd[70];
  char  chd[8][24];
/*SAC HEADER*/

  struct station *STAT;


/*****Block to read station information and define stations to be used*****/
  stationfile=(char *)malloc(sizeof(char)*120);
  statuselist=(char *)malloc(sizeof(char)*120);
  prgmpath   =(char *)malloc(sizeof(char)*120);
  dstream    =(char *)malloc(sizeof(char)*4);
  getenv_s ("REDI_MT_STATLIST", &stationfile);
  getenv_s ("REDI_MT_PROG1_STATLIST", &statuselist);
  getenv_s ("REDI_MT_BINDIR", &prgmpath);
  getenv_s ("REDI_MT_DATASTREAM", &dstream);
  getenv_d ("REDI_MT_PROG1_PAGE", &page);
  getenv_d ("REDI_MT_PROG1_NSTAT", &statcnt);
  getenv_d ("REDI_MT_PROG1_STATMAX", &statmax);
  getenv_d ("REDI_MT_PLOT", &plotflag);

  if(!strncmp(dstream,"BH",2))
    sacfilelen=6000;
  if(!strncmp(dstream,"LH",2))
    sacfilelen=300;
     

  STAT=(struct station *)malloc(sizeof(struct station)*statmax);

  stations=(char **)malloc(sizeof(char *)*statmax);
  for(i=0; i < statmax; i++)
    stations[i]=(char *)malloc(sizeof(char)*6);

  slen=0;
  for(i=0; i < statcnt; i++)
    {
    sscanf((statuselist+slen),"%s",stations[i]);
    fprintf(stderr,"%s\n",stations[i]);
    slen += strlen(stations[i]);
    slen++;
    }

  ccnt=0;
  stin=fopen(stationfile,"r");
  while(fgets(line,120,stin) != NULL)
     {
     sscanf(line,"%s %f %f",STAT[ccnt].name,&STAT[ccnt].lat,&STAT[ccnt].lon);
     STAT[ccnt].dist=STAT[ccnt].az=0.0;
     STAT[ccnt].flag=-1;

     for(j=0; j < statcnt; j++)
	if(!strncmp(STAT[ccnt].name,stations[j],3)) STAT[ccnt].flag=0;

     fprintf(stderr,"%s %f %f %d\n",STAT[ccnt].name,STAT[ccnt].lat,STAT[ccnt].lon,STAT[ccnt].flag);
      
     ccnt++;
     if(ccnt > statmax)
	      {
	      fprintf(stderr,"Error: Number of stations exceeds statmax\n");
	      exit(-1);
	      }
     }
     fclose(stin);
     statmax=ccnt;
/*****End block to read station information and define stations to be used*****/


  if((in=fopen("tdmt_redi_sched.in","r")) == NULL)
     {
     fprintf(stdout,"There is no tdmt_redi_sched.in file\n");
     exit(-1);
     }
     fscanf(in,"%s %d %d %d %d %f %f %f %f %f %d",CONFIGFILE,&year,&jday,
	    &hr,&mn,&sec,&lat,&lon,&depth,&mag,&redi_id);

     if(mag < 3.9)
       { 
       lowcorner=0.02;
       highcorner=0.10;
       }
     else if(mag >= 3.9 && mag < 5.0)
       { 
       lowcorner=0.02;
       highcorner=0.05;
       }
     else if(mag >= 5.0)
       { 
       lowcorner=0.01;
       highcorner=0.05;
       }
       
       
     ydoy2mdy(year,jday,&mo,&dy);    /*Calculate month and day and construct time string*/
     sscanf(time,"%d:%d",&hr,&mn);
     sscanf(date,"%d/%d",&mo,&dy);
     sprintf(time,"%04d%02d%02d%02d%02d00",year,mo,dy,hr,mn);


    /*GET DATA WRITE TO SAC and CHECK FOR APPROPRIATE LENGTH FILES*/
      sprintf(line,"%s/tdmt_redi_getdata %03d %02d%02d %d %02d %02d 5 > msgs.getdata",
      prgmpath,jday,mo,dy,year,hr,mn);
      fprintf(stdout,"%s\n",line);
      system(line);   

      for(i=0; i < statmax;i++) /* Check for existance of SAC files*/
	  {
	  /*CHECK VERTICAL COMPONENT FILES*/
	  sprintf(line,"%s.%s.%sZ",time,STAT[i].name,dstream);
	  if((fd=open(line,O_RDONLY, 0)) == -1)
	    {
	    fprintf(stdout,"Cannot open SAC file: %s\n",line);
	    STAT[i].flag=-1;
	    }
          if(STAT[i].flag != -1)
	   {
	   READTEST=read(fd,fhd,70*4);
	   READTEST=read(fd,ihd,40*4);
	       if(READTEST == 0 || READTEST == -1 || ihd[9] != sacfilelen)
	         {
	         fprintf(stdout,"SAC file %s incorrect length: %d\n",line,ihd[9]);
	         STAT[i].flag=-1;
	         }
	   close(fd);
	   }

	  /*CHECK NS COMPONENT FILES*/
	  sprintf(line,"%s.%s.%sN",time,STAT[i].name,dstream);
	  if((fd=open(line,O_RDONLY, 0)) == -1)
	    {
	    fprintf(stdout,"Cannot open SAC file: %s\n",line);
	    STAT[i].flag=-1;
	    }
          if(STAT[i].flag != -1)
	   {
	   READTEST=read(fd,fhd,70*4);
	   READTEST=read(fd,ihd,40*4);
	       if(READTEST == 0 || READTEST == -1 || ihd[9] != sacfilelen)
	         {
	         fprintf(stdout,"SAC file %s incorrect length: %d\n",line,ihd[9]);
	         STAT[i].flag=-1;
	         }
	   close(fd);
	   }

	  /*CHECK EW COMPONENT FILES*/
	  sprintf(line,"%s.%s.%sE",time,STAT[i].name,dstream);
	  if((fd=open(line,O_RDONLY, 0)) == -1)
	    {
	    fprintf(stdout,"Cannot open SAC file: %s\n",line);
	    STAT[i].flag=-1;
	    }
          if(STAT[i].flag != -1)
	   {
	   READTEST=read(fd,fhd,70*4);
	   READTEST=read(fd,ihd,40*4);
	       if(READTEST == 0 || READTEST == -1 || ihd[9] != sacfilelen)
	         {
	         fprintf(stdout,"SAC file %s incorrect length: %d\n",line,ihd[9]);
	         STAT[i].flag=-1;
	         }
	   close(fd);
	   }

	  }
    /*GET DATA WRITE TO SAC and CHECK FOR APPROPRIATE LENGTH FILES*/

     min1f=min2f=min3f=statmax+1;
     min1=min2=min3=1000.0;
     for(i=0; i < statmax;i++)/*STATION DISTANCE FILTER LOOP*/
       {
     fprintf(stderr,"%s %f %f %d\n",STAT[i].name,STAT[i].lat,STAT[i].lon,STAT[i].flag);
       slat=STAT[i].lat;
       slon=STAT[i].lon;
       delaz5_(&lat,&lon,&slat,&slon,&delt,&deltdg,
		      &deltkm,&azes,&azesdg,&azse,&azsedg,&geograph);

       STAT[i].dist=deltkm;
       STAT[i].az  =azesdg;
       fprintf(stdout,"%s  %f\n",STAT[i].name,STAT[i].dist);

       if(deltkm > 45.0 && deltkm < 400.0 && STAT[i].flag != -1) /*GET NEARREGIONAL RANGES*/
	  {
       if(deltkm < min1)                   /*FIND THREE CLOSEST DISTANCES*/
	   {
	   min3=min2;
	   min3f=min2f;
	   min2=min1;
	   min2f=min1f;
	   min1=deltkm;
	   min1f=i;
	   }
       if(deltkm < min2 && deltkm > min1)
	   {
	   min3=min2;
	   min3f=min2f;
	   min2=deltkm;
	   min2f=i;
	   }
       if(deltkm < min3 && deltkm > min2)
	   {
	   min3=deltkm;
	   min3f=i;
	   }
	  }
       }/*END STATION TEST LOOP*/
       fprintf(stdout,"min1=%.3f\nmin2=%.3f\nmin3=%.3f\n",min1,min2,min3);
       fprintf(stdout,"min1f=%d\nmin2f=%d\nmin3f=%d\n",min1f,min2f,min3f);

     cnt=0;
     for(i=0; i < statmax;i++)                        /*PREP SELECT DATA*/
       {
          if((i == min1f || i == min2f || i == min3f) && STAT[i].flag != -1)
	   {
	   cnt++;
	   STAT[i].flag=1;
           sprintf(line,"%s/tdmt_redi_prepdata %s %s %d %03d %f %f %.2f %.2f > msgs.prepdata",prgmpath,time,STAT[i].name,
	   year,jday,lat,lon,lowcorner,highcorner);
           fprintf(stdout,"%s\n",line);
           system(line);   
	   }
       } 




      for(i=0; i < statmax; i++)          /*GRAB APPROPRIATE SYNTHETICS*/
	 {
	 if(STAT[i].flag == 1)
	   {
           dist = (int)(STAT[i].dist/5.0 + 0.5)*5;
       fprintf(stdout,"%s dist=%d\n",STAT[i].name, dist);

	   if(lat < 39.0 && lat >= 35.0 && lon < (-119.0 -(lat-35.0)*0.75))
	     sprintf(gftype,"gil7_");
	   else if(lat < 42.0 && lat >= 39.0 && lon < (-122.0 -(lat-39.0)*0.33))
	     sprintf(gftype,"gil7_");
           else sprintf(gftype,"sc");

	   if(!strcmp(gftype,"sc"))
	     {
	     sprintf(line,"%s/tdmt_redi_prepsyn %s %d %.2f %.2f > msgs.prepsyn",prgmpath,gftype,dist,lowcorner,highcorner);
             fprintf(stdout,"%s\n",line);
	     system(line);
	     }

	   if(!strcmp(gftype,"gil7_"))
	     {
	     sprintf(line,"%s/tdmt_redi_prepsyn %s %d %.2f %.2f > msgs.prepsyn",prgmpath,gftype,dist,lowcorner,highcorner);
             fprintf(stdout,"%s\n",line);
	     system(line);
	     }
	   }
	 }

      if(cnt==0)
	 {
	 fprintf(stdout,"Station count %d  Quitting\n",cnt);
	 exit(-1);
	 }


   /*PREPARE mt_inv.in file for sc model loop over depth and perform inversion*/
   if(!strcmp(gftype,"sc"))
     {
  /* for(j=5; j < 18; j += 3)  */
     for(j=5; j < 40; j += 3)
	{
	if(j==20) j++;;  /*To correct for depth sampling of GFS*/
        mtout=fopen("mt_inv.in","w");
        fprintf(mtout,"%d %d 1 %d\n",cnt,j,plotflag);

        for(i=0; i < statmax; i++) 
	 if(STAT[i].flag == 1)
	  if(highcorner < 0.10)            /*WRITE mt_inv.in file DATA LINES*/
	    fprintf(mtout,"%s_f%.2f.data %.1f  %.1f 0 120\n",STAT[i].name,highcorner,STAT[i].dist,STAT[i].az); 
	  else 
	    fprintf(mtout,"%s_f%.2f.data %.1f  %.1f 0 60\n",STAT[i].name,highcorner,STAT[i].dist,STAT[i].az); 

        for(i=0; i < statmax; i++) 
	 if(STAT[i].flag == 1)
	  {
          dist = (int)(STAT[i].dist/5.0 + 0.5)*5;
          if(highcorner < 0.10)           /*WRITE mt_inv.in file SYNTHETIC LINES*/
   	    fprintf(mtout,"%s%dd%d 0 120\n",gftype,dist,j);
          else
   	    fprintf(mtout,"%s%dd%d 0 60\n",gftype,dist,j);
          }

	 fclose(mtout);
         sprintf(line,"%s/tdmt_invc > plot_d%d",prgmpath,j);
         fprintf(stdout,"%s\n",line);
         system(line);
       }
     }

   /*PREPARE mt_inv.in file for gil7 model loop over depth and perform inversion*/
   if(!strcmp(gftype,"gil7_"))
     {
  /* for(j=5; j < 18; j += 3) */
     for(j=5; j < 40; j += 3)
	{
	if(j==17) j++;
        mtout=fopen("mt_inv.in","w");
        fprintf(mtout,"%d %d 1 %d\n",cnt,j,plotflag);

        for(i=0; i < statmax; i++) 
	 if(STAT[i].flag == 1)
	  if(highcorner < 0.10)            /*WRITE mt_inv.in file DATA LINES*/
	    fprintf(mtout,"%s_f%.2f.data %.1f  %.1f 0 120\n",STAT[i].name,highcorner,STAT[i].dist,STAT[i].az); 
	  else 
	    fprintf(mtout,"%s_f%.2f.data %.1f  %.1f 0 60\n",STAT[i].name,highcorner,STAT[i].dist,STAT[i].az); 

        for(i=0; i < statmax; i++) 
	 if(STAT[i].flag == 1)
	  {
          dist = (int)(STAT[i].dist/5.0 + 0.5)*5;
          if(highcorner < 0.10)           /*WRITE mt_inv.in file SYNTHETIC LINES*/
   	    fprintf(mtout,"%s%dd%d 0 120\n",gftype,dist,j);
          else
   	    fprintf(mtout,"%s%dd%d 0 60\n",gftype,dist,j);
          }

	 fclose(mtout);
         sprintf(line,"%s/tdmt_invc > plot_d%d",prgmpath,j);
         fprintf(stdout,"%s\n",line);
         system(line);
       }
     }







       /*Read mt_inv.out and create page message file*/
       makepage(redi_id,time,lat,lon,depth,cnt);

       if(page)
	 {
         system("mailx -s TDMT user@localhost < pager.file");
	 } else {
	    fprintf(stderr,"NO PAGE OPTION ON\n");}




  }/*End*/


/* Subroutine to compute day of year
	based on Kernighan and Ritchie, p.111

modified from dyr.c
	Greg Anderson
	UCB Seismographic Station
	11 November 1992

*/


int jdayest(mo,dy,year)
  int mo,dy,year;
  {
	int doy;
        
	/*year += CENTURY;*/
        mdy2ydoy(mo,dy,year,&doy);
	return(doy);
  }

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


/* function mdy2ydoy -- calculate day of year from year, month, and day */
mdy2ydoy(month,day,year,doy)
int month,day,year,*doy;
{
	int i;
	
	*doy = day;
	for (i=1;i<month;i++)
		*doy += months_days[IS_LEAP(year)][i];
} 
/* end function mdy2ydoy */	



/*Program to strip out the hottest MT info and page dreger*/
makepage(id,time,lat,lon,z,nstations)
   int id,nstations;
   float lat, lon, z;
   char *time;
   {
   int Z, Zsave;
   int methodtag=1,ncomponents;
   int pdc,pclvd,qual,pdcsave,pclvdsave,qualsave;
   float Mo, Mofactor, Mw, S1, S2, R1, R2, D1, D2;
   float Mosave, Mwsave, S1save, S2save, R1save, R2save, D1save, D2save;
   float mxx,mxy,mxz,myy,myz,mzz,tau=0.0;
   float mxxsave,mxysave,mxzsave,myysave,myzsave,mzzsave;
   float VR, VRtest=0.0;
   char rstring[120];

   ncomponents=3*nstations;

   if((mtoutin=fopen("mt_inv_redi.out","r")) == NULL)
      {
      fprintf(stdout,"Unable to open mt_inv.out\n");
      exit(-1);
      }

   while(fgets(rstring,120,mtoutin) != NULL)
       {
       if(!strncmp(rstring,"Depth=",6))
	 sscanf(rstring,"Depth=%d",&Z);
       if(!strncmp(rstring,"Mo=",3))
	 sscanf(rstring,"Mo=%e",&Mo);
       if(!strncmp(rstring,"Mw=",3))
	 sscanf(rstring,"Mw=%f",&Mw);
       if(!strncmp(rstring,"Strike=",7))
	 sscanf(rstring,"Strike=%f ; %f",&S1,&S2);
       if(!strncmp(rstring,"Rake=",5))
	 sscanf(rstring,"Rake=%f ; %f",&R1,&R2);
       if(!strncmp(rstring,"Dip=",4))
	 sscanf(rstring,"Dip=%f ; %f",&D1,&D2);
       if(!strncmp(rstring,"VarRed=",7))
	 sscanf(rstring,"VarRed=%f",&VR);
       if(!strncmp(rstring,"Mxx=",4))
	 sscanf(rstring,"Mxx=%f",&mxx);
       if(!strncmp(rstring,"Mxy=",4))
	 sscanf(rstring,"Mxy=%f",&mxy);
       if(!strncmp(rstring,"Mxz=",4))
	 sscanf(rstring,"Mxz=%f",&mxz);
       if(!strncmp(rstring,"Myy=",4))
	 sscanf(rstring,"Myy=%f",&myy);
       if(!strncmp(rstring,"Myz=",4))
	 sscanf(rstring,"Myz=%f",&myz);
       if(!strncmp(rstring,"Mzz=",4))
	 sscanf(rstring,"Mzz=%f",&mzz);
       if(!strncmp(rstring,"Pdc=",4))
	 sscanf(rstring,"Pdc=%d",&pdc);
       if(!strncmp(rstring,"Pclvd=",6))
	 sscanf(rstring,"Pclvd=%d",&pclvd);
       if(!strncmp(rstring,"Quality=",8))
	 sscanf(rstring,"Quality=%d",&qual);

       if(VR > VRtest)  /*Save best depth*/
	   {
	   Mosave=Mo;
	   Mwsave=Mw;
	   S1save=S1;
	   S2save=S2;
	   R1save=R1;
	   R2save=R2;
	   D1save=D1;
	   D2save=D2;
	   VRtest=VR;
           Zsave=Z;
	   mxxsave=mxx;
	   mxysave=mxy;
	   mxzsave=mxz;
	   myysave=myy;
	   myzsave=myz;
	   mzzsave=mzz;
	   qualsave=qual;
	   pdcsave=pdc;
	   pclvdsave=pclvd;
	   }
       }
 
   sprintf(rstring,"REDIMT:%d %.1f %s %.2f %.2f %.1f %.2E %.0f,%.0f,%.0f  %.0f,%.0f,%.0f %d VR%.1f",
    id, Mwsave, time, lat, lon, z, Mosave, S1save, R1save, D1save, S2save, R2save, D2save, Zsave, VRtest);

   fprintf(stdout,"%s\n",rstring);
   pagefile=fopen("pager.file","w");
   fprintf(pagefile,"%s",rstring);
   fclose(pagefile);
   fclose(mtoutin);


/*Write output file to be used by REDI*/
   Mofactor = Mosave/1.0e+20;
	   mxxsave /= Mofactor;
	   mxysave /= Mofactor;
	   mxzsave /= Mofactor;
	   myysave /= Mofactor;
	   myzsave /= Mofactor;
	   mzzsave /= Mofactor;

   pagefile = fopen("FILE1","w");

   fprintf(pagefile,"%d \n1 \n%7.2e \n%4.2f \n%6.3f \n%6.3f \n%6.3f \n%6.3f \n%6.3f \n%6.3f \n%6.1f \n%4.1f \n%6.1f \n%6.1f \n%4.1f \n%6.1f \n",
	id,Mosave,Mwsave,mxxsave,mxysave,mxzsave,myysave,myzsave,
	 mzzsave,S1save,D1save,R1save,S2save,D2save,R2save);
   fprintf(pagefile,"%2d.\n%4.1f\n%2d\n%2d\n%4d\n%5d\n%2d\n",
	    Zsave,tau,nstations,ncomponents,pdcsave,pclvdsave,qualsave);
   fclose(pagefile);

   }
