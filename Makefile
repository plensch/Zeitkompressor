CC = gcc
WINCC = x86_64-w64-mingw32-gcc
OBJECTS = tempomap.c stb_image.h stb_image_write.h
SAFE = -Wall -Wextra -fsanitize=address
CHECK = -fsyntax-only -Wall -Wextra
FAST = -O3 -static -s
LIBS = -lm
NAME = tempomap

fast: $(OBJECTS)
	$(CC) $(FAST) -o $(NAME) $(OBJECTS) $(LIBS)
safe: $(OBJECTS)
	$(CC) $(SAFE) -o $(NAME) $(OBJECTS) $(LIBS)
check:
	$(CC) $(SAFE) -o $(NAME) $(OBJECTS) $(LIBS)
windows: $(OBJECTS)
	$(WINCC) $(FAST) -o $(NAME).exe $(OBJECTS) $(LIBS)
clean:
	rm $(NAME)
