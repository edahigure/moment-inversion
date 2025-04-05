#!/bin/bash
ls *.disp> list

for next in `cat list`
do

  IFS='.'  read -ra next_splited <<< $next

echo csh ./run_filter $next ${next_splited[0]} 0.02 0.05

csh ./run_filter $next ${next_splited[0]} 0.02 0.05


done
