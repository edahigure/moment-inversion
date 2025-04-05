CFLAGS	= 
FFLAGS 	= 

#NRC=/data/SOFTWARE_PACKAGES/num_recipes_c_v2/nrcv2.a
#NRC=/usr/contrib/src/num_recipes_c_v2/nrcv2.a
NRC=nrcv2.a
ALL	= libget libstd tdmt_msgs_scan tdmt_redi_sched get_resp sac2helm window tdmt_invc bin2sac sac2bin fromHelm mkHelm putmech putmt  FKRPROG wvint9 psigl
PROGLIST= tdmt_msgs_scan tdmt_redi_sched get_resp sac2helm window tdmt_invc bin2sac sac2bin fromHelm mkHelm putmech putmt  FKRPROG wvint9

all:	$(ALL)

tdmt_msgs_scan: tdmt_msgs_scan.c getenv_utils.c
	gcc -o tdmt_msgs_scan tdmt_msgs_scan.c getenv_utils.c
	\mv tdmt_msgs_scan ./BIN

tdmt_redi_sched: tdmt_redi_sched.c distaz.c getenv_utils.c
	gcc -o tdmt_redi_sched tdmt_redi_sched.c distaz.c getenv_utils.c -lm
	\mv tdmt_redi_sched ./BIN

get_resp: get_resp.c getenv_utils.c getpar.c 
	gcc -o get_resp get_resp.c getenv_utils.c getpar.c 
	\mv get_resp ./BIN

sac2helm: sac2helm.c getpar.c 
	gcc -o sac2helm sac2helm.c getpar.c 
	\mv sac2helm ./BIN

sac2bin: sac2bin.c getpar.c 
	gcc -o sac2bin sac2bin.c getpar.c 
	\mv sac2bin ./BIN

putmech:  putmech.c readhelm.c getpar.c 
	gcc -g -o putmech putmech.c readhelm.c getpar.c  -lm
	\mv putmech ./BIN

putmt:  putmt.c readhelm.c getpar.c 
	gcc -g -o putmt putmt.c readhelm.c getpar.c  -lm
	\mv putmt ./BIN

window: window.c getpar.c 
	gcc -o window window.c getpar.c 
	\mv window ./BIN


tdmt_invc: libstd  tdmt_invc.c 
	gcc -c -O tdmt_invc_linux.c minvdbl_linux.c correl2b_linux.c readhelm.c mt_plot6iso2_linux2.c fitcheck2.c  -lm
	gfortran -c -O fmap_subs_linux.f
	gfortran -o tdmt_invc tdmt_invc_linux.o minvdbl_linux.o correl2b_linux.o readhelm.o mt_plot6iso2_linux2.o fitcheck2.o fmap_subs_linux.o $(NRC) libstd.a -lm
	\mv tdmt_invc ./BIN

fromHelm: fromHelm5.c fixbase.c getpar.c
	gcc -o fromHelm fromHelm5.c fixbase.c getpar.c
	\mv fromHelm ./BIN

mkHelm: mkHelm2.c getpar.c
	gcc -o mkHelm mkHelm2.c getpar.c
	\mv mkHelm ./BIN

bin2sac: bin2sac.c getpar.c 
	gcc -o bin2sac bin2sac.c getpar.c 
	\mv bin2sac ./BIN

libget: 
	(cd ./GETPAR; make libget)

libstd: libget.a 
	(cd ./STDPLT/plotsubs; make)

psigl:
	(cd ./STDPLT/devs/psigl; make)

##FK-Integration routines provided by Chandan Saikia
FKRPROG: FKRPROG.f
	gfortran  -O3 FKRPROG.f -o FKRPROG
	mv FKRPROG ./BIN

wvint9: wvint9.f cwrite.o
	cc -c cwrite.c
	gfortran wvint9.f cwrite.o -o wvint9
	mv wvint9 ./BIN

##Cleanup routine

cleanup:
	(\rm *.o libget.a libstd.a)
	(cd BIN;\rm $(PROGLIST))
	(cd GETPAR;make cleanup)
	(cd STDPLT;make cleanup)
