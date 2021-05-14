//
// Created by iliya on 06.05.2021.
//

#pragma once

#include "const.h"
#include "map.h"
#include "view.cpp"

using namespace sf;

class Object {

public:
    int x, y = 0;
    sf::Clock clock1;

    Object(int X, int Y) {

        x = X;
        y = Y;

        clock1.restart();
    }
    Object(Object const &bomb1) {
        x = bomb1.x;
        y = bomb1.y;
        clock1 = bomb1.clock1;
    }

    virtual bool isExploded()  = 0;
    virtual String getClassName() = 0;

    virtual ~Object() = default;
    virtual int getFireBuff() = 0;

};



class Bomb : public Object {
    int explosion_time;
    int fire_buff = 1;
public:

    Bomb(int X, int Y)
        : Object(X, Y) {
        explosion_time = 3;
        TileMap[y][x] = BOMB_CONST;
    }

    bool isExploded()  {
        sf::Time elapsed = clock1.getElapsedTime();
        if (elapsed.asSeconds() >= float(explosion_time)) {
            return true;
        }
        else
            return false;
    }
    String getClassName() {
        return "Bomb";
    }
    void setFireBuff(int buff) {
        fire_buff = buff;
    }
    int getFireBuff() {
        return fire_buff;
    }
    ~Bomb() {
        TileMap[y][x] = FIRE_CONST; //do not create Fire object
    }

};
class Fire : public Object {
    int explosion_time;
public:

    Fire(int X, int Y)
        : Object(X, Y){
        explosion_time = 2;
        TileMap[y][x] = FIRE_CONST;
    }

    bool isExploded()  {
        sf::Time elapsed = clock1.getElapsedTime();
        if (elapsed.asSeconds() >= float(explosion_time)) {
            return true;
        }
        else
            return false;
    }
    String getClassName() {
        return "Fire";
    }
    ~Fire() {
        TileMap[y][x] = VOID_CONST;
    }
    int getFireBuff() {return 0;}
};

bool operator != (const Object& left, const Object& right) {

    bool flag = true;
    if (left.x == right.x)
        flag = false;
    if (left.y == right.y)
        flag = false;
    return flag;
}

#define PL_SPEED 0.3
#define PL_ANIMATION 0.005
class Player {

private:
    float x, y, width, height, dx, dy, speed = 0; // coordinates x and y, width, height, and speed parameters.
    float speed_buff = 1;
    int fire_buff = 1;
    int direction = 0; // 0 - right, 1 - up and so on
    bool life = false;

    Texture texture;


public:


    Sprite sprite;
    Player(const String& F, float X, float Y, float W, float H) {
        life = true;
        dx = 0;
        dy = 0;

        width = W;
        height = H;
        Image image;
        image.loadFromFile(F);
        image.createMaskFromColor(Color(255, 0, 0));
        //Texture texture;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        x = X;
        y = Y;
        sprite.setTextureRect(IntRect(0, 0, width, height));
    }
    void update(float time);
    void keyboardReact(float &Frame, float time);
    void mapInteract();
    float getCoordinateX()  const {return x;}
    float getCoordinateY()  const {return y;}
    int getFireBuff()       const {return fire_buff;}

    Bomb* plantBomb() const;


};





void Player::keyboardReact(float &Frame, float time) {

    if (life == false ) { return; }
/////////////////////////////////////////////////////////
// move control buttons
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))) {
        direction = 0;
        speed = PL_SPEED * speed_buff;
        Frame += PL_ANIMATION * time;
        if (Frame > 3) Frame -= 3;
        sprite.setTextureRect(IntRect(int(width * int(Frame)),
                                      2 * height, int(width), int(height)));
        //getPlayerCoordinateForView(getCoordinateX(), getCoordinateY());
        //to make camera independent it centers on player only after key pressed
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (sf::Keyboard::isKeyPressed(sf::Keyboard::W)))) {
        direction = 1;
        speed = PL_SPEED * speed_buff;
        Frame += PL_ANIMATION * time;
        if (Frame > 3) Frame -= 3;
        sprite.setTextureRect(IntRect(int(width * int(Frame)),
                                      3 * height, int(width), int(height)));
        //getPlayerCoordinateForView(getCoordinateX(), getCoordinateY());
        //to make camera independent it centers on player only after key pressed
    }
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (sf::Keyboard::isKeyPressed(sf::Keyboard::A)))) {
        direction = 2;
        speed = PL_SPEED * speed_buff;
        Frame += PL_ANIMATION * time;
        if (Frame > 3) Frame -= 3;
        sprite.setTextureRect(IntRect(int(height * int(Frame)),
                                      int(width), int(height), int(width)));
        //getPlayerCoordinateForView(getCoordinateX(), getCoordinateY());
        //to make camera independent it centers on player only after key pressed
    }
    //////////////////////////////
    /*


    НЕ ЗАБЫТЬ ПРОВЕРИТЬ СООТВЕТСТВИЕ WIDTH И HEIGHT
    ВОЗМОЖНО ПРИДЕТСЯ ПОМЕНЯТЬ ИХ МЕСТАМИ
    В ФУНКЦИИ НАЛЕВО УЖЕ ИЗМЕНЕНЫ МЕСТАМИ ЧТОБЫ ПРИ ТЕСТАХ ЗАМЕТИТЬ
    ПРОВЕРИТЬ НА НЕКВАДРАТНЫХ СПРАЙТАХ
    ПРИ ТЕСТАХ НА КВАДРАТНОМ ВРОДЕ КАК НАДО ЛЕВУЮ ФУНКЦИЮ СДЕЛАТЬ КАК ОСТАЛЬНЫЕ

    */
    //////////////////////////////

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (sf::Keyboard::isKeyPressed(sf::Keyboard::S)))) {
        direction = 3;
        speed = PL_SPEED * speed_buff;
        Frame += PL_ANIMATION * time;
        if (Frame > 3) Frame -= 3;
        sprite.setTextureRect(IntRect(int(width * int(Frame)),
                                      0 * height, int(width), int(height)));
        //getPlayerCoordinateForView(getCoordinateX(), getCoordinateY());
        //to make camera independent it centers on player only after key pressed
    }
//////////////////////////////////////

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {



    }

}
void Player::update(float time) {

    switch(direction) {
        case 0:
            dx = speed;
            dy = 0;
            break;
        case 1:
            dx = 0;
            dy = -speed;
            break;
        case 2:
            dx = -speed;
            dy = 0;
            break;
        case 3:
            dx = 0;
            dy = speed;
            break;
        default:
            std::cout << "Mistake in Player::update";
    }
    x += dx * time;
    y += dy * time;

    speed = 0; //to stop it
    sprite.setPosition(x, y);
    mapInteract();
}
void Player::mapInteract() {
    for (int i = y / MAP_COMM_SIZE; i < (y + height) / MAP_COMM_SIZE; i++)
        for (int j = x / MAP_COMM_SIZE; j< (x + width) / MAP_COMM_SIZE; j++) {
            if (TileMap[i][j] == EDGE_CONST) {
                if (dy > 0) {
                    y = i * MAP_COMM_SIZE - height;
                }
                if (dy < 0) {
                    y = (i + 1) * MAP_COMM_SIZE;
                }
                if (dx > 0) {
                    x = j * MAP_COMM_SIZE - width;
                }
                if (dx < 0) {
                    x = (j + 1) * MAP_COMM_SIZE;
                }
            }

            if (TileMap[i][j] == BOX_CONST) {
                fire_buff += 1;
                TileMap[i][j] = VOID_CONST;
            }
            if (TileMap[i][j] == FIRE_CONST) {
                life = false;
            }
        }
}

Bomb* Player::plantBomb() const {

    int i = (int(y) + int(height) / 2 ) / MAP_COMM_SIZE;
    int j = (int(x) + int(width)  / 2 ) / MAP_COMM_SIZE;
    Bomb* bomb_ptr = new Bomb(j, i);
    bomb_ptr->setFireBuff(getFireBuff());
    //std::cout << "planted" << std::endl;
    return bomb_ptr;
}
