CC = gcc
OBJECTS = tempomap.c stb_image.h stb_image_write.h
CFLAGS = -Wall -Wextra -O2
LIBS = -lm
#BINDIR = $(DESTDIR)/usr/bin
NAME = tempomap

tempomap: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(LIBS)
run: tempomap
	./$(NAME)
clean:
	rm $(NAME)
