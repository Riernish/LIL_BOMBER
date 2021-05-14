//
// Created by iliya on 08.05.2021.
//

#include "view.h"

void getPlayerCoordinateForView(float x,float y) {

    view.setCenter(x + VIEW_DEF_CONST, y);


}
void viewMap(float time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        view.move(VIEW_VOL*time, 0);//right
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
        view.move(0, -VIEW_VOL * time);//up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        view.move(0, VIEW_VOL * time);//down
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
        view.move(-VIEW_VOL*time, 0);//left
    }
}
void viewChange() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
        view.zoom(1.0100f);
        //view.zoom(1.0006f); //slower zoom
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        //view.setRotation(90);//turns camera
        view.rotate(0.3);// slowly turns camera
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        //view.setRotation(90);//turns camera
        view.rotate(-0.3);// slowly turns camera another side
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);//basic size
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        view.setSize(FUNNY_WIDTH, FUNNY_HEIGHT);//funny size
    }


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));//split screen
    }
}
