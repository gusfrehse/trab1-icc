CC		= gcc
CFLAGS	= -std=gnu17
LFLAGS	= -lm

PROG	= newtonPC
OBJS	= main.o interface.o criticante.o sistema.o utils.o utilidades.o Rosenbrock.o

.PHONY: clean all

$(PROG) : $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean :
	rm -f $(OBJS) $(PROG)