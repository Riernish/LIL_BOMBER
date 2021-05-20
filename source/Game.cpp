
#include "const.h"
#include "classes.h"
#include "menu/menu.cpp"

#include <algorithm>



void draw_map(sf::Sprite &map, sf::RenderWindow &window);
void check_objects(std::list <Object*>& objects, int fire_buff);
void make_fire  (std::list <Object*>& objects, Object* item, int fire_buff);

void gameRunning();
void generateMap();

#define TIME_CONSTANT_WE_CAN_ALLOW 1400

#define END_PATH "source/images/endgame.jpg"
#define MAIN_MUSIC "source/music/derevna_durakov.ogg"
#define VICTORY_MUSIC "source/music/victory.ogg"


/*
 *
 * main function of this game
 * returns true if restart button pressed
 * returns false otherwise
 *
 */
bool startGame() {

    generateMap();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "LIL BOMBER", sf::Style::Fullscreen);

    // , sf::Style::Fullscreen

    menu(window);

    sf::Music music;
    music.openFromFile(MAIN_MUSIC);
    music.setVolume(50);
    music.play();
    music.setLoop(true);

    sf::Music victory_music;
    victory_music.openFromFile(VICTORY_MUSIC);




    view.reset(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

//////////////////////////
    sf::Image map_image;
    map_image.loadFromFile(MAP_PATH);
    sf::Texture map_texture;
    map_texture.loadFromImage(map_image);
    sf::Sprite map_sprite;
    map_sprite.setTexture(map_texture);
//////////////////////////

    sf::Image endgame_image;
    endgame_image.loadFromFile(END_PATH);
    sf::Texture endgame_texture;
    endgame_texture.loadFromImage(endgame_image);
    sf::Sprite endgame;
    endgame.setTexture(endgame_texture);
    //endgame.rotate(90);
    endgame.scale(2.5, 0.5);
    endgame.setPosition(450, 275);

    Font font;
    font.loadFromFile("source/Font/alpha_echo.ttf");
    Text text("", font, 128);
    text.setFillColor(Color::Black);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);



    Player hero_1("source/images/blue48.png", 64, 64, PL_TEXTURE_SIZE, PL_TEXTURE_SIZE, "WASD");
    Player hero_2("source/images/red48.png", 23 * 64, 12 * 64, PL_TEXTURE_SIZE, PL_TEXTURE_SIZE, "ARROWS");


    Clock clock;
    float CurrentFrame = 0;
    bool win_1;
    bool win_2;
    int flag_saved = 3;

    std::list <Object*> bomb_map;

    window.setKeyRepeatEnabled(false); //disable repetitions

    while (window.isOpen()) {

        float time = clock.restart().asMicroseconds();
        time /= TIME_CONSTANT_WE_CAN_ALLOW;


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Tab)) { return true; }//restart game
        if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//close game

        hero_1.keyboardReact(CurrentFrame, time, bomb_map, event);
        hero_2.keyboardReact(CurrentFrame, time, bomb_map, event);

        check_bombs(bomb_map, hero_1.getFireBuff());
        check_bombs(bomb_map, hero_2.getFireBuff());

        win_1 = hero_1.update(time);
        win_2 = hero_2.update(time);


        window.setView(view);
        viewMap(time);
        viewChange();

        window.clear(Color(128,106,89)); // color to fill my emptiness

        draw_map(map_sprite, window);
        int flag = win_1 * 1 + win_2 * 2;
        if (flag_saved == 3 && flag != 3) { // to show victory message
            flag_saved = flag;
            music.stop();
            victory_music.play();
            victory_music.setLoop(true);

        }
        switch (flag_saved) {
            case 1:

                text.setString("Blue wins!!!");
                flag_saved = 1;
                text.setPosition(500, 300);
                text.setFillColor(sf::Color::Blue);
                hero_1.kill();
                window.draw(endgame);
                window.draw(text);
                break;
            case 2:

                text.setString("Red wins!!!");
                flag_saved = 2;
                text.setPosition(500, 300);
                text.setFillColor(sf::Color::Red);
                hero_2.kill();
                window.draw(endgame);
                window.draw(text);
                break;
            case 3:
                break;
            default:
                std::cout << "Something went wrong with players life state";
        }


        window.draw(hero_1.sprite);
        window.draw(hero_2.sprite);

        window.display();
    }

    return false;
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

/*
 *
 * checks bombs and fire
 * if they explode
 * deletes them
 *
 */
void check_objects(std::list <Object*>& objects, int fire_buff) {

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

}

void gameRunning() {
    if (startGame()) {gameRunning(); }
}

void generateMap() {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    for (int i = 0; i < HEIGHT_MAP; i++)
        for (int j = 0; j < WIDTH_MAP; j++) {
            TileMap[i][j] = TileMap_original[i][j];
            if (TileMap[i][j] == BOX_CONST) {
                unsigned marvel = mersenne() % 10;
                switch (marvel) {//here you can work with probabilities
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        break;
                    case 5:
                        TileMap[i][j] = SPDBUFF_BOX_CONST;
                        break;
                    case 6:
                    case 7:
                    case 8:
                        break;
                    case 9:
                        TileMap[i][j] = FBUFF_BOX_CONST;
                        break;
                    default:
                        break;
                }

            }

        }
}