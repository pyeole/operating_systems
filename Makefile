#----------------------Targets-----------------------------
all:mythread.a queue.o my_thread_library.o

mythread.a: queue.o my_thread_library.o
	ar rcs mythread.a queue.o my_thread_library.o

queue.o: queue.c
	gcc -I. queue.c -o queue.o -c -w

my_thread_library.o: my_thread_library.c
	gcc -I. my_thread_library.c -o my_thread_library.o -c -w

clean:
	rm -rf mythread.a queue.o my_thread_library.o
