#!/usr/bin/env python3

import os
import re
import csv

ROSENBROCK_GENERATOR = "gera_rosenbrock_mod.sh"
CORE = "3"
PROG_NORMAL = "newtonPC"
PROG_OTIMIZADO = "newtonPC"

trechos = [ 'newton-padrao-total', 'newton-padrao-gradiente', 'newton-padrao-hessiana', 'newton-padrao-sl',
            'newton-inexato-total', 'newton-inexato-gradiente', 'newton-inexato-hessiana', 'newton-inexato-sl']
grupos = [ "L3", "L2CACHE", "FLOPS_DP" ]



def roda_e_gera_csv_do_likwid(n, prog, nome_arq, grupo_metrica):
    os.system("echo 'performance' > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor")
    os.system("./" + ROSENBROCK_GENERATOR + " " + str(n) + " | likwid-perfctr -C " + CORE + " -g " + grupo_metrica + " -m -o " + nome_arq + " ./" + prog + " > /dev/null 2>&1")
    os.system("echo 'powersave' > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor")

def gera_dados_do_csv_do_likwid(reader, metrica, trecho):
    print("gerando metrica", metrica, "trecho", trecho)
    trecho_atual = ''
    for row in reader:

        # mudar o trecho atual, caso tenha uma mudanca de trechos
        if row[0] == 'TABLE':
            trecho_atual = row[1]
        
        else: # nao mudou o trecho
            if trecho_atual == ('Region ' + trecho):
                if metrica == row[0].strip():
                    return row[1]
                    # row[1] == valor da metrica no trecho

    print("ERRO: metrica", metrica, "nao encontrada para grafico", trecho)
    exit(-123)



def update_graficos(n, grupo, metrica):
    roda_e_gera_csv_do_likwid(n, PROG_NORMAL, "tmpnormal.csv", grupo)
    roda_e_gera_csv_do_likwid(n, PROG_OTIMIZADO, "tmpotimizado.csv", grupo)

    with open("tmpnormal.csv", "r") as nf, open("tmpotimizado.csv") as of:
        normal_reader = list(csv.reader(nf, delimiter=','))
        otimizado_reader = list(csv.reader(of, delimiter=','))

        for trecho in trechos:
            with open(grupo + '-' + trecho + '.csv', "a") as f:
                dado_normal = gera_dados_do_csv_do_likwid(normal_reader, metrica, trecho)
                dado_otimizado = gera_dados_do_csv_do_likwid(otimizado_reader, metrica, trecho)
                linha = str(n) + ',' + dado_normal + ',' + dado_otimizado
                if grupo == "FLOPS_DP" and metrica == "FLOPS_DP":
                    linha += ',' + gera_dados_do_csv_do_likwid(normal_reader, "AVX DP MFLOP/s", trecho)
                    linha += ',' + gera_dados_do_csv_do_likwid(otimizado_reader, "AVX DP MFLOP/s", trecho)
                linha += '\n'
                f.write(linha)

def reset_graficos():
    for grupo in grupos:
        for trecho in trechos:
            nome = grupo + '-' + trecho + '.csv'
            if os.path.exists(nome):
                os.remove(nome)

def main():
    reset_graficos()
    #ns = [10, 32, 50, 64, 100, 128, 200, 250, 256, 300, 400, 512, 600, 1000, 1024, 2000, 2048, 3000, 4096]
    ns = [10, 32, 50, 64, 100, 128, 200, 250, 256]

    for n in ns:
        print("n =", n)
        update_graficos(n, "CLOCK",  "Runtime unhalted [s]")
        update_graficos(n, "L3",  "L3 load bandwidth [MBytes/s]")
        update_graficos(n, "L2CACHE",  "L2 miss ratio")
        update_graficos(n, "FLOPS_DP",  "DP MFLOP/s")

    exit(0)



main()
