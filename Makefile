
CC = gcc

main: main.c
	$(CC) main.c drawui.c -o main `pkg-config gtk+-2.0 --cflags --libs` -Wall

.PHONY:clean
clean: 
	rm -f *.o main
