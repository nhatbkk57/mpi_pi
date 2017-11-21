set key left top
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"
set xlabel "Number of Slave"
set ylabel "Time Execution [s]"
set xrange [0:12]
#set logscale x
#set logscale y
set datafile separator ","
plot 'pi_montecarlo_par.csv' using 1:2 with linespoints title "Non-OpenMP", 'pi_montecarlo_par.csv' using 1:3 with linespoints title "OpenMP"
pause -1