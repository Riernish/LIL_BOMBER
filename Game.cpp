
#include "const.h"
#include "classes.h"
#include "menu.cpp"

#include <algorithm>


void draw_map(sf::Sprite &map, sf::RenderWindow &window);
void check_bombs(std::list <Object*>& objects, int fire_buff);
void make_fire  (std::list <Object*>& objects, Object* item, int fire_buff);

void gameRunning();

#define TIME_CONSTANT_WE_CAN_ALLOW 1400

bool startGame() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!", sf::Style::Fullscreen);

    // , sf::Style::Fullscreen

    menu(window);


    view.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

//////////////////////////
    sf::Image map_image;
    map_image.loadFromFile(MAP_PATH);
    sf::Texture map_texture;
    map_texture.loadFromImage(map_image);
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);

    Font font;
    font.loadFromFile("Font/alpha_echo.ttf");
    Text text("", font, 128);
    text.setFillColor(Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);



    Player hero_1("images/pers48.png", 64, 64, PL_TEXTURE_SIZE, PL_TEXTURE_SIZE, "WASD");
    Player hero_2("images/pers48.png", 23 * 64, 12 * 64, PL_TEXTURE_SIZE, PL_TEXTURE_SIZE, "ARROWS");


    Clock clock;
    float CurrentFrame = 0;

    std::list <Object*> bomb_map;

    window.setKeyRepeatEnabled(false);

    while (window.isOpen()) {

        float time = clock.restart().asMicroseconds();
        time /= TIME_CONSTANT_WE_CAN_ALLOW;


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            /*if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    bomb_map.push_back(hero_2.plantBomb());
                }
                if (event.key.code == sf::Keyboard::LShift) {
                    bomb_map.push_back(hero_1.plantBomb());
                }
            }*/
        }

        if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//restart game
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//close game

        hero_1.keyboardReact(CurrentFrame, time, bomb_map, event);
        hero_2.keyboardReact(CurrentFrame, time, bomb_map, event);

        check_bombs(bomb_map, hero_1.getFireBuff());
        check_bombs(bomb_map, hero_2.getFireBuff());

        //std::cout << bomb_map.size() << std::endl;
        hero_1.update(time);
        hero_2.update(time);

        window.setView(view);
        viewMap(time);
        viewChange();

        window.clear(Color(128,106,89)); // color to fill my emptiness
        draw_map(map_sprite, window);

        text.setString("Zubko lox");
        //text.setPosition(view.getCenter().x - 100, view.getCenter().y - 100);
        window.draw(text);

        window.draw(hero_1.sprite);
        window.draw(hero_2.sprite);

        window.display();
    }

    return 0;
}

void draw_map(sf::Sprite &map, sf::RenderWindow &window) {
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++) {
            if (TileMap[i][j] == VOID_CONST)  map.setTextureRect(IntRect(0, 0,
                                                                         MAP_COMM_SIZE, MAP_COMM_SIZE)); // first block image
            if (TileMap[i][j] == BOX_CONST
                || TileMap[i][j] == FBUFF_BOX_CONST
                || TileMap[i][j] == SPDBUFF_BOX_CONST)  map.setTextureRect(IntRect(MAP_COMM_SIZE, 0,
                                                                                   MAP_COMM_SIZE, MAP_COMM_SIZE));//second
            if (TileMap[i][j] == EDGE_CONST) map.setTextureRect(IntRect(2 * MAP_COMM_SIZE, 0,
                                                                        MAP_COMM_SIZE, MAP_COMM_SIZE));//and so on
            if (TileMap[i][j] == BOMB_CONST) map.setTextureRect(IntRect(3 * MAP_COMM_SIZE, 0,
                                                                        MAP_COMM_SIZE, MAP_COMM_SIZE));
            if (TileMap[i][j] == FIRE_CONST) map.setTextureRect(IntRect(4 * MAP_COMM_SIZE, 0,
                                                                        MAP_COMM_SIZE, MAP_COMM_SIZE));
            if (TileMap[i][j] == SPDBUFF_CONST) map.setTextureRect(IntRect(5 * MAP_COMM_SIZE, 0,
                                                                           MAP_COMM_SIZE, MAP_COMM_SIZE));
            if (TileMap[i][j] == FBUFF_CONST) map.setTextureRect(IntRect(6 * MAP_COMM_SIZE, 0,
                                                                         MAP_COMM_SIZE, MAP_COMM_SIZE));


            map.setPosition(j * MAP_COMM_SIZE, i * MAP_COMM_SIZE);

            window.draw(map);
        }
}

/*void check_bombs(std::list <Object*>& objects, int fire_buff) {
    std::list<Object*> new_objects;

    for (auto it = objects.begin(); it != objects.end(); ++it) {
        auto item = (*it);
        if (item->isExploded()) {
            if (item->getClassName() == "Bomb")
                make_fire(new_objects, item, item->getFireBuff());
            delete item;
        } else {
            new_objects.push_back(item);
        }
    }

    objects.clear();
    objects = new_objects;
}*/

void check_bombs(std::list <Object*>& objects, int fire_buff) {

    auto it_new_end = std::remove_if(objects.begin(), objects.end(), [&objects, fire_buff](Object* item) {
        bool res = item ->isExploded();
        if (res) {
            //TileMap[item ->y][item ->x] = ' ';
            if (item ->getClassName() == "Bomb") {

                make_fire(objects, item, item ->getFireBuff());
            }
            delete item;
        }
        return res;
    });

    objects.erase(it_new_end, objects.end());

    /*for (it = objects.begin(); it != objects.end(); it++) {
        if ((*it).isExploded()) {
            int x = (*it).x;
            int y = (*it).y;
            TileMap[y][x] = ' ';
            it = objects.erase(it);
            it--;
        }

    }*/

}


/*
 * in this function in order to prevent segmentation faults
 * created 4 independent cycles
 * each cycle draws its own single line
 * of bomb plus-formed explosion trace
 */
void make_fire( std::list <Object*>& objects, Object*item, int fire_buff) {
    int box_type = 0;
    bool stop_now = true;
    objects.push_back(new Fire(item -> x, item -> y, box_type));
    for (int i = 1; i <= fire_buff; i++) {
        if (TileMap[item->y + i][item->x] == EDGE_CONST)
            break;
        switch(TileMap[item->y + i][item->x]) {
            case BOX_CONST:
                box_type = 1;
                break;
            case SPDBUFF_BOX_CONST:
                box_type = 2;
                break;
            case FBUFF_BOX_CONST:
                box_type = 3;
                break;
            case FIRE_CONST:
            case BOMB_CONST:
            case VOID_CONST:
                box_type = 0;
                stop_now = false;
                break;
            default:
                std::cout << "Error in make_fire" << std::endl;
        }
        objects.push_back(new Fire(item ->x, item ->y + i, box_type));
        if (stop_now)
            break;
        stop_now = true;
    }
    for (int i = 1; i <= fire_buff; i++) {
        if (TileMap[item->y - i][item->x] == EDGE_CONST)
            break;
        switch(TileMap[item->y - i][item->x]) {
            case BOX_CONST:
                box_type = 1;
                break;
            case SPDBUFF_BOX_CONST:
                box_type = 2;
                break;
            case FBUFF_BOX_CONST:
                box_type = 3;
                break;
            case FIRE_CONST:
            case BOMB_CONST:
            case VOID_CONST:
                box_type = 0;
                stop_now = false;
                break;
            default:
                std::cout << "Error in make_fire" << std::endl;
        }
        objects.push_back(new Fire(item ->x, item ->y - i, box_type));
        if (stop_now)
            break;
        stop_now = true;
    }
    for (int i = 1; i <= fire_buff; i++) {
        if (TileMap[item->y][item->x + i] == EDGE_CONST)
            break;
        switch(TileMap[item->y][item->x + i]) {
            case BOX_CONST:
                box_type = 1;
                break;
            case SPDBUFF_BOX_CONST:
                box_type = 2;
                break;
            case FBUFF_BOX_CONST:
                box_type = 3;
                break;
            case FIRE_CONST:
            case BOMB_CONST:
            case VOID_CONST:
                box_type = 0;
                stop_now = false;
                break;
            default:
                std::cout << "Error in make_fire" << std::endl;
        }
        objects.push_back(new Fire(item ->x + i, item ->y , box_type));
        if (stop_now)
            break;
        stop_now = true;
    }
    for (int i = 1; i <= fire_buff; i++) {
        if (TileMap[item->y][item->x - i] == EDGE_CONST)
            break;
        switch(TileMap[item->y][item->x - i]) {
            case BOX_CONST:
                box_type = 1;
                break;
            case SPDBUFF_BOX_CONST:
                box_type = 2;
                break;
            case FBUFF_BOX_CONST:
                box_type = 3;
                break;
            case FIRE_CONST:
            case BOMB_CONST:
            case VOID_CONST:
                box_type = 0;
                stop_now = false;
                break;

            default:
                std::cout << "Error in make_fire" << std::endl;
        }
        objects.push_back(new Fire(item ->x - i, item ->y , box_type));
        if (stop_now)
            break;
        stop_now = true;
    }
    /*
     * Создать треды тута !!!!1
     *
     */
}

void gameRunning() {
    if (startGame()) {gameRunning(); }
}