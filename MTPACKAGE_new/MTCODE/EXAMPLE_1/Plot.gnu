set terminal wxt persist title "GnuPlot"
unset key
set grid
set pointsize .3
plot  "gf0.xy" , "gf1.xy" , "gf2.xy" using 1:2 with lines linecolor rgb "blue" 