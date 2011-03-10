
CC = gcc
LD = gcc
TARGET=main

CFLAGS= -Wall -Os -DX86 `pkg-config --cflags webkit-1.0`
LDFLAGS= -lm `pkg-config --libs webkit-1.0`
OBJS+= main.o drawui.o drawfb.o callback.o content_en.o file_op.o

%.o:%.c %.h base.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@  

all:$(TARGET)

$(TARGET):$(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

#$(OBJS):%.o:%.c %.h
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@ 

.PHONY:clean
clean: 
	rm -f *.o $(TARGET) $(OBJS)
