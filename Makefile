all: main.c scheduler.o
	gcc -Wall main.c scheduler.o

scheduler.o: scheduler.c
	gcc -Wall -c scheduler.c

clean:
	rm -fr *.o *.out

#Falta colocar os arquivos em uma pasta separada src e build