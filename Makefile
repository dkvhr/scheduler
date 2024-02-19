all: main.c scheduler.o queue.o
	gcc -Wall main.c scheduler.o queue.o

scheduler.o: scheduler.c
	gcc -Wall -c scheduler.c

queue.o: queue.c
	gcc -Wall -c queue.c

run:
	./a.out

clean:
	rm -fr *.o *.out

#Falta colocar os arquivos em uma pasta separada src e build