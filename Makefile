CC=g++


all: game_compile

game_compile:
	$(CC) -c main.cpp
	$(CC) main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
