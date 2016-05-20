set terminal png
set output "sb.png"
set logscale x 2
set logscale y 10
set format x "%.0f"
set grid xtics ytics mxtics mytics
set xlabel "Stride (Bytes)"
set ylabel "Average time per read (cycles)"
plot "run.txt" index 0 using ($2*4):($3/2.67e-9) with linespoints title "4 KB (n=1024)", \
     "run.txt" index 1 using ($2*4):($3/2.67e-9) with linespoints title "8 KiB", \
     "run.txt" index 2 using ($2*4):($3/2.67e-9) with linespoints title "16 KiB", \
     "run.txt" index 3 using ($2*4):($3/2.67e-9) with linespoints title "32 KiB", \
     "run.txt" index 4 using ($2*4):($3/2.67e-9) with linespoints title "64 KiB", \
     "run.txt" index 5 using ($2*4):($3/2.67e-9) with linespoints title "128 KiB", \
     "run.txt" index 6 using ($2*4):($3/2.67e-9) with linespoints title "256 KiB", \
     "run.txt" index 7 using ($2*4):($3/2.67e-9) with linespoints title "512 KiB", \
     "run.txt" index 8 using ($2*4):($3/2.67e-9) with linespoints title "1 MiB", \
     "run.txt" index 9 using ($2*4):($3/2.67e-9) with linespoints title "2 MiB", \
     "run.txt" index 10 using ($2*4):($3/2.67e-9) with linespoints title "4 MiB", \
     "run.txt" index 11 using ($2*4):($3/2.67e-9) with linespoints title "8 MiB", \
     "run.txt" index 12 using ($2*4):($3/2.67e-9) with linespoints title "16 MiB", \
     "run.txt" index 13 using ($2*4):($3/2.67e-9) with linespoints title "32 MiB", \
     "run.txt" index 14 using ($2*4):($3/2.67e-9) with linespoints title "64 MiB"
