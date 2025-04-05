/* copyright (c) Robert W. Clayton
 *		 Seismological Laboratory
 *		 Caltech
 *		 Pasadena, CA 91125
 *
 * Getarg routines:
 *
 * Externally visable routines:
 *
 *		setarg(argc,argv)
 *		getarg(name,type,valptr)
 *		endarg()
 *
 * To get C-version:
 *		cc -c getarg.c
 *
 * To get F77-version:
 *		cp getarg.c fgetarg.c
 *		cc -c -DFORTRAN fgetarg.c
 *		rm fgetarg.c
 */
#include	<stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME		64	/* max length of name */
#define MAXVALUE	1024	/* max length of value */
#define MAXVECTOR	10	/* max # of elements for unspecified vectors */
#define GETARG_ERROR	102	/* exit status for getarg error */

#define INIT	 1	/* bits for FLAGS (ext_arg.argflags) */
#define END_PAR	 2

#define LISTINC		32	/* increment size for arglist */
#define BUFINC		1024	/* increment size for argbuf */

struct arglist		/* structure of list set up by setarg */
   {
	char *argname;
	char *argval;
	int hash;
   };
struct ext_arg		/* global variables for getarg */
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
   }	ext_arg;

/* abbreviations: */
#define AL 		struct arglist
#define PROGNAME	ext_arg.progname
#define FLAGS		ext_arg.argflags
#define ARGLIST		ext_arg.arglist
#define ARGHEAD		ext_arg.arghead
#define ARGBUF		ext_arg.argbuf
#define NLIST		ext_arg.nlist
#define NBUF		ext_arg.nbuf
#define LISTMAX		ext_arg.listmax
#define BUFMAX		ext_arg.bufmax



#ifdef FORTRAN
void setarg_(char *list,char *subname,int dum1,int dum2);
#else
void  setarg(char *list,char *subname);
#endif


void ga_add_entry(register char *name,register char *value);	/* add an entry to arglist, expanding memory */

#ifdef FORTRAN
void endarg_(void);
#else
void endarg(void); /* free arglist & argbuf memory, & process STOP command */
#endif


#ifdef FORTRAN
int getarg_(char *name,char *type,int *val,int dum1,int dum2);
/* dum1 & dum2 are extra args that fortran puts in */
#else
int getarg(char *name,char *type,int *val);
#endif


int ga_compute_hash(register char *s);

int ga_getvector(char *list,char *type,int *val);


void ga_getarg_err(char *subname,char *mess);


