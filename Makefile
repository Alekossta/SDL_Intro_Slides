compiler=gcc
LIBRARIES = -lSDL2

build-and-run: main.c 
	$(compiler) main.c $(LIBRARIES)
	./a.out