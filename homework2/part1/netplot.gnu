set term png
set output 'netplot.png'
set title "Network Ping-Pong Latency"
set log xy
set xlabel "Message Size (Bytes)"
set ylabel "Time (s)"
set grid
plot "results.dat" notitle with linespoints
