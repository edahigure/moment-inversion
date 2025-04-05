#include "getpar.h"
#ifdef FORTRAN
setpar_()
#else
void setpar(ac,av)		/* set up arglist & process INPUT command */
int ac; char **av;
#endif
   {
	register char *pl, *pn, *pv;
	char  t, name[MAXNAME], value[MAXVALUE];
	FILE *file, *gp_create_dump();
	int i, addflags, nevlist, testav, testae;
	struct arglist *alptr;
#ifdef FORTRAN
	int ac; char **av;
	extern int xargc; extern char **xargv;
	ac= xargc; av= xargv;
#endif

	PROGNAME= *av;
	FLAGS= INIT;
	LISTFILE= stderr;

	ARGLIST= NULL;
	ARGBUF = NULL;
	NLIST= NBUF= LISTMAX= BUFMAX= 0;
#ifdef ENVIRONMENT
	gp_do_environment(ac,av);
#endif
	nevlist= NLIST;
	while(--ac>0)
	   {
		av++;
		pl= *av;
		while(*pl == ' ' || *pl == '\t') pl++;
		/* get name */
		pn= name;
		while(*pl != '=' && *pl != '\0') *pn++ = *pl++;
		*pn++ = '\0';
		/* get value */
		if(*pl == '=') pl++;
		pv= value;
		if(*pl == '"' || *pl == '\'')
		   {
			t= *pl++;
			while(*pl != '\0')
			   {
				if(*pl == t)
				   {
					if(pl[-1] != '\\') break;
					pv[-1]= t;
					pl++;
				   }
				 else	*pv++ = *pl++;
			   }
		   }
		 else	while(*pl) *pv++ = *pl++;
		*pv= '\0';
		if(name[0] == '-') gp_add_entry("SWITCH",&name[1]);
		 else		gp_add_entry(name,value);
		if(strcmp("par",name)==0) /* par file */
			gp_do_par_file(value,1);
	   }
	/* do not internally call getpar before this point because
	   ARGHEAD is not set. The search will have no stopping point */
	ARGHEAD= ARGLIST;
#ifdef ENVIRONMENT
	*value= '\0';
	if(GETPAR("NOENV","s",(int*)value)) ARGHEAD= ARGLIST+ nevlist;
#endif
	addflags= 0;
	*value= '\0';
	if(GETPAR("STOP","s",(int*)value)) addflags |= STOP;
	*value= '\0';
	if(GETPAR("VERBOSE","s",(int*)value)) addflags |= VERBOSE;
	*value= '\0';
	if(GETPAR("LIST","s",(int*)value))
	   {
		addflags |= LIST;
		LISTFILE =gp_create_dump(value,"list");
	   }
	*value= '\0';
	if(GETPAR("INPUT","s",(int*)value))
	   {
		file =gp_create_dump(value,"list input");
		fprintf(file,"%s: getpar input listing\n",PROGNAME);
		for(i=0, alptr=ARGLIST; i<NLIST; i++, alptr++)
		   {
			fprintf(file,"%3d: %16s = %s\n",
				i,alptr->argname,alptr->argval);
		   }
		gp_close_dump(file);
	   }
	FLAGS |= addflags;
   }

void gp_add_entry(name,value)	/* add an entry to arglist, expanding memory */
register char *name, *value;	/* if necessary */
   {
	struct arglist *alptr;
	int len;
	register char *ptr;

	/* check arglist memory */
	if(NLIST >= LISTMAX)
	   {
		LISTMAX += LISTINC;
		if(ARGLIST == NULL)
			ARGLIST= (AL *)malloc(LISTMAX * sizeof(AL));
		 else	ARGLIST= (AL *)realloc(ARGLIST,LISTMAX * sizeof(AL));
	   }
	/* check argbuf memory */
	len= strlen(name) + strlen(value) + 2; /* +2 for terminating nulls */
	if(NBUF+len >= BUFMAX)
	   {
		BUFMAX += BUFINC;
		if(ARGBUF == NULL)
			ARGBUF= (char *)malloc(BUFMAX);
		 else	ARGBUF= (char *)realloc(ARGBUF,BUFMAX);
	   }
	if(ARGBUF == NULL || ARGLIST == NULL)
		gp_getpar_err("setpar","cannot allocate memory");

	/* add name */
	alptr= ARGLIST + NLIST;
	alptr->hash= gp_compute_hash(name);
	ptr= alptr->argname= ARGBUF + NBUF;
	do *ptr++ = *name; while(*name++);

	/* add value */
	NBUF += len;
	alptr->argval= ptr;
	do *ptr++ = *value; while(*value++);
	NLIST++;
   }
#ifdef ENVIRONMENT
void gp_do_environment(ac,av)
int ac; char **av;
   {
	char **ae;
	register char *pl, *pn, *pv;
	char name[MAXNAME], value[MAXVALUE], t;

	/* The environ pointer ae, is assumed to have a specific relation
	   to the arg pointer av. This may not be portable. */
	ae= av +(ac+1);
	if(ae == NULL) return;

	while(*ae != NULL)
	   {
		pl= *ae++;
		while(*pl == ' ' || *pl == '\t') pl++;
		/* get name */
		pn= name;
		while(*pl != '=' && *pl != '\0') *pn++ = *pl++;
		*pn = '\0';
		if(strcmp("NOENV",pn) == 0) return;

		/* get value */
		if(*pl == '=') pl++;
		pv= value;
		if(*pl == '"' || *pl == '\'')
		   {
			t= *pl++;
			while(*pl != '\0')
			   {
				if(*pl == t)
				   {
					if(pl[-1] != '\\') break;
					pv[-1]= t;
					pl++;
				   }
				 else	*pv++ = *pl++;
			   }
		   }
		 else	while(*pl) *pv++ = *pl++;
		*pv= '\0';
		gp_add_entry(name,value);
	   }
   }
#endif

void ENDPAR(void)	/* free arglist & argbuf memory, & process STOP command */
   {
	if(ARGLIST != NULL) free(ARGLIST);
	if(ARGBUF  != NULL) free(ARGBUF);
	ARGBUF=  NULL;
	ARGLIST= NULL;
	if(FLAGS & STOP)
	   {
		fprintf(stderr,"%s[endpar]: stop due to STOP in input\n",
			PROGNAME);
		exit(GETPAR_STOP);
	   }
	FLAGS= END_PAR;	/* this stops further getpar calls */
   }

#ifdef FORTRAN
int mstpar_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int mstpar(name,type,val)
#endif
char *name, *type;
int *val;
   {
	int cnt;
	char *typemess;

	if( (cnt= GETPAR(name,type,val)) > 0) return(cnt);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }

	switch(*type)
	   {
		case 'd': typemess= "an integer";	break;
		case 'f': typemess= "a float";		break;
		case 'F': typemess= "a double";		break;
		case 's': typemess= "a string";		break;
		case 'b': typemess= "a boolean";	break;
		case 'v': switch(type[1])
			   {
				case 'd': typemess= "an integer vector"; break;
				case 'f': typemess= "a float vector"; 	 break;
				case 'F': typemess= "a double vector";	 break;
				default : typemess= "unknow vectorn (error)";
					break;
			   }
			  break;
		default : typemess= "unknown (error)";	break;
	   }


   char* msg;
   sprintf(msg,"must specify value for '%s', expecting %s",name,typemess);
	gp_getpar_err("mstpar",msg);
   }




#ifdef FORTRAN
int mstpar_s_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int mstpar_s(name,type,val)
#endif
char *name, *type;
char *val;
   {
	int cnt;
	char *typemess;

	if( (cnt= GETPAR_S(name,type,val)) > 0) return(cnt);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }

	switch(*type)
	   {
		case 'd': typemess= "an integer";	break;
		case 'f': typemess= "a float";		break;
		case 'F': typemess= "a double";		break;
		case 's': typemess= "a string";		break;
		case 'b': typemess= "a boolean";	break;
		case 'v': switch(type[1])
			   {
				case 'd': typemess= "an integer vector"; break;
				case 'f': typemess= "a float vector"; 	 break;
				case 'F': typemess= "a double vector";	 break;
				default : typemess= "unknow vectorn (error)";
					break;
			   }
			  break;
		default : typemess= "unknown (error)";	break;
	   }


   char* msg;
   sprintf(msg,"must specify value for '%s', expecting %s",name,typemess);
	gp_getpar_err("mstpar",msg);
   }



#ifdef FORTRAN
int mstpar_d_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int mstpar_d(name,type,val)
#endif
char *name, *type;
double *val;
   {
	int cnt;
	char *typemess;

	if( (cnt= GETPAR_D(name,type,val)) > 0) return(cnt);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }

	switch(*type)
	   {
		case 'd': typemess= "an integer";	break;
		case 'f': typemess= "a float";		break;
		case 'F': typemess= "a double";		break;
		case 's': typemess= "a string";		break;
		case 'b': typemess= "a boolean";	break;
		case 'v': switch(type[1])
			   {
				case 'd': typemess= "an integer vector"; break;
				case 'f': typemess= "a float vector"; 	 break;
				case 'F': typemess= "a double vector";	 break;
				default : typemess= "unknow vectorn (error)";
					break;
			   }
			  break;
		default : typemess= "unknown (error)";	break;
	   }


   char* msg;
   sprintf(msg,"must specify value for '%s', expecting %s",name,typemess);
	gp_getpar_err("mstpar",msg);
   }




#ifdef FORTRAN
int mstpar_f_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int mstpar_f(name,type,val)
#endif
char *name, *type;
float *val;
   {
	int cnt;
	char *typemess;

	if( (cnt= GETPAR_F(name,type,val)) > 0) return(cnt);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }

	switch(*type)
	   {
		case 'd': typemess= "an integer";	break;
		case 'f': typemess= "a float";		break;
		case 'F': typemess= "a double";		break;
		case 's': typemess= "a string";		break;
		case 'b': typemess= "a boolean";	break;
		case 'v': switch(type[1])
			   {
				case 'd': typemess= "an integer vector"; break;
				case 'f': typemess= "a float vector"; 	 break;
				case 'F': typemess= "a double vector";	 break;
				default : typemess= "unknow vectorn (error)";
					break;
			   }
			  break;
		default : typemess= "unknown (error)";	break;
	   }


   char* msg;
   sprintf(msg,"must specify value for '%s', expecting %s",name,typemess);
	gp_getpar_err("mstpar",msg);
   }




#ifdef FORTRAN
int getpar_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int getpar(name,type,val)
#endif
char *name, *type;
int *val;
   {
	register char *sptr;
	register struct arglist *alptr;
	register int i;
	double atof(), *dbl;
	float *flt;
	int h, hno, hyes, found;
	char line[MAXLINE], *str, *noname;

   char *mess;

	if(FLAGS & END_PAR)
		gp_getpar_err("getpar","called after endpar");
	if( (FLAGS & INIT) == 0)
		gp_getpar_err("getpar","not initialized with setpar");
	if(FLAGS & VERBOSE)
		fprintf(stderr,"getpar: looking for %s\n",name);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }


	if(*type == 'b') goto boolean;

	h= gp_compute_hash(name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != h) continue;
		if(strcmp(alptr->argname,name) != 0) continue;
		str= alptr->argval;
		switch(*type)
		   {
			case 'd':
				*val= atoi(str);
				found=1;
				break;
			case 'f':
				flt= (float *) val;
				*flt= atof(str);
				found=1;
				break;
			case 'F':
				dbl= (double *) val;
				*dbl= atof(str);
				found=1;
				break;
			case 's':
				sptr= (char *) val;
				while(*str) *sptr++ = *str++;
				*sptr= '\0';
				found=1;
				break;
			case 'v':
				found= gp_getvector(str,type,val);
				break;
			default:

            sprintf(mess,"unknown conversion type %s",type);
				gp_getpar_err("getpar",mess);
				break;
		   }
		break;
	   }
	goto list;
boolean:
	noname= line;
	sprintf(noname,"no%s",name);
	hno = gp_compute_hash(noname);
	hyes= gp_compute_hash(  name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != hno && alptr->hash != hyes) continue;
		if(strcmp(alptr->argname,  name)== 0)
		   {
			if(alptr->argval[0] == '\0') *val= 1;
			 else *val= atol(alptr->argval);
			found++;
			break;
		   }
		if(strcmp(alptr->argname,noname)== 0)
		   {	*val= 0; found++; break; }
	   }
   list:
	if(FLAGS & LIST)
	   {
		switch(*type)
		   {
			case 'd': sprintf(line,"(int) = %d",*val); break;
			case 'f': flt= (float *)val;
				  sprintf(line,"(flt) = %14.6e",*flt); break;
			case 'F': dbl= (double *)val;
				  sprintf(line,"(dbl) = %14.6e",*dbl); break;
			case 's': sprintf(line,"(str) = %s",(char*)val); break;
			case 'b': sprintf(line,"(boo) = %d",*val); break;
			case 'v': switch(type[1])
				   {
					/* should list these out */
					case 'd': sprintf(line,"(int vec)");
						break;
					case 'f': sprintf(line,"(flt vec)");
						break;
					case 'F': sprintf(line,"(dbl vec)");
						break;
					default : sprintf(line," vec type error");
						break;
				   }
				  break;
			default : sprintf(line," type error"); break;
		   }
		fprintf(LISTFILE,"%16s (%s) %s \n",name,
			(found ? "set":"def"),line);
	   }
	return(found);
   }







#ifdef FORTRAN
int getpar_s_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int getpar_s(name,type,val)
#endif
char *name, *type;
char *val;
   {
	register char *sptr;
	register struct arglist *alptr;
	register int i;
	double atof(), *dbl;
	float *flt;
	int h, hno, hyes, found;
	char line[MAXLINE], *str, *noname;

   char *mess;

	if(FLAGS & END_PAR)
		gp_getpar_err("getpar","called after endpar");
	if( (FLAGS & INIT) == 0)
		gp_getpar_err("getpar","not initialized with setpar");
	if(FLAGS & VERBOSE)
		fprintf(stderr,"getpar: looking for %s\n",name);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }


	if(*type == 'b') goto boolean;

	h= gp_compute_hash(name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != h) continue;
		if(strcmp(alptr->argname,name) != 0) continue;
		str= alptr->argval;
		switch(*type)
		   {
			case 'd':
				*val= atoi(str);
				found=1;
				break;
			case 'f':
				flt= (float *) val;
				*flt= atof(str);
				found=1;
				break;
			case 'F':
				dbl= (double *) val;
				*dbl= atof(str);
				found=1;
				break;
			case 's':
				sptr= (char *) val;
				while(*str) *sptr++ = *str++;
				*sptr= '\0';
				found=1;
				break;
			case 'v':
				found= gp_getvector_s(str,type,val);
				break;
			default:

            sprintf(mess,"unknown conversion type %s",type);
				gp_getpar_err("getpar",mess);
				break;
		   }
		break;
	   }
	goto list;
boolean:
	noname= line;
	sprintf(noname,"no%s",name);
	hno = gp_compute_hash(noname);
	hyes= gp_compute_hash(  name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != hno && alptr->hash != hyes) continue;
		if(strcmp(alptr->argname,  name)== 0)
		   {
			if(alptr->argval[0] == '\0') *val= 1;
			 else *val= atol(alptr->argval);
			found++;
			break;
		   }
		if(strcmp(alptr->argname,noname)== 0)
		   {	*val= 0; found++; break; }
	   }
   list:
	if(FLAGS & LIST)
	   {
		switch(*type)
		   {
			case 'd': sprintf(line,"(int) = %d",*val); break;
			case 'f': flt= (float *)val;
				  sprintf(line,"(flt) = %14.6e",*flt); break;
			case 'F': dbl= (double *)val;
				  sprintf(line,"(dbl) = %14.6e",*dbl); break;
			case 's': sprintf(line,"(str) = %s",(char*)val); break;
			case 'b': sprintf(line,"(boo) = %d",*val); break;
			case 'v': switch(type[1])
				   {
					/* should list these out */
					case 'd': sprintf(line,"(int vec)");
						break;
					case 'f': sprintf(line,"(flt vec)");
						break;
					case 'F': sprintf(line,"(dbl vec)");
						break;
					default : sprintf(line," vec type error");
						break;
				   }
				  break;
			default : sprintf(line," type error"); break;
		   }
		fprintf(LISTFILE,"%16s (%s) %s \n",name,
			(found ? "set":"def"),line);
	   }
	return(found);
   }





#ifdef FORTRAN
int getpar_d_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int getpar_d(name,type,val)
#endif
char *name, *type;
double *val;
   {
	register char *sptr;
	register struct arglist *alptr;
	register int i;
	double atof(), *dbl;
	float *flt;
	int h, hno, hyes, found;
	char line[MAXLINE], *str, *noname;

   char *mess;

	if(FLAGS & END_PAR)
		gp_getpar_err("getpar","called after endpar");
	if( (FLAGS & INIT) == 0)
		gp_getpar_err("getpar","not initialized with setpar");
	if(FLAGS & VERBOSE)
		fprintf(stderr,"getpar: looking for %s\n",name);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }


	if(*type == 'b') goto boolean;

	h= gp_compute_hash(name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != h) continue;
		if(strcmp(alptr->argname,name) != 0) continue;
		str= alptr->argval;
		switch(*type)
		   {
			case 'd':
				*val= atoi(str);
				found=1;
				break;
			case 'f':
				flt= (float *) val;
				*flt= atof(str);
				found=1;
				break;
			case 'F':
				dbl= (double *) val;
				*dbl= atof(str);
				found=1;
				break;
			case 's':
				sptr= (char *) val;
				while(*str) *sptr++ = *str++;
				*sptr= '\0';
				found=1;
				break;
			case 'v':
				found= gp_getvector_d(str,type,val);
				break;
			default:

            sprintf(mess,"unknown conversion type %s",type);
				gp_getpar_err("getpar",mess);
				break;
		   }
		break;
	   }
	goto list;
boolean:
	noname= line;
	sprintf(noname,"no%s",name);
	hno = gp_compute_hash(noname);
	hyes= gp_compute_hash(  name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != hno && alptr->hash != hyes) continue;
		if(strcmp(alptr->argname,  name)== 0)
		   {
			if(alptr->argval[0] == '\0') *val= 1;
			 else *val= atol(alptr->argval);
			found++;
			break;
		   }
		if(strcmp(alptr->argname,noname)== 0)
		   {	*val= 0; found++; break; }
	   }
   list:
	if(FLAGS & LIST)
	   {
		switch(*type)
		   {
			case 'd': sprintf(line,"(int) = %lf",*val); break;
			case 'f': flt= (float *)val;
				  sprintf(line,"(flt) = %14.6e",*flt); break;
			case 'F': dbl= (double *)val;
				  sprintf(line,"(dbl) = %14.6e",*dbl); break;
			case 's': sprintf(line,"(str) = %s",(char*)val); break;
			case 'b': sprintf(line,"(boo) = %lf",*val); break;
			case 'v': switch(type[1])
				   {
					/* should list these out */
					case 'd': sprintf(line,"(int vec)");
						break;
					case 'f': sprintf(line,"(flt vec)");
						break;
					case 'F': sprintf(line,"(dbl vec)");
						break;
					default : sprintf(line," vec type error");
						break;
				   }
				  break;
			default : sprintf(line," type error"); break;
		   }
		fprintf(LISTFILE,"%16s (%s) %s \n",name,
			(found ? "set":"def"),line);
	   }
	return(found);
   }





#ifdef FORTRAN
int getpar_f_(name,type,val,dum1,dum2)
int dum1, dum2;	/* dum1 & dum2 are extra args that fortran puts in */
#else
int getpar_f(name,type,val)
#endif
char *name, *type;
float *val;
   {
	register char *sptr;
	register struct arglist *alptr;
	register int i;
	double atof(), *dbl;
	float *flt;
	int h, hno, hyes, found;
	char line[MAXLINE], *str, *noname;

   char *mess;

	if(FLAGS & END_PAR)
		gp_getpar_err("getpar","called after endpar");
	if( (FLAGS & INIT) == 0)
		gp_getpar_err("getpar","not initialized with setpar");
	if(FLAGS & VERBOSE)
		fprintf(stderr,"getpar: looking for %s\n",name);

	/* The following line corrects a common input error */
	if(type[1]=='v') { type[1]= type[0]; type[0]='v'; }


	if(*type == 'b') goto boolean;

	h= gp_compute_hash(name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != h) continue;
		if(strcmp(alptr->argname,name) != 0) continue;
		str= alptr->argval;
		switch(*type)
		   {
			case 'd':
				*val= atoi(str);
				found=1;
				break;
			case 'f':
				flt= (float *) val;
				*flt= atof(str);
				found=1;
				break;
			case 'F':
				dbl= (double *) val;
				*dbl= atof(str);
				found=1;
				break;
			case 's':
				sptr= (char *) val;
				while(*str) *sptr++ = *str++;
				*sptr= '\0';
				found=1;
				break;
			case 'v':
				found= gp_getvector_f(str,type,val);
				break;
			default:

            sprintf(mess,"unknown conversion type %s",type);
				gp_getpar_err("getpar",mess);
				break;
		   }
		break;
	   }
	goto list;
boolean:
	noname= line;
	sprintf(noname,"no%s",name);
	hno = gp_compute_hash(noname);
	hyes= gp_compute_hash(  name);
	found=0;
	/* search list backwards, stopping at first find */
	for(alptr= ARGLIST +(NLIST-1); alptr >= ARGHEAD; alptr--)
	   {
		if(alptr->hash != hno && alptr->hash != hyes) continue;
		if(strcmp(alptr->argname,  name)== 0)
		   {
			if(alptr->argval[0] == '\0') *val= 1;
			 else *val= atol(alptr->argval);
			found++;
			break;
		   }
		if(strcmp(alptr->argname,noname)== 0)
		   {	*val= 0; found++; break; }
	   }
   list:
	if(FLAGS & LIST)
	   {
		switch(*type)
		   {
			case 'd': sprintf(line,"(int) = %lf",*val); break;
			case 'f': flt= (float *)val;
				  sprintf(line,"(flt) = %14.6e",*flt); break;
			case 'F': dbl= (double *)val;
				  sprintf(line,"(dbl) = %14.6e",*dbl); break;
			case 's': sprintf(line,"(str) = %s",(char*)val); break;
			case 'b': sprintf(line,"(boo) = %lf",*val); break;
			case 'v': switch(type[1])
				   {
					/* should list these out */
					case 'd': sprintf(line,"(int vec)");
						break;
					case 'f': sprintf(line,"(flt vec)");
						break;
					case 'F': sprintf(line,"(dbl vec)");
						break;
					default : sprintf(line," vec type error");
						break;
				   }
				  break;
			default : sprintf(line," type error"); break;
		   }
		fprintf(LISTFILE,"%16s (%s) %s \n",name,
			(found ? "set":"def"),line);
	   }
	return(found);
   }







FILE *gp_create_dump(fname,filetype)
char *fname;
char *filetype;
   {
	FILE *temp;

	if(*fname == '\0') return(stderr);
	if(strcmp(fname,"stderr") == 0) return(stderr);
	if(strcmp(fname,"stdout") == 0) return(stdout);
	if( (temp= fopen(fname,"w")) != NULL) return(temp);
	fprintf(stderr,"%s[setpar]: cannot create %s file %s\n",
		PROGNAME,filetype,fname);
	return(stderr);
   }

void gp_close_dump(file)
FILE *file;
   {
	if(file == stderr || file == stdout) return;
	fclose(file);
   }

int gp_compute_hash(s)
register char *s;
   {
	register int h;
	h= s[0];
	if(s[1]) h |= (s[1])<<8;	else return(h);
	if(s[2]) h |= (s[2])<<16;	else return(h);
	if(s[3]) h |= (s[3])<<24;
	return(h);
   }

void gp_do_par_file(fname,level)
char *fname;
int level;
   {
	register char *pl, *pn, *pv;
	char t1, t2, line[MAXLINE], name[MAXNAME], value[MAXVALUE];
	FILE *file, *fopen();
   char* mess;

	if(level > MAXPARLEVEL){
      sprintf(mess,"%d (too many) recursive par file",level);
		gp_getpar_err("setpar",mess);
   }
		
	if( (file=fopen(fname,"r"))==NULL){
      sprintf(mess,"cannot open par file %s",fname);
		gp_getpar_err("setpar",mess);
   }

	while( fgets(line,MAXLINE,file) != NULL )
	   {
		pl= line;
		/* loop over entries on each line */
	loop:	while(*pl==' ' || *pl=='\t') pl++;
		if(*pl=='\0'|| *pl=='\n') continue;
		if(*pl=='#') continue; /* comments on rest of line */

		/* get name */
		pn= name;
		while(*pl != '=' && *pl != '\0' && *pl != ' '
			&& *pl != '\t') *pn++ = *pl++;
		*pn = '\0';
		if(*pl == '=') pl++;

		/* get value */
		*value= '\0';
		pv= value;
		if(*pl=='"' || *pl=='\'')	{ t1= t2= *pl++; }
		 else				{ t1= ' '; t2= '\t'; }
		while(*pl!=t1 && *pl!=t2 &&
			*pl!='\0' && *pl!='\n') *pv++= *pl++;
		*pv= '\0';
		if(*pl=='"' || *pl=='\'') pl++;
		gp_add_entry(name,value);
		if(strcmp("par",name) == 0)
			gp_do_par_file(value,level+1);
		goto loop;
	   }
	fclose(file);
   }

void gp_getpar_err(subname,mess)
char *subname, *mess;

   {
	fprintf(stderr,"\n***** ERROR in %s[%s] *****\n\t",
		(PROGNAME == NULL ? "(unknown)" : PROGNAME),subname);
	fprintf(stderr,"%s",mess);
	fprintf(stderr,"\n");
	exit(GETPAR_ERROR);
   }
int gp_getvector(list,type,val)
char *list, *type;
int *val;
   {
	register char *p;
	register int index, cnt;
	char *valptr;
	int limit;
	int ival, *iptr;
	float fval, *fptr;
	double dval, *dptr, atof();
   char* mess;

	limit= MAXVECTOR;
	if(type[2] == '(' || type[2] == '[') limit= atol(&type[3]);
	if(limit <= 0){
      sprintf(mess,"bad limit=%d specified",limit);
		gp_getpar_err("getpar",mess);}
	index= 0;
	p= list;
	while(*p != '\0'  && index < limit)
	   {
		cnt=1;
	 backup: /* return to here if we find a repetition factor */
		while(*p == ' ' || *p == '\t') p++;
		if(*p == '\0') return(index);
		valptr= p;
		while( *p != ',' && *p != '*' && *p != 'x' && *p != 'X' &&
			*p != '\0') p++;
		if(*p == '*' || *p == 'x' || *p == 'X')
		   {
			cnt= atol(valptr);
			if(cnt <= 0){
            sprintf(mess,"bad repetition factor=%d specified",cnt);
				gp_getpar_err("getpar",mess);
         }
			if(index+cnt > limit) cnt= limit - index;
			p++;
			goto backup;
		   }
		switch(type[1])
		   {
			case 'd':
				iptr= (int *) val;
				ival= atol(valptr);
				while(cnt--) iptr[index++] = ival;
				break;
			case 'f':
				fptr= (float *) val;
				fval= atof(valptr);
				while(cnt--) fptr[index++] = fval;
				break;
			case 'F':
				dptr= (double *) val;
				dval= atof(valptr);
				while(cnt--) dptr[index++] = dval;
				break;
			default:
            sprintf(mess,"bad vector type=%c specified",type[1]);
				gp_getpar_err("getpar",mess);
					
				break;
		   }
		if(*p != '\0') p++;
	   }
	return(index);
   }





int gp_getvector_s(list,type,val)
char *list, *type;
char *val;
   {
	register char *p;
	register int index, cnt;
	char *valptr;
	int limit;
	int ival, *iptr;
	float fval, *fptr;
	double dval, *dptr, atof();
   char* mess;

	limit= MAXVECTOR;
	if(type[2] == '(' || type[2] == '[') limit= atol(&type[3]);
	if(limit <= 0){
      sprintf(mess,"bad limit=%d specified",limit);
		gp_getpar_err("getpar",mess);}
	index= 0;
	p= list;
	while(*p != '\0'  && index < limit)
	   {
		cnt=1;
	 backup: /* return to here if we find a repetition factor */
		while(*p == ' ' || *p == '\t') p++;
		if(*p == '\0') return(index);
		valptr= p;
		while( *p != ',' && *p != '*' && *p != 'x' && *p != 'X' &&
			*p != '\0') p++;
		if(*p == '*' || *p == 'x' || *p == 'X')
		   {
			cnt= atol(valptr);
			if(cnt <= 0){
            sprintf(mess,"bad repetition factor=%d specified",cnt);
				gp_getpar_err("getpar",mess);
         }
			if(index+cnt > limit) cnt= limit - index;
			p++;
			goto backup;
		   }
		switch(type[1])
		   {
			case 'd':
				iptr= (int *) val;
				ival= atol(valptr);
				while(cnt--) iptr[index++] = ival;
				break;
			case 'f':
				fptr= (float *) val;
				fval= atof(valptr);
				while(cnt--) fptr[index++] = fval;
				break;
			case 'F':
				dptr= (double *) val;
				dval= atof(valptr);
				while(cnt--) dptr[index++] = dval;
				break;
			default:
            sprintf(mess,"bad vector type=%c specified",type[1]);
				gp_getpar_err("getpar",mess);
					
				break;
		   }
		if(*p != '\0') p++;
	   }
	return(index);
   }







int gp_getvector_d(list,type,val)
char *list, *type;
double *val;
   {
	register char *p;
	register int index, cnt;
	char *valptr;
	int limit;
	int ival, *iptr;
	float fval, *fptr;
	double dval, *dptr, atof();
   char* mess;

	limit= MAXVECTOR;
	if(type[2] == '(' || type[2] == '[') limit= atol(&type[3]);
	if(limit <= 0){
      sprintf(mess,"bad limit=%d specified",limit);
		gp_getpar_err("getpar",mess);}
	index= 0;
	p= list;
	while(*p != '\0'  && index < limit)
	   {
		cnt=1;
	 backup: /* return to here if we find a repetition factor */
		while(*p == ' ' || *p == '\t') p++;
		if(*p == '\0') return(index);
		valptr= p;
		while( *p != ',' && *p != '*' && *p != 'x' && *p != 'X' &&
			*p != '\0') p++;
		if(*p == '*' || *p == 'x' || *p == 'X')
		   {
			cnt= atol(valptr);
			if(cnt <= 0){
            sprintf(mess,"bad repetition factor=%d specified",cnt);
				gp_getpar_err("getpar",mess);
         }
			if(index+cnt > limit) cnt= limit - index;
			p++;
			goto backup;
		   }
		switch(type[1])
		   {
			case 'd':
				iptr= (int *) val;
				ival= atol(valptr);
				while(cnt--) iptr[index++] = ival;
				break;
			case 'f':
				fptr= (float *) val;
				fval= atof(valptr);
				while(cnt--) fptr[index++] = fval;
				break;
			case 'F':
				dptr= (double *) val;
				dval= atof(valptr);
				while(cnt--) dptr[index++] = dval;
				break;
			default:
            sprintf(mess,"bad vector type=%c specified",type[1]);
				gp_getpar_err("getpar",mess);
					
				break;
		   }
		if(*p != '\0') p++;
	   }
	return(index);
   }






int gp_getvector_f(list,type,val)
char *list, *type;
float *val;
   {
	register char *p;
	register int index, cnt;
	char *valptr;
	int limit;
	int ival, *iptr;
	float fval, *fptr;
	double dval, *dptr, atof();
   char* mess;

	limit= MAXVECTOR;
	if(type[2] == '(' || type[2] == '[') limit= atol(&type[3]);
	if(limit <= 0){
      sprintf(mess,"bad limit=%d specified",limit);
		gp_getpar_err("getpar",mess);}
	index= 0;
	p= list;
	while(*p != '\0'  && index < limit)
	   {
		cnt=1;
	 backup: /* return to here if we find a repetition factor */
		while(*p == ' ' || *p == '\t') p++;
		if(*p == '\0') return(index);
		valptr= p;
		while( *p != ',' && *p != '*' && *p != 'x' && *p != 'X' &&
			*p != '\0') p++;
		if(*p == '*' || *p == 'x' || *p == 'X')
		   {
			cnt= atol(valptr);
			if(cnt <= 0){
            sprintf(mess,"bad repetition factor=%d specified",cnt);
				gp_getpar_err("getpar",mess);
         }
			if(index+cnt > limit) cnt= limit - index;
			p++;
			goto backup;
		   }
		switch(type[1])
		   {
			case 'd':
				iptr= (int *) val;
				ival= atol(valptr);
				while(cnt--) iptr[index++] = ival;
				break;
			case 'f':
				fptr= (float *) val;
				fval= atof(valptr);
				while(cnt--) fptr[index++] = fval;
				break;
			case 'F':
				dptr= (double *) val;
				dval= atof(valptr);
				while(cnt--) dptr[index++] = dval;
				break;
			default:
            sprintf(mess,"bad vector type=%c specified",type[1]);
				gp_getpar_err("getpar",mess);
					
				break;
		   }
		if(*p != '\0') p++;
	   }
	return(index);
   }
