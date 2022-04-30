set datafile separator ','
set terminal dumb feed 190 30 enhanced
set xlabel "N"
set autoscale


# Tempo

set title "Tempo Método Inexato Total"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-inexato-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-inexato-total.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Inexato Gradiente"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-inexato-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-inexato-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Inexato Hessianas"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-inexato-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-inexato-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Inexato Sistema Linear"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-inexato-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-inexato-sl.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Padrao Total"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-padrao-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-padrao-total.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Padrao Gradiente"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-padrao-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-padrao-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Padrao Hessianas"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-padrao-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-padrao-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Tempo Método Padrao Sistema Linear"
set ylabel "Tempo [s]"
set logscale y 10
plot "./graficos/CLOCK-newton-padrao-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/CLOCK-newton-padrao-sl.csv" using 0:2 title 'otimizado' with lines




# Banda de memória

set title "Banda de memória Método Inexato Total"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-inexato-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-inexato-total.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Inexato Gradiente"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-inexato-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-inexato-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Inexato Hessianas"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-inexato-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-inexato-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Inexato Sistema Linear"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-inexato-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-inexato-sl.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Padrao Total"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-padrao-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-padrao-total.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Padrao Gradiente"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-padrao-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-padrao-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Padrao Hessianas"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-padrao-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-padrao-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Banda de memória Método Padrao Sistema Linear"
set ylabel "Banda de memória [MBytes/s]"
set logscale y 10
plot "./graficos/L3-newton-padrao-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L3-newton-padrao-sl.csv" using 0:2 title 'otimizado' with lines



# Cache Miss L2

set title "Cache Miss L2 Método Inexato Total"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-inexato-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-inexato-total.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Inexato Gradiente"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-inexato-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-inexato-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Inexato Hessianas"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-inexato-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-inexato-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Inexato Sistema Linear"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-inexato-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-inexato-sl.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Padrao Total"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-padrao-total.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-padrao-total.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Padrao Gradiente"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-padrao-gradiente.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-padrao-gradiente.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Padrao Hessianas"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-padrao-hessiana.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-padrao-hessiana.csv" using 0:2 title 'otimizado' with lines

set title "Cache Miss L2 Método Padrao Sistema Linear"
set ylabel "Cache Miss L2"
set logscale y 10
plot "./graficos/L2CACHE-newton-padrao-sl.csv" using 0:1 title 'normal' with lines, \
     "./graficos/L2CACHE-newton-padrao-sl.csv" using 0:2 title 'otimizado' with lines



# FLOPS

set title "Flops Método Inexato Total"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-inexato-total.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-inexato-total.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Inexato Gradiente"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-inexato-gradiente.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-inexato-gradiente.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-inexato-gradiente.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-inexato-gradiente.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Inexato Hessiana"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-inexato-hessiana.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-inexato-hessiana.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-inexato-hessiana.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-inexato-hessiana.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Inexato Sistema Linear"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-inexato-sl.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-inexato-sl.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-inexato-sl.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-inexato-sl.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Padrão Total"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-padrao-total.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-padrao-total.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-padrao-total.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-padrao-total.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Padrão Gradiente"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-padrao-gradiente.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-padrao-gradiente.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-padrao-gradiente.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-padrao-gradiente.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Padrão Hessiana"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-padrao-hessiana.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-padrao-hessiana.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-padrao-hessiana.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-padrao-hessiana.csv" using 0:4 title 'otimizado avx' with lines

set title "Flops Método Padrão Sistema Linear"
set ylabel "Flops [MFLOP/s]"
unset logscale
plot "FLOPS_DP-newton-padrao-sl.csv" using 0:1 title 'normal' with lines, \
     "FLOPS_DP-newton-padrao-sl.csv" using 0:2 title 'otimizado' with lines, \
     "FLOPS_DP-newton-padrao-sl.csv" using 0:3 title 'normal avx' with lines, \
     "FLOPS_DP-newton-padrao-sl.csv" using 0:4 title 'otimizado avx' with lines

