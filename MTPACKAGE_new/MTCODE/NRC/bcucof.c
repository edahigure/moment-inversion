void bcucof(float y[], float y1[], float y2[], float y12[], float d1, float d2,
	float **c)
{
	static int wt[16][16]=
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
		-3,0,0,3,0,0,0,0,-2,0,0,-1,0,0,0,0,
		2,0,0,-2,0,0,0,0,1,0,0,1,0,0,0,0,
		0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
		0,0,0,0,-3,0,0,3,0,0,0,0,-2,0,0,-1,
		0,0,0,0,2,0,0,-2,0,0,0,0,1,0,0,1,
		-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0,
		9,-9,9,-9,6,3,-3,-6,6,-6,-3,3,4,2,1,2,
		-6,6,-6,6,-4,-2,2,4,-3,3,3,-3,-2,-1,-1,-2,
		2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0,
		-6,6,-6,6,-3,-3,3,3,-4,4,2,-2,-2,-2,-1,-1,
		4,-4,4,-4,2,2,-2,-2,2,-2,-2,2,1,1,1,1};
	int l,k,j,i;
	float xx,d1d2,cl[16],x[16];

	d1d2=d1*d2;
	for (i=1;i<=4;i++) {
		x[i-1]=y[i];
		x[i+3]=y1[i]*d1;
		x[i+7]=y2[i]*d2;
		x[i+11]=y12[i]*d1d2;
	}
	for (i=0;i<=15;i++) {
		xx=0.0;
		for (k=0;k<=15;k++) xx += wt[i][k]*x[k];
		cl[i]=xx;
	}
	l=0;
	for (i=1;i<=4;i++)
		for (j=1;j<=4;j++) c[i][j]=cl[l++];
}
/* (C) Copr. 1986-92 Numerical Recipes Software #. */
