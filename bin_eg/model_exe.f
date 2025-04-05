      subroutine my_eval(command)
      character(len=300) :: command
      integer status

      write(*,'(A,A)') 'command:', trim(command)
	   status=system(trim(command))	
	   if (status.ne.0) then
		     print *,'Cannot do:'//trim(command)
		     stop
	   end if	
      end subroutine my_eval


      program main


      implicit none

      character(len=300) :: str_sdep,str_k
      character(len=300) :: structfile


      integer i,N_layer,j_src,flag
      PARAMETER (N_layer=7) 
      real*8 z_layer(N_layer+1)
      real*8 h_layer(N_layer)
      real*8 vp(N_layer)
      real*8 vs(N_layer)
      real*8 dens(N_layer)
      real*8 qp(N_layer)
      real*8 qs(N_layer)
      real*8 z_src
      real*8 sdep,ddist
      real*16 pi,startdist
      integer distitr

      CHARACTER(len=32) :: arg
      character(len=300) :: model_name

      integer NN
      real*8 alpha
      real*8 vred
      real*8 DT



      call get_command_argument(1, arg)
      read(arg,*) model_name

      call get_command_argument(2, arg)
      read(arg,*) NN

      call get_command_argument(3, arg)
      read(arg,*) DT


      call get_command_argument(4, arg)
      read(arg,*) sdep

      call get_command_argument(5, arg)
      read(arg,*) startdist


      call get_command_argument(6, arg)
      read(arg,*) distitr


      call get_command_argument(7, arg)
      read(arg,*) ddist

      
      pi=4.0d0*atan(1.0d0)

!      startdist=125.0d0
!      distitr=16
!      ddist=5


      alpha=6.0d0
      vred=60.0d0



!      z_layer= (/0.0,1.0,3.0,4.0,5.0,17.0,25.0,85.0/)
!      vp=(/3.2, 4.50,4.8,5.51,6.21,6.89,7.83/) 
!      vs=(/1.5,2.4,2.78,3.18,3.40,3.98,4.52/) 
!      dens=(/2.28,2.28,2.58, 2.58, 2.68, 3.0,3.26/) 
!      qp=(/600,600,600,600,600,600,600/) 
!      qs=(/300,300,300,300,300,300,300/) 


      z_layer= (/0.0,  2.0 ,  7.0,  15.0,  70.0, 92.0, 115.0,6115.0 /) 
      vp=(/4.0,6.0,6.4,7.5, 7.52, 7.6,9.3/) 
      vs=(/2.28,3.42,3.64,4.20, 4.21,4.25,5.31/) 
      dens=(/2.3,2.4, 2.67, 3.1,  3.2, 3.2,3.7/) 
      qp=(/250,250,250,250,250,250,250/)
      qs=(/125,125,125,125,125,125,125/)




      write(*,*) z_layer
  


      write(str_sdep,'(f20.10)') sdep
      write(*,'(A,A)') 'str_sdep: ', trim(str_sdep)
      write(str_k,'(I0)') 1


      write(structfile,'(A)') trim(model_name)

      write(*,'(A)') trim(structfile)

      open(1,file=trim(structfile),status='unknown')



      write(1,'(A)') '.F.'
      write(1,'(11I5)') 0,64
      write(1,'(A)') 'GREEN.'//trim(str_k)

      do i=1,N_layer      
      h_layer(i)=z_layer(i+1)-z_layer(i)
      write(*,*) h_layer(i)
      end do

      z_src=sdep
      j_src=N_layer+1;


!ENCUENTRA LA CAPA DONDE SE ENCUENTRA LA FUENTE
      do i=1,N_layer

      if(z_src .ge. z_layer(i).and. z_src .lt. z_layer(i+1)) then
         j_src=i
      write(*,*) "i z_src , z_layer(i), z_layer(i+1)"
      write(*,*) i,z_src , z_layer(i), z_layer(i+1)

      endif

      end do

      if(j_src .le. N_layer) then

!ENCUENTRA SI LA FUENTE ESTA EN UNA INTERFAZ

      flag=0
      do i=1,N_layer+1
      if (z_src .eq. z_layer(i) ) then
      flag=1
      end if     
      end do

!EN CASO NECESARIO PONE UNA CAPA MAS
      if (flag .eq. 0) then
      write(1,'(2F10.4,3I5,F10.4,I5,I5)')  
     +alpha,z_src,1,NN/2,NN,DT,N_layer+1,1
      write(1,'(11I5)') 1,1,1,1,1,1,1,1,1,1,0

      write(1,'(4E11.4,2F10.2)')(h_layer(I),vp(I),vs(I),
     +dens(I),qp(I),qs(I),I=1,j_src-1)

      write(1,'(4E11.4,2F10.2)')z_src-z_layer(j_src),vp(j_src),
     +vs(j_src),dens(j_src),qp(j_src),qs(j_src)

      write(1,'(4E11.4,2F10.2)')z_layer(j_src+1)-z_src,vp(j_src),
     +vs(j_src),dens(j_src),qp(j_src),qs(j_src)

      if (j_src+1 .le. N_layer) then
      write(1,'(4E11.4,2F10.2)')(h_layer(I),vp(I),vs(I),
     +dens(I),qp(I),qs(I),I=j_src+1,N_layer)
      end if
      

      write(1,'(I5)') j_src+1


!EN CASO DE QUE LA FUENTE SE ENCUENTRE EN UNA INTERFAZ NO AGREGA NADA
      else

      write(1,'(2F10.4,3I5,F10.4,I5,I5)')  
     +alpha,z_src,1,NN/2,NN,DT,N_layer,1
      write(1,'(11I5)') 1,1,1,1,1,1,1,1,1,1,0

      write(1,'(4E11.4,2F10.2)')(h_layer(I),vp(I),vs(I),dens(I),
     +qp(I),qs(I),I=1,N_layer)

      write(1,'(I5)') j_src


      end if

!EN CASO DE QUE LA FUENTE SE SALGA DEL RANGO DE PROFUNDIDADES SE PARA LA EJECUCION

      else

		     print *,'The depth source exceeds the maximum layer depth:
     +add one more layer'
		     stop


      end if

      write(1,'(2E15.7,I9)') 0.4000000E+03,1.500000E+00,0 
      write(1,'(I5,4F10.2)') distitr,10000.0,30.0,1.2,1.0

      do  i=0,distitr-1
      write(1,'(3F10.4)') 1.0d0*(startdist+ddist*i), 0.0, vred

      end do


      close(1)


      end program main
