CC=g++


all: game_compile

game_compile:
	$(CC) -c -O3 -W  source/main.cpp
	$(CC) main.o -O3 -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
