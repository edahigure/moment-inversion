#include	<stdio.h>
#define MAX	  100
#define MAXDATA	10000
#define COMMENT	'#'
char chardatafile[64]	= "chardata.h";

struct charglyph
   {
	short	npoints;
	short	hshift;
	short	vshift;
	short	width;
	int	index;
   };
struct charglyph Rcg[96];
struct charglyph Icg[96];
struct charglyph Bcg[96];
struct charglyph Scg[96];
struct charglyph *cg;

short	Rdata[MAXDATA];
short	Idata[MAXDATA];
short	Bdata[MAXDATA];
short	Sdata[MAXDATA];
short	*data;

int	nchar	=0;
int	nRdata	=0;
int	nIdata	=0;
int	nBdata	=0;
int	nSdata	=0;
int	linenum	=0;
char line[256], *args[128];

main(ac,av)
int ac; char **av;
   {
	int i, k, n, nargs, np;
	int x, y, hshift, vshift, width, flag, ichar;
	char type, dochar;
	short *sptr;
	int *ndata;
	FILE *fd, *fopen();

	linenum= 0;
	while( (n=getline(stdin,line)) >= 0)
	   {
		linenum++;
		/*fprintf(stderr,"%3d: %s\n",linenum,line);*/
		if(n == 0) continue;	/* blank line */
		if(line[0] == COMMENT) continue;	/* comment */
		if(line[0] != '+')
			err("expecting a '+' field");
		nargs= setpointers(&line[1],args);
		if(nargs < 4)
			err("expecting 4 arguments in '+' field");
		if(args[0][0] != '"' && args[0][2] != '"')
			err("char incorrectly named");
		/* for(i=0; i<nargs; i++)
			fprintf(stderr,"%2d: %s\n",i,args[i]); */
		dochar= args[0][1];
		ichar= dochar - ' ';
		if(ichar < 0 || ichar > 96)
			err("ichar=%d",ichar);
		type  = args[1][0];
		hshift= atol(args[2]);
		vshift= atol(args[3]);
		width = atol(args[4]);
		switch(type)
		   {
			case 'R': cg= Rcg;
			ndata= &nRdata;
			data= Rdata;
			break;
			case 'I': cg= Icg; ndata= &nIdata; data= Idata; break;
			case 'B': cg= Bcg; ndata= &nBdata; data= Bdata; break;
			case 'S': cg= Scg; ndata= &nSdata; data= Sdata; break;
			default:
				err("Unknown font type=%c\n",type);
		   }
		cg[ichar].hshift= hshift;
		cg[ichar].vshift= vshift;
		cg[ichar].width = width ;
		cg[ichar].index = *ndata ;
		np =0;
		while(1)
		   {
			n= getline(stdin,line);
			linenum++;
			if(n == EOF) break;
			if(n == 0) continue;
			if(line[0] == COMMENT) continue;
			if(line[0] == '+')
			   {
				ungetline(line,n);
				linenum--;
				break;
			   }
			nargs= setpointers(line,args);
			if(nargs == 1 && args[0][0]== '"')
			   {
				i= args[0][1] - ' ';
				if(cg[i].npoints == 0)
					err("symbol >%c< not yet defined",args[0][1]);
				sptr= &data[cg[i].index];
				for(k=0; k< cg[i].npoints; k++)
				   {
					data[*ndata]= sptr[k];
					(*ndata)++;
					np++;
				   }
				continue;
			   }
			if(nargs % 3)
				err("bad number of points");
			for(i=0; i<nargs; i += 3)
			   {
				switch(args[i][0])
				   {
					case 'm': flag=1; break;
					case 'd': flag=0; break;
					default:
						err("incorrect m/d arg= %c",args[i][0]);
				   }
				x= atol(args[i+1]);
				y= atol(args[i+2]);
				data[*ndata]= (y<<8) | x | (flag << 7);
				(*ndata)++;
				np++;
			   }
			if(np == 0)
				err("no points specified");
		   }
		cg[ichar].npoints= np;
		fprintf(stderr,">%c< i=%3d type=%c h=%2d v=%2d wid=%2d np=%2d ndata=%4d\n",
			dochar,ichar,type,hshift,vshift,width,np,*ndata);
		nchar++;
	   }
	fd= stdout;
	fprintf(fd,"/* DO NOT MODIFY THIS FILE */\n");
	fprintf(fd,"/* It is machine generated by 'make_char_structs' */\n");
	fprintf(fd,"/* It will be overwritten when a char is added or modified */\n");
	/* define structure */
	fprintf(fd,"struct charglyph\n   {\n");
	fprintf(fd,"\tshort\tnpoints;\n");
	fprintf(fd,"\tshort\thshift;\n");
	fprintf(fd,"\tshort\tvshift;\n");
	fprintf(fd,"\tshort\twidth;\n");
	fprintf(fd,"\tint\tindex;\n");
	fprintf(fd,"   };\n");

	printstruct(fd,Rcg,Rdata,nRdata,"Rcg","Rdata");
	printstruct(fd,Icg,Idata,nIdata,"Icg","Idata");
	printstruct(fd,Bcg,Bdata,nBdata,"Bcg","Bdata");
	printstruct(fd,Scg,Sdata,nSdata,"Scg","Sdata");
   }
printstruct(fd,cg,data,ndata,gname,dname)
FILE *fd;
struct charglyph *cg;
short *data;
int ndata;
char *gname, *dname;
   {
	int i, nc;
	nc= 0;
	for(i=0; i< 96; i++)
		if(cg[i].npoints) nc++;
	if(nc)
	   {
		fprintf(fd,"struct charglyph %s[] = {\n",gname);
		for(i=0; i< 96; i++)
		   {
			fprintf(fd,"\t%3d, %2d, %2d, %2d, %5d,\n",
				cg[i].npoints,
				cg[i].hshift,
				cg[i].vshift,
				cg[i].width,
				cg[i].index);
		   }
		fprintf(fd,"\t0   };\n");
		fprintf(fd,"short %s[] = {\n\t",dname);
		for(i=0; i<ndata; i++)
		   {
			if(i>0 && (i%10)==0) fprintf(fd,"\n\t");
			fprintf(fd,"%6d,",data[i]);
		   }
		fprintf(fd,"0 };\n");
	   }
	 else
	   {
		fprintf(fd,"struct charglyph **%s = NULL;\n",gname);
		fprintf(fd,"short *%s = NULL;\n",dname);
	   }
   }


int oldcount	=0;
int undo	=0;
getline(fd,line)
FILE *fd;
char *line;
   {
	char c;
	int n;

	if(undo)
	   {
		undo= 0;
		return(oldcount);
	   }
	n=0;
	while( (c=getc(fd)) != EOF)
	   {
		if(c == '\n')
		   {
			*line= '\0';
			return(n);
		   }
		*line++ = c;
		n++;
	   }
	return(EOF);
   }
ungetline(line,n)
char *line;
int n;
   {
	oldcount= n;
	undo= 1;
   }

err(mess,a1,a2,a3,a4,a5)
char *mess;
int a1,a2,a3,a4,a5;
   {
	fprintf(stderr,"error near line %d\n",linenum);
	fprintf(stderr,mess,a1,a2,a3,a4,a5);
	fprintf(stderr,"\n");
	exit(-1);
   }
setpointers(list, ptrs)
char *list, *ptrs[];
   {
	int n;

	n=0;
	while( *list != '\0' )
	   {
		while(*list == ' ' || *list == '\t') list++;
		if(*list == '\0') break;
		ptrs[n]= list;
		n++;
		while(*list != ' ' && *list != '\t' && *list != '\0') list++;
		if(*list == '\0') break;
		*list++ = '\0';
	   }
	return(n);
   }
