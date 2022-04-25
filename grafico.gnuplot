set datafile separator ','
set terminal dumb feed 180 30 enhanced
set autoscale
set logscale y 2
plot "CLOCK-newton-inexato-total.csv" using 1:2 with lines
