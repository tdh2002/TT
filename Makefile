
CC = gcc

main: main.c drawui.c callback.c drawui.h base.h content_en.h
	$(CC) main.c drawui.c callback.c -o main `pkg-config --cflags --libs gtk+-2.0` -Wall

.PHONY:clean
clean: 
	rm -f *.o main
