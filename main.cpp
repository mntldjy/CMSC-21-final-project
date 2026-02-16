#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MainMenu.h"
#include "ElementMenu.h"
#include "fireElement.h"
#include "waterElement.h"
#include "airElement.h"
#include "landElement.h"
#include "FgameProper.h"
#include "AgameProper.h"
#include "WgameProper.h"
#include "LgameProper.h"
#include <vector>
#include <iostream>

using namespace sf;

int main() {
    // Get desktop resolution for fullscreen
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow MENU(desktop, "MAIN MENU", Style::Fullscreen);
    MainMenu menu(desktop.width, desktop.height);

    // Background music
    Music bgMusic;
    if (!bgMusic.openFromFile("images/music/bgm.wav")) {
        std::cerr << "Error loading background music\n";
    }
    else {
        bgMusic.setLoop(true);
        bgMusic.setVolume(100);
        bgMusic.play();
    }

    // Click sound effect
    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile("images/music/click.wav")) {
        std::cerr << "Error loading click sound effect\n";
    }
    Sound clickSound;
    clickSound.setBuffer(clickBuffer);
    clickSound.setVolume(100);

    // Fighting scene music
    Music fightMusic;
    if (!fightMusic.openFromFile("images/music/fight.wav")) {
        std::cerr << "Error loading fight music\n";
    }
    fightMusic.setLoop(true);
    fightMusic.setVolume(200);

    // Backgrounds
    RectangleShape background(Vector2f(desktop.width, desktop.height));
    Texture Maintexture;
    Maintexture.loadFromFile("images/background.jpg");
    background.setTexture(&Maintexture);

    RectangleShape PBackground(Vector2f(desktop.width, desktop.height));
    Texture back_texture;
    back_texture.loadFromFile("images/selection.jpg");
    PBackground.setTexture(&back_texture);

    RectangleShape IBackground(Vector2f(desktop.width, desktop.height));
    Texture Instruction_texture;
    Instruction_texture.loadFromFile("images/instructions.jpg");
    IBackground.setTexture(&Instruction_texture);

    RectangleShape CBackground(Vector2f(desktop.width, desktop.height));
    Texture Credits_texture;
    Credits_texture.loadFromFile("images/credits.jpg");
    CBackground.setTexture(&Credits_texture);

    while (MENU.isOpen()) {
        Event event;
        while (MENU.pollEvent(event)) {
            if (event.type == Event::Closed) {
                MENU.close();
            }

            if (event.type == Event::KeyReleased) {
                if (event.key.code == Keyboard::Up) {
                    menu.MoveUp();
                    break;
                }
                if (event.key.code == Keyboard::Down) {
                    menu.MoveDown();
                    break;
                }
                if (event.key.code == Keyboard::Return) {
                    clickSound.play();
                    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                    RenderWindow PLAY(desktop, "CHOOSE ELEMENT", Style::Fullscreen);
                    ElementMenu play(PLAY.getSize().x, PLAY.getSize().y);
                    RenderWindow INSTRUCTIONS(desktop, "INSTRUCTIONS", Style::Fullscreen);
                    RenderWindow CREDITS(desktop, "CREDITS", Style::Fullscreen);

                    int  x = menu.MainMenuPressed();
                    if (x == 0) {
                        while (PLAY.isOpen()) {
                            Event aevent;
                            while (PLAY.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed) {
                                    PLAY.close();
                                }
                                if (aevent.type == Event::KeyReleased) {
                                    if (aevent.key.code == Keyboard::Right) {
                                        play.MoveRight();
                                        break;
                                    }
                                    if (aevent.key.code == Keyboard::Left) {
                                        play.MoveLeft();
                                        break;
                                    }
                                    if (aevent.key.code == Keyboard::Space) {
                                        clickSound.play();
                                        sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                        PLAY.close();
                                    }
                                    if (aevent.key.code == Keyboard::Return) {
                                        clickSound.play();
                                        sf::sleep(sf::milliseconds(500)); //to synchronize sound effect

                                        VideoMode fullscreen = VideoMode::getDesktopMode();

                                        RenderWindow FIRE(fullscreen, "FIRE ELEMENT", Style::Fullscreen);
                                        fireElement fire(FIRE.getSize().x, FIRE.getSize().y);
                                        RenderWindow AIR(fullscreen, "AIR ELEMENT", Style::Fullscreen);
                                        airElement air(AIR.getSize().x, AIR.getSize().y);
                                        RenderWindow WATER(fullscreen, "WATER ELEMENT", Style::Fullscreen);
                                        waterElement water(WATER.getSize().x, WATER.getSize().y);
                                        RenderWindow LAND(fullscreen, "LAND ELEMENT", Style::Fullscreen);
                                        landElement land(LAND.getSize().x, LAND.getSize().y);


                                        int y = play.ElementSelected();

                                        if (y == 0) {
                                            while (FIRE.isOpen()) {
                                                Event fevent;
                                                while (FIRE.pollEvent(fevent)) {
                                                    if (fevent.type == Event::Closed) {
                                                        FIRE.close();
                                                    }
                                                    if (fevent.type == Event::KeyReleased) {
                                                        if (fevent.key.code == Keyboard::Space) {
                                                            clickSound.play();
                                                            sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                                            FIRE.close();
                                                        }
                                                    }
                                                    //if (fevent.type == Event::MouseButtonPressed) {
                                                    //    clickSound.play();
                                                    //    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect

                                                    //    Vector2f mousePos(fevent.mouseButton.x, fevent.mouseButton.y);
                                                    //    if (fire.getStartButton().getGlobalBounds().contains(mousePos)) {

                                                    /*if (fevent.type == Event::KeyReleased) {
                                                        if (fevent.key.code == Keyboard::Enter) {*/
                                                    if (fevent.type == Event::MouseButtonPressed) {
                                                            // change bgm
                                                            bgMusic.stop();
                                                            fightMusic.play();

                                                            // game logic starts here
                                                            FgameProper game;
                                                            game.run(fightMusic);

                                                            fightMusic.stop();

                                                            //back to main menu
                                                            LAND.close();
                                                            FIRE.close();
                                                            WATER.close();
                                                            AIR.close();
                                                            PLAY.close();

                                                            // change bgm back
                                                            bgMusic.play();
                                                    }
                                                }
                                                FIRE.clear();
                                                fire.draw(FIRE);
                                                WATER.close();
                                                AIR.close();
                                                LAND.close();
                                                FIRE.display();
                                            }
                                        }

                                        if (y == 1) {
                                            while (AIR.isOpen()) {
                                                Event aevent;
                                                while (AIR.pollEvent(aevent)) {
                                                    if (aevent.type == Event::Closed) {
                                                        AIR.close();
                                                    }
                                                    if (aevent.type == Event::KeyReleased) {
                                                        if (aevent.key.code == Keyboard::Space) {
                                                            clickSound.play();
                                                            sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                                            AIR.close();
                                                        }
                                                    }
                                                    //if (aevent.type == Event::MouseButtonPressed) {
                                                    //    clickSound.play();
                                                    //    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect

                                                    //    Vector2f mousePos(aevent.mouseButton.x, aevent.mouseButton.y);
                                                    //    if (fire.getStartButton().getGlobalBounds().contains(mousePos)) {

                                                    if (aevent.type == Event::MouseButtonPressed) {
                                                            // change bgm
                                                            bgMusic.stop();
                                                            fightMusic.play();

                                                            // game logic starts here
                                                            AgameProper game;
                                                            game.run(fightMusic);

                                                            fightMusic.stop();

                                                            //back to main menu
                                                            LAND.close();
                                                            FIRE.close();
                                                            WATER.close();
                                                            AIR.close();
                                                            PLAY.close();

                                                            // change bgm back
                                                            bgMusic.play();
                                                    }
                                                }
                                                AIR.clear();
                                                air.draw(AIR);
                                                FIRE.close();
                                                WATER.close();
                                                LAND.close();
                                                AIR.display();
                                            }
                                        }

                                        if (y == 2) {
                                            while (WATER.isOpen()) {
                                                Event wevent;
                                                while (WATER.pollEvent(wevent)) {
                                                    if (wevent.type == Event::Closed) {
                                                        WATER.close();
                                                    }
                                                    if (wevent.type == Event::KeyReleased) {
                                                        if (wevent.key.code == Keyboard::Space) {
                                                            clickSound.play();
                                                            sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                                            WATER.close();
                                                        }
                                                    }
                                                    //if (wevent.type == Event::MouseButtonPressed) {
                                                    //    clickSound.play();
                                                    //    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect

                                                    //    Vector2f mousePos(wevent.mouseButton.x, wevent.mouseButton.y);
                                                    //    if (fire.getStartButton().getGlobalBounds().contains(mousePos)) {

                                                    if(wevent.type == Event::MouseButtonPressed) {
                                                            // change bgm
                                                            bgMusic.stop();
                                                            fightMusic.play();

                                                            // game logic starts here
                                                            WgameProper game;
                                                            game.run(fightMusic);

                                                            fightMusic.stop();

                                                            //back to main menu
                                                            LAND.close();
                                                            FIRE.close();
                                                            WATER.close();
                                                            AIR.close();
                                                            PLAY.close();

                                                            // change bgm back
                                                            bgMusic.play();
                                                    }
                                                }
                                                WATER.clear();
                                                water.draw(WATER);
                                                FIRE.close();
                                                AIR.close();
                                                LAND.close();
                                                WATER.display();
                                            }
                                        }

                                        if (y == 3) {
                                            while (LAND.isOpen()) {
                                                Event levent;
                                                while (LAND.pollEvent(levent)) {
                                                    if (levent.type == Event::Closed) {
                                                        LAND.close();
                                                    }
                                                    if (levent.type == Event::KeyReleased) {
                                                        if (levent.key.code == Keyboard::Space) {
                                                            clickSound.play();
                                                            sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                                            LAND.close();
                                                        }
                                                    }
                                                    //if (levent.type == Event::MouseButtonPressed) {
                                                    //    clickSound.play();
                                                    //    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect

                                                    //    Vector2f mousePos(levent.mouseButton.x, levent.mouseButton.y);
                                                    //    if (fire.getStartButton().getGlobalBounds().contains(mousePos)) {

                                                    if (levent.type == Event::MouseButtonPressed) {
                                                            // change bgm
                                                            bgMusic.stop();
                                                            fightMusic.play();

                                                            // game logic starts here
                                                            LgameProper game;
                                                            game.run(fightMusic);

                                                            fightMusic.stop();

                                                            //back to main menu
                                                            LAND.close();
                                                            FIRE.close();
                                                            WATER.close();
                                                            AIR.close();
                                                            PLAY.close();

                                                            // change bgm back
                                                            bgMusic.play();
                                                    }
                                                }
                                                LAND.clear();
                                                land.draw(LAND);
                                                FIRE.close();
                                                WATER.close();
                                                AIR.close();
                                                LAND.display();
                                            }
                                        }
                                    }
                                }
                            }
                            INSTRUCTIONS.close();
                            CREDITS.close();
                            PLAY.clear();
                            PLAY.draw(PBackground);
                            play.draw(PLAY);
                            PLAY.display();
                        }
                    }

                    if (x == 1) {
                        while (INSTRUCTIONS.isOpen()) {
                            Event aevent;
                            while (INSTRUCTIONS.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed || aevent.key.code == Keyboard::Space) {
                                    clickSound.play();
                                    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                    INSTRUCTIONS.close();
                                }
                            }
                            PLAY.close();
                            INSTRUCTIONS.clear();
                            INSTRUCTIONS.draw(IBackground);
                            CREDITS.close();
                            INSTRUCTIONS.display();
                        }
                    }

                    if (x == 2) {
                        while (CREDITS.isOpen()) {
                            Event aevent;
                            while (CREDITS.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed || aevent.key.code == Keyboard::Space) {
                                    clickSound.play();
                                    sf::sleep(sf::milliseconds(500)); //to synchronize sound effect
                                    CREDITS.close();
                                }
                            }
                            PLAY.close();
                            INSTRUCTIONS.close();
                            CREDITS.clear();
                            CREDITS.draw(CBackground);
                            CREDITS.display();
                        }
                    }

                    if (x == 3)
                        MENU.close();
                }
            }
        }

        MENU.clear();
        MENU.draw(background);
        menu.draw(MENU);
        MENU.display();
    }

    return 0;
}