CC = gcc
all: twoD threeD
twoD: main.c 2d-game.c
	$(CC) main.c 2d-game.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o bin/2d-game
threeD: main.c 3d-game.c
	$(CC) main.c 3d-game.c -DTHREED -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o bin/3d-game
clean:
	rm -f bin/2d-game bin/3d-game
.PHONY: all clean
