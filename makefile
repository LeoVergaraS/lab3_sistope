FLAGS = -Wall -lm

lab3: lab3.c funciones.o
	gcc $(FLAGS) -pthread -o lab3 lab3.c

funciones.o: funciones.c 
	gcc -c $(FLAGS) funciones.c

clear:
	rm -f *.o lab3