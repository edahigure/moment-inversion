/* copyright (c) Robert W. Clayton
 *		 Seismological Laboratory
 *		 Caltech
 *		 Pasadena, CA 91125
 *
 * Getpar routines:
 *
 * Externally visable routines:
 *
 *		setpar(argc,argv)
 *		getpar(name,type,valptr)
 *		mstpar(name,type,valptr)
 *		endpar()
 *
 * To get C-version:
 *		cc -c getpar.c
 *
 * To get F77-version:
 *		cp getpar.c fgetpar.c
 *		cc -c -DFORTRAN fgetpar.c
 *		rm fgetpar.c
 *
 * To get the environment processing stuff add the flag
 *-DENVIRONMENT to each of the cc's above.
 */
#include	<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAXLINE		1024	/* max length of line in par file */
#define MAXNAME		64	/* max length of name */
#define MAXVALUE	1024	/* max length of value */
#define MAXFILENAME	64	/* max length of par file name */
#define MAXVECTOR	10	/* max # of elements for unspecified vectors */
#define GETPAR_ERROR	100	/* exit status for getpar error */
#define GETPAR_STOP	101	/* exit status for STOP or mstpar */
#define MAXPARLEVEL	4	/* max recurrsion level for par files */

#ifdef FORTRAN
#define GETPAR	getpar_
#define MSTPAR	mstpar_
#define ENDPAR	endpar_
#else
#define GETPAR	getpar
#define MSTPAR	mstpar
#define ENDPAR	endpar

#define GETPAR_S	getpar_s
#define MSTPAR_S	mstpar_s
#define ENDPAR_S	endpar_s

#define GETPAR_D	getpar_d
#define MSTPAR_D	mstpar_d
#define ENDPAR_D	endpar_d


#define GETPAR_F	getpar_f
#define MSTPAR_F	mstpar_f
#define ENDPAR_F	endpar_f


#endif

#define INIT	 1	/* bits for FLAGS (ext_par.argflags) */
#define STOP	 2
#define LIST	 4
#define END_PAR	 8
#define VERBOSE	16

#define LISTINC		32	/* increment size for arglist */
#define BUFINC		1024	/* increment size for argbuf */

struct arglist		/* structure of list set up by setpar */
   {
	char *argname;
	char *argval;
	int hash;
   };
struct ext_par		/* global variables for getpar */
   {
	char *progname;
	int argflags;
	struct arglist *arglist;
	struct arglist *arghead;
	char *argbuf;
	int nlist;
	int nbuf;
	int listmax;
	int bufmax;
	FILE *listout;
   }	ext_par;

/* abbreviations: */
#define AL 		struct arglist
#define PROGNAME	ext_par.progname
#define FLAGS		ext_par.argflags
#define ARGLIST		ext_par.arglist
#define ARGHEAD		ext_par.arghead
#define ARGBUF		ext_par.argbuf
#define NLIST		ext_par.nlist
#define NBUF		ext_par.nbuf
#define LISTMAX		ext_par.listmax
#define BUFMAX		ext_par.bufmax
#define LISTFILE	ext_par.listout


#ifdef FORTRAN
setpar_();
#else
void setpar(int ac,char **av);		/* set up arglist & process INPUT command */
#endif


void gp_add_entry(register char *name,register char *value);	/* add an entry to arglist, expanding memory */
/* if necessary */
void gp_do_environment(int ac,char **av);


#ifdef FORTRAN
int mstpar_(char *name,char *type,int *val,int dum1,int dum2);
#else
int mstpar(char *name,char *type,int *val);
#endif

#ifdef FORTRAN
int mstpar_s_(char *name,char *type,char *val,int dum1,int dum2);
#else
int mstpar_s(char *name,char *type,char *val);
#endif

#ifdef FORTRAN
int mstpar_d_(char *name,char *type,double *val,int dum1,int dum2);
#else
int mstpar_d(char *name,char *type,double *val);
#endif

#ifdef FORTRAN
int mstpar_f_(char *name,char *type,float *val,int dum1,int dum2);
#else
int mstpar_f(char *name,char *type,float *val);
#endif



#ifdef FORTRAN
int getpar_(char *name,char *type,int *val,int dum1,int dum2);
#else
int getpar(char *name,char *type,int *val);
#endif


#ifdef FORTRAN
int getpar_s_(char *name,char *type,char *val,int dum1,int dum2);
#else
int getpar_s(char *name,char *type,char *val);
#endif


#ifdef FORTRAN
int getpar_d_(char *name,char *type,double *val,int dum1,int dum2);
#else
int getpar_d(char *name,char *type,double *val);
#endif


#ifdef FORTRAN
int getpar_f_(char *name,char *type,float *val,int dum1,int dum2);
#else
int getpar_f(char *name,char *type,float *val);
#endif







FILE *gp_create_dump(char *fname,char *filetype);

int gp_compute_hash(register char *s);

void gp_do_par_file(char *fname,int level);

void gp_close_dump(FILE *file);



int gp_getvector(char *list,char *type,int *val);
int gp_getvector_s(char *list,char *type,char *val);
int gp_getvector_d(char *list,char *type,double *val);
int gp_getvector_f(char *list,char *type,float *val);

void gp_getpar_err(char *subname,char *mess);

void endpar(void);
