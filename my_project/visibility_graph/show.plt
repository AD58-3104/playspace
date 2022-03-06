# clear
set term wxt size 1280,960
set xlabel "x axis"
set ylabel "y axis"
plot "data.dat" w lp
# set xrange[0:5]
# set yrange[-0.5:1.1]
# plot "velo.dat" using 1:2 title "CoM x velocity " with linespoints ps 1 pt 7, "velo.dat" using 1:3 title "CoM y velocity " with linespoints ps 1 pt 7
unset multiplot