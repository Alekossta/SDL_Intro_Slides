compiler=gcc
LIBRARIES = -lSDL2 -lSDL2_image
FPS ?= 60

build-and-run: main.c 
	$(compiler) main.c $(LIBRARIES) -D FPS=$(FPS)
	./a.out