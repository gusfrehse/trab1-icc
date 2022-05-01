CC		= gcc
CFLAGS	= -std=gnu17 -march=native -ftree-vectorize -mavx2 -ffast-math
LFLAGS	= -lm

NORMAL_CFLAGS           := $(CFLAGS) -O3

PROFILING_CFLAGS	:= $(CFLAGS) -O0 -pg -fno-inline
PROFILING_LFLAGS	:= $(LFLAGS)

LIKWID_CFLAGS		:= $(CFLAGS) -O3 -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -DLIKWID
LIKWID_LFLAGS		:= $(LFLAGS) -L${LIKWID_LIB} -llikwid

T2_DIR				= t2
T1_DIR				= t1
T1_LINHA_DIR		= t1_linha
PROFILING_DIR		= profiling

EXEC	= newtonPC
OBJS	= main.o interface.o criticante.o sistema.o utils.o utilidades.o

T1_EXEC			= $(addsuffix _t1, $(EXEC))
T2_EXEC			= $(EXEC)
T1_LINHA_EXEC	= $(addsuffix _t1_linha, $(EXEC))

T1_OBJS			= $(addprefix $(T1_DIR)/, $(OBJS))
T2_OBJS			= $(addprefix $(T2_DIR)/, $(OBJS) Rosenbrock.o)
T1_LINHA_OBJS	= $(addprefix $(T1_LINHA_DIR)/, $(OBJS) Rosenbrock.o)

.PHONY: clean graph t1-linha t1-linha-profiling t1-linha-likwid t1l t1lp t1ll t2-profiling t2-likwid t2 t2p t2l

t1l: CFLAGS = $(NORMAL_CFLAGS)
t1l: t1-linha

t1-linha: $(T1_LINHA_EXEC)

$(T1_LINHA_EXEC): $(T1_LINHA_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

t1lp: t1-linha-profiling

t1-linha-profiling: CFLAGS = $(PROFILING_CFLAGS)
t1-linha-profiling: LFLAGS = $(PROFILING_LFLAGS)
t1-linha-profiling: $(T1_LINHA_EXEC)

t1ll: t1-linha-likwid

t1-linha-likwid: CFLAGS = $(LIKWID_CFLAGS)
t1-linha-likwid: LFLAGS = $(LIKWID_LFLAGS)
t1-linha-likwid: $(T1_LINHA_EXEC)

t2: CFLAGS = $(NORMAL_CFLAGS)
t2: $(T2_EXEC)

$(T2_EXEC): $(T2_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

t2p: t2-profiling

t2-profiling: CFLAGS = $(PROFILING_CFLAGS)
t2-profiling: LFLAGS = $(PROFILING_LFLAGS)
t2-profiling: $(T2_EXEC)

t2l: t2-likwid

t2-likwid: CFLAGS = $(LIKWID_CFLAGS)
t2-likwid: LFLAGS = $(LIKWID_LFLAGS)
t2-likwid: $(T2_EXEC)

graphics: t1ll t2l
	# ./gera_graficos.py $(T1_LINHA_EXEC) $(T2_EXEC)

clean:
	rm -rf $(T1_EXEC) $(T1_LINHA_EXEC) $(T2_EXEC) $(T1_OBJS) $(T1_LINHA_OBJS) $(T2_OBJS) gmon.out
