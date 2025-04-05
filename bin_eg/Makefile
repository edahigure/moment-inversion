
ALL	=  small

all:$(ALL)

small: 
	g++  calc.cpp -lm -o calc
	gcc distance_sac.c sacio.a distaz.c -lm -o distance_sac
	gcc dist_azi_sac.c sacio.a distaz.c -lm -o dist_azi_sac
	gcc greater_equal.c -o greater_equal
	gcc julian_day.c -o julian_day
	gcc lesser_than.c -o lesser_than
	gcc plot_helm.c readhelm.c -o plot_helm 
	gcc -o sac2helm sac2helm.c getpar.c 
	gcc strcmp.c -o strcmp
	gfortran model_exe.f -o model_exe
	gcc copy_gf.c -lm -o copy_gf
