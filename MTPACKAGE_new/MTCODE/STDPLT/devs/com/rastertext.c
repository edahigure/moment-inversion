#include "vfont.h"
#include <stdio.h>
#define MAC '#'
#define NFONTMAX 10

extern int textcenter;
extern float textsize, textangle;
extern float textcosang, textsinang;
extern int textfont;
extern float pixpoint, pixinch;

struct fontinfo
   {
	int fontdes;
	int fontuse;
	struct dispatch *fontdisp;
	char *fontbits;
   }  fonts[NFONTMAX];

int nfont;

rastext(xp,yp,linebuf)
int xp,yp;
char linebuf[];
   {
	int tempsize,  temptype;
	int tempdir;
	int xpt, ypt, arg;
	float wid,hgt;
	int vshift, hshift, txorig, tyorig, lastx, lasty;
	int tempcolor, n, psize; /*psize is height of char in pixels*/
	struct fontinfo *pfont, *loadfont();
	struct dispatch *disp;
	char *cptr, *charbits, c;
	float PI = 3.141592;

	xpt= xp;
	ypt= yp;
	txorig = xp;
	tyorig = yp;
	lastx = xp;
	lasty = xp;
	tempsize=  getsize(textsize);  /* tempsize is font size */
	psize = tempsize  / pixpoint;  /* size of char in pixels */

	temptype=  textfont;



	pfont= loadfont(tempsize,temptype);


	if (textcenter)
	   {
		tempdir = 0;
		wid = 0;
		cptr= linebuf;

		while( (c= *cptr++) != '\0' )
		   {
			disp= & pfont->fontdisp[(int)(c)&0x7f];
			wid += disp->width;
		   }
		hgt = psize;
		hshift = textcenter & 0x03;
		xpt -=  0.5*hshift*wid*textcosang;
		ypt -=  0.5*hshift*wid*textsinang;
		vshift= (textcenter>>2) % 3;
		xpt += 0.5*vshift*hgt*textsinang;
		ypt -= 0.5*vshift*hgt*textcosang;
	   }

	tempdir=4;
	if (textangle==0.0) tempdir=0;
	else if (textangle < (PI/2.0)+.01  && textangle > (PI/2.0) -.01)
			tempdir=1;
	else if (textangle < PI+.01  && textangle > PI -.01)
			tempdir=2;
	else if (textangle < (PI/-2.0)+.01 && textangle > (PI/-2.0)-.01)
			tempdir=3;
	cptr= linebuf;

	while( (c= *cptr++) != '\0' )
	   {
		if(c == MAC)
		   {
			if( (c= *cptr++) == '\0' ) break;
			if(c == MAC) goto bypass;
			if(cptr[0] != '(')
			   {
				fprintf(stderr,"rastext: '('-error mac=%c\n",c);
				break;
			   }
			arg= atol(&cptr[1]);
			while(*cptr && *cptr != ')') cptr++;
			if( *cptr != ')')
			   {
				fprintf(stderr,"rastext: ')'-error mac=%c\n",c);
				break;
			   }
			cptr++;
			switch(c)
			   {
				case 'f': temptype= arg; break;
				case 's': tempsize= arg; break;
				case 'c': tempcolor=arg; break;/* does nothing*/
				default:
					fprintf(stderr,"bad macro mac=%c\n",c);
					goto bypass;
					break;
			   }
			if(cptr[1] == MAC) continue;
			pfont= loadfont(tempsize,temptype);
		   }
		 else
		   {
		
		bypass:
			switch(c)
			   {
				case '\n': 
					   txorig += 1.2*psize*textsinang;
					   tyorig -= 1.2*psize*textcosang;
					   xpt = txorig;
					   ypt = tyorig;
					   break;
				case '\b': xpt = lastx;
					   ypt = lasty;
					   break;
				default:
					lastx = xpt;
					lasty = ypt;
					disp= & pfont->fontdisp[(int)(c)&0x7f];
					charbits= pfont->fontbits + disp->addr;
					bitset(&xpt,&ypt,tempdir,charbits,disp);
					pfont->fontuse++;
					break;
			   }
		   } /*else*/
	   }
   }


bitset(xp,yp,dir,charbits,disp)
int *xp, *yp, dir;
char *charbits;
struct dispatch *disp;
   {
	int nrow, ncol, rowwid, xbase, ybase, icol, irow, bit, xpt, ypt;
	float fxbase, fybase, fxpt, fypt, S, C;
	char *ptr, c[8];

	nrow	= disp->up   + disp->down;
	ncol	= disp->left + disp->right;
	rowwid	= (ncol+7)/8;
	if(dir == 4)
	   {
		C= textcosang;
		S= textsinang;
		fxbase= *xp - C*disp->left -S*disp->up;
		fybase= *yp - S*disp->left +C*disp->up;
		*xp += C*disp->width;
		*yp += S*disp->width;
		for(irow=0; irow<nrow; irow++)
		   {
			fxpt= fxbase;
			fypt= fybase;
			ptr= charbits + rowwid*irow;
			bit= 0x80;
			for(icol=0; icol<ncol; icol++)
			   {
				if(bit & *ptr)
				   { 
					do_point((int)(fxpt),(int)(fypt));
				   }
				bit >>= 1;
				if(bit == 0)
				   {
					bit= 0x80;
					ptr++;
				   }
				fxpt += C;
				fypt += S;
			   }
			fybase -= C;
			fxbase += S;
		   }
		return 1;
	   }

	switch(dir&03)
	   {
		case 0:
			xbase= *xp - disp->left;
			ybase= *yp + disp->up;
			*xp += disp->width;
			for(irow=0; irow<nrow; irow++)
			   {
				xpt= xbase;
				ptr= charbits + rowwid*irow;
				bit= 0x80;
				for(icol=0; icol<ncol; icol++)
				   {
					if(bit & *ptr)
					   { 
						do_point(xpt,ybase);
					   }
					bit >>= 1;
					if(bit == 0)
					   {
						bit= 0x80;
						ptr++;
					   }
					xpt++;
				   }
				ybase--;
			   }
			break;
		case 1:
			xbase= *xp - disp->up;
			ybase= *yp - disp->left;
			*yp += disp->width;
			for(irow=0; irow<nrow; irow++)
			   {
				ypt= ybase;
				ptr= charbits + rowwid*irow;
				bit= 0x80;
				for(icol=0; icol<ncol; icol++)
				   {
					if(bit & *ptr)
					   { 
						do_point(xbase,ypt);
					   }
					bit >>= 1;
					if(bit == 0)
					   {
						bit= 0x80;
						ptr++;
					   }
					ypt++;
				   }
				xbase++;
			   }
			break;
		case 2:
			xbase= *xp + disp->left;
			ybase= *yp - disp->up;
			*xp -= disp->width;
			for(irow=0; irow<nrow; irow++)
			   {
				xpt= xbase;
				ptr= charbits + rowwid*irow;
				bit= 0x80;
				for(icol=0; icol<ncol; icol++)
				   {
					if(bit & *ptr)
					   { 
						do_point(xpt,ybase);
					   }
					bit >>= 1;
					if(bit == 0)
					   {
						bit= 0x80;
						ptr++;
					   }
					xpt--;
				   }
				ybase++;
			   }
			break;
		case 3:
			xbase= *xp + disp->up;
			ybase= *yp + disp->left;
			*yp -= disp->width;
			for(irow=0; irow<nrow; irow++)
			   {
				ypt= ybase;
				ptr= charbits + rowwid*irow;
				bit= 0x80;
				for(icol=0; icol<ncol; icol++)
				   {
					if(bit & *ptr)
					   { 
						do_point(xbase,ypt);
					   }
					bit >>= 1;
					if(bit == 0)
					   {
						bit= 0x80;
						ptr++;
					   }
					ypt--;
				   }
				xbase--;
			   }
			break;
	   }
   }
char fdir[]	="/usr/lib/vfont";

struct fontinfo *loadfont(size,type)
int size, type;
   {
	int fontdes, i, imin, minuse, iuse;
	int fd;
	char fname[40];
	struct fontinfo *ptr;
	static oldfontdes = -1;
	static struct fontinfo *oldfontptr=0;
	struct header header;

	fontdes= size | (type <<8);
	if(fontdes == oldfontdes) return(oldfontptr);
	for(i= 0; i<nfont; i++)
	   {
		ptr= fonts +i;
		if(ptr->fontdes == fontdes)
		   {
			oldfontptr= ptr;
			oldfontdes= fontdes;
			return(ptr);
		   }
	   }
	if(nfont >= NFONTMAX)
	   {
		minuse= fonts[0].fontuse;
		imin= 0;
		for(i=1; i<nfont; i++)
		   {
			ptr= fonts +i;
			if(ptr->fontuse < minuse)
			   {
				minuse= ptr->fontuse;
				imin= i;
			   }
		   }
		/*fprintf(stderr,"font overload fontuse=%d\n",minuse);*/
		iuse= imin;
		free(fonts[iuse].fontdisp);
	   }
	 else
	   {
		iuse= nfont;
		nfont++;
	   }
	ptr= &fonts[iuse];
	ptr->fontdes= fontdes;
	ptr->fontuse= 0;
	switch(type)
	   {
		case 1:
			sprintf(fname,"%s/R.%d",fdir,size);
			break;
		case 2:
			sprintf(fname,"%s/I.%d",fdir,size);
			break;
		case 3:
			sprintf(fname,"%s/B.%d",fdir,size);
			break;
		case 4:
			sprintf(fname,"%s/S.%d",fdir,size);
			break;
	   }
	if( (fd= open(fname,0)) < 0)
	   {
		fprintf(stderr,"cannot open %s\n",fname);
		exit(1);
	   }
	read(fd,&header,10);
	if( header.magic != 0436)
	   {
		fprintf(stderr,"bad header in %s\n",fname);
		exit(1);
	   }
	ptr->fontdisp= (struct dispatch *)malloc(128*10 + header.size);
	if(ptr->fontdisp == NULL)
	   {
		fprintf(stderr,"cannot allocate memory in load\n");
		exit(1);
	   }
	read(fd,ptr->fontdisp,128*10);
	ptr->fontbits= (char *)(ptr->fontdisp + 128);
	lseek(fd,10 + 256*10,0);
	read(fd,ptr->fontbits,header.size);
	ptr->fontdisp[(int)(' ')].width= ptr->fontdisp[(int)('i')].width;
	close(fd);
	return(ptr);
   }


/* convert to a vfont size */
getsize(size)
float size;
{
	int isize;
	float fsize;

	fsize = size;

	if (size < 0.0 || size > 28.0)   /*kludge because can't pass a float*/
		return(36); 	/*with value of 36.0 - it gets messed up*/

	if (size < 4.0) 	/* assume size is in inches and convert
				   to a vfont size */
		fsize = pixpoint * size * pixinch;

	isize = (int) fsize;

	if (isize <= 8) return(8);
	if (isize <=10) return(10);
	if (isize <=12) return(12);
	if (isize <=14) return(14);
	if (isize <=16) return(16);
	if (isize <=18) return(18);
	if (isize <=20) return(20);
	if (isize <=22) return(22);
	if (isize <=24) return(24);
	if (isize <=28) return(28);
	return(36);

} /*getsize*/
						



