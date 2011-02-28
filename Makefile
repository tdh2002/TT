
CC = gcc

main: main.c drawui.c callback.c content_en.c drawfb.c drawui.h base.h language.h drawfb.h
	$(CC) main.c drawui.c drawfb.c callback.c content_en.c -o main -DX86 `pkg-config --cflags --libs gtk+-2.0` -lm -Wall

.PHONY:clean
clean: 
	rm -f *.o main
