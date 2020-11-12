# Target library
lib := libuthread.a

all: context.o preempt.o queue.o sem.o uthread.o
	ar rcs $(lib) $^
	rm -f $^

context.o: context.c private.h uthread.h
	gcc -c -o $@ $<

preempt.o: preempt.c private.h uthread.h
	gcc -c -o $@ $<

queue.o: queue.c queue.h
	gcc -c -o $@ $<

sem.o: sem.c private.h sem.h queue.h
	gcc -c -o $@ $<

uthread.o: uthread.c uthread.h private.h queue.h
	gcc -c -o $@ $<

clean:
	rm -f libuthread.a
