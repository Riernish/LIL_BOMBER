//
// Created by iliya on 08.05.2021.
//

#include "SFML/Graphics.hpp"
#include "const.h"

#define VIEW_DEF_CONST 100
#define VIEW_VOL 0.3

#define FUNNY_WIDTH  4 * 5 * WINDOW_COMM_SIZE
#define FUNNY_HEIGHT 3 * 5 * WINDOW_COMM_SIZE

using namespace sf;

sf::View view;


void getPlayerCoordinateForView(float x,float y);
void viewMap(float time);
void viewChange();
