#include <stdio.h>
#include <stdlib.h>


void main(int argc, char** av)
{
   int year,month,day,jday;
   year=atoi(av[1]);
   month=atoi(av[2]);
   day=atoi(av[3]);
//   printf("julian day: year: %d  month: %d  day :%d\n\n",year,month,day);

   if(year%4!=0){
//   printf("%d\n",year%4);
       if(month==1){
//   printf("%d\n",month);          
          if(day>=1 && day<=31 ){
             jday=day;
             
          }
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==2){
          
          if(day>=1 && day<=28)
          jday=31+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==3){
          
          if(day>=1 && day<=31)
          jday=59+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==4){
          
          if(day>=1 && day<=30)
          jday=90+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==5){
          
          if(day>=1 && day<=31)
          jday=120+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==6){
          
          if(day>=1 && day<=30)
          jday=151+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==7){
          
          if(day>=1 && day<=31)
          jday=181+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==8){
          
          if(day>=1 && day<=31)
          jday=212+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==9){
          
          if(day>=1 && day<=30)
          jday=243+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==10){
          
          if(day>=1 && day<=31)
          jday=273+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==11){
          
          if(day>=1 && day<=30)
          jday=304+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==12){
          
          if(day>=1 && day<=31)
          jday=334+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

      printf("%d\n",jday);  
   
   }



   if(year%4==0)
   {
//       printf("%d\n",year%4);
       if(month==1){
//        printf("%d\n",month);          
          if(day>=1 && day<=31 ){
             jday=day;
             
          }
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==2){
          
          if(day>=1 && day<=29)
          jday=31+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==3){
          
          if(day>=1 && day<=31)
          jday=60+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==4){
          
          if(day>=1 && day<=30)
          jday=91+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==5){
          
          if(day>=1 && day<=31)
          jday=121+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==6){
          
          if(day>=1 && day<=30)
          jday=152+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==7){
          
          if(day>=1 && day<=31)
          jday=182+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==8){
          
          if(day>=1 && day<=31)
          jday=213+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==9){
          
          if(day>=1 && day<=30)
          jday=244+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }
       if(month==10){
          
          if(day>=1 && day<=31)
          jday=274+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==11){
          
          if(day>=1 && day<=30)
          jday=305+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }

       if(month==12){
          
          if(day>=1 && day<=31)
          jday=335+day;
          else{
          fprintf(stderr,"wrong date\n");
          jday=0;
          }

       }


      printf("%03d\n",jday);  
   
   }


        

}

