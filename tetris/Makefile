.PHONY:clean
OBJ = tetris.o def.o

cc = gcc
CFLAGS = -Wall -g

all:tetris

tetris: $(OBJ)
	$(cc) $(CFLAGS) $^ -o $@

%.o : %.c
	gcc -c $< -o $@

clean:
	rm -rf $(OBJ)
