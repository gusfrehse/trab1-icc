CC		= gcc
CFLAGS	= -std=gnu17 -O3 -mavx -march=native
LFLAGS	= -lm

PROG	= newtonPC
OBJS	= main.o interface.o criticante.o sistema.o utils.o utilidades.o Rosenbrock.o

.PHONY: clean all

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean :
	rm -f $(OBJS) $(PROG)