

main : main.o
main.o : main.c
	
clean :
	rm -f *.o
	rm -f main