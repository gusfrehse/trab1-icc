CC		= gcc
CFLAGS	= -std=gnu17 -O3 -mavx -march=native -DLIKWID_PERFMON -I${LIKWID_INCLUDE}
LFLAGS	= -lm -L${LIKWID_LIB} -llikwid
PROFILING = -std=gnu17 -O0 -pg -mavx -march=native -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -fno-inline

PROG	= newtonPC
OBJS	= main.o interface.o criticante.o sistema.o utils.o utilidades.o Rosenbrock.o


.PHONY: clean all run profiling

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROG)

run: all
	./gera_rosenbrock_simples.sh | likwid-perfctr -C 3 -g L3 -m ./$(PROG)

profiling: $(OBJS)
	$(CC) $(PROFILING) -o $(PROG) $^ $(LFLAGS)
	./gera_rosenbrock_simples.sh | ./$(PROG)
	gprof ./$(PROG) gmon.out
