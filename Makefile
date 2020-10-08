CC = gcc
OBJECTS = tempomap.c stb_image.h stb_image_write.h
SAFE = -Wall -Wextra -O2 -fsanitize=address
FAST = -O3
LIBS = -lm -pthread
#BINDIR = $(DESTDIR)/usr/bin
NAME = tempomap

safe: $(OBJECTS)
	$(CC) $(SAFE) -o $(NAME) $(OBJECTS) $(LIBS)
fast: $(OBJECTS)
	$(CC) $(FAST) -o $(NAME) $(OBJECTS) $(LIBS)
run: tempomap
	./$(NAME)
clean:
	rm $(NAME)
