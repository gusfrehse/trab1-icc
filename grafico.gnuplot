set datafile separator ','
#set terminal dumb feed 180 30 enhanced
set autoscale
#set logscale y 2
plot "FLOPS_DP-newton-inexato-total.csv" using 1:2 title 'normal flops' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 1:3 title 'otimizado flops' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 1:4 title 'normal flops avx' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 1:5 title 'otimizado flops avx' with lines, \

