//
// Created by iliya on 07.05.2021.
//

#include <SFML/Graphics.hpp>

#define MAP_PATH "images/map_x2_2.png"

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
        "0          s            0",
        "0   s               ss s0",
        "0     s  0 0            0",
        "0                       0",
        "0       xxxxx    q      0",
        "0                       0",
        "0      pppp           000",
        "0       s         000   0",
        "0                       0",
        "0                       0",
        "0                       0",
        "0                       0",
        "0000000000000000000000000",
};