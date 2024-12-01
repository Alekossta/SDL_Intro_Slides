compiler=gcc
LIBRARIES = -lSDL2 -lSDL2_image

build-and-run: main.c 
	$(compiler) main.c $(LIBRARIES)
	./a.out