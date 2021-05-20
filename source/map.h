//
// Created by iliya on 07.05.2021.
//

#include <SFML/Graphics.hpp>

#define MAP_PATH "source/images/map_x2_2.png"

const int HEIGHT_MAP = 14;
const int WIDTH_MAP = 25;
#define MAP_COMM_SIZE 64

#define VOID_CONST ' '
#define BOMB_CONST 'b'
#define EDGE_CONST '0'
#define FIRE_CONST 'f'
#define BOX_CONST  's'
#define SPDBUFF_BOX_CONST 'x'
#define SPDBUFF_CONST 'q'
#define FBUFF_BOX_CONST 'p'
#define FBUFF_CONST 'o'

sf::String TileMap[HEIGHT_MAP] = {
        "0000000000000000000000000",
        "0      ssxssssppssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0 0",
        "0ssssssssssssssssssssss 0",
        "0 0 0 0 0 0 0 0 0 0 0 0 0",
        "0000000000000000000000000",
};
sf::String const TileMap_original[HEIGHT_MAP] = {
        "0000000000000000000000000",
        "0      sssssssppssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0s0",
        "0sssssssssssssssssssssss0",
        "0s0s0s0s0s0s0s0s0s0s0s0 0",
        "0ssssssssssssssssssssss 0",
        "0 0 0 0 0 0 0 0 0 0 0 0 0",
        "0000000000000000000000000",
};
/*
sf::String TileMap[HEIGHT_MAP] = {
        "0000000000000000000000000",
        "0          s            0",
        "0 0   s  0 0            0",
        "0   s               ss s0",
        "0 0     0  0000         0",
        "0       0  0            0",
        "0 0     0  0      q     0",
        "0       0000000         0",
        "0 0        0  0       000",
        "0          0  0    00   0",
        "0 0     0000  0         0",
        "0                       0",
        "0 0 0 0 0 0 0 0 0 0 0 0 0",
        "0000000000000000000000000",
};*/
