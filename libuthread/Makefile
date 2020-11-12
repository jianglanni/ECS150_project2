# Target library
lib := libuthread.a
CFLAGS	:= -Wall -Werror -Wextra

all: context.o preempt.o queue.o sem.o uthread.o
	ar rcs $(lib) $^
	rm -f $^

context.o: context.c private.h uthread.h
	gcc -c $(CFLAGS) -o $@ $<

preempt.o: preempt.c private.h uthread.h
	gcc -c $(CFLAGS) -o $@ $<

queue.o: queue.c queue.h
	gcc -c $(CFLAGS) -o $@ $<

sem.o: sem.c private.h sem.h queue.h
	gcc -c $(CFLAGS) -o $@ $<

uthread.o: uthread.c uthread.h private.h queue.h
	gcc -c $(CFLAGS) -o $@ $<

clean:
	rm -f libuthread.a
