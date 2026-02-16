#include "AgameProper.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <list>
using namespace sf;
using namespace std;

AgameProper::AgameProper() {
}

// --- Main Game Logic ---
void AgameProper::run(Music& fightMusic) {
    srand(time(0));
    VideoMode desktop = VideoMode::getDesktopMode();
    int W = desktop.width;
    int H = desktop.height;
    RenderWindow app(desktop, "Air Element", Style::Fullscreen);
    app.setFramerateLimit(60);

    // Load collision sound
    SoundBuffer collisionBuffer;
    if (!collisionBuffer.loadFromFile("images/music/collision.wav")) {
        std::cerr << "Error loading collision effect\n";
    }
    Sound collisionSound;
    collisionSound.setBuffer(collisionBuffer);

    // Load fire effect sound
    SoundBuffer fireBuffer;
    if (!fireBuffer.loadFromFile("images/music/fire.wav")) {
        std::cerr << "Error loading fire sound effect\n";
    }
    Sound fireSound;
    fireSound.setBuffer(fireBuffer);

    // Load resources
    t1.loadFromFile("images/orb.png");
    t2.loadFromFile("images/airBG.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/meteors/yellow.png");
    t5.loadFromFile("images/bulletblue.png");
    t6.loadFromFile("images/explosions/type_B.png");
    t1.setSmooth(true);
    t2.setSmooth(true);

    background = Sprite(t2);

    sExplosion = Animation(t3, 0, 0, 256, 256, 48, 0.5f);
    sRock = Animation(t4, 0, 0, 56, 69, 16, 0.2f);
    sBullet = Animation(t5, 0, 0, 32, 64, 16, 0.8f);
    sPlayerAnim = Animation(t1, 0, 0, 71, 70, 1, 0);
    sExplosion_ship = Animation(t6, 0, 0, 192, 192, 64, 0.5f);

    font.loadFromFile("select.ttf");
    scoreText = Text("Score: 0", font, 50);
    timerText = Text("Time: 0", font, 50);
    scoreText.setFillColor(Color::Black);
    timerText.setFillColor(Color::Black);
    scoreText.setPosition(20, 30);
    timerText.setPosition(20, 80);

    uiBackground = RectangleShape(Vector2f(250, 120));
    uiBackground.setFillColor(Color(255, 255, 255, 100));
    uiBackground.setPosition(10, 30);

    // Initial entities
    for (int i = 0; i < 10; i++) {
        asteroid* a = new asteroid();
        a->settings(sRock, rand() % W, 0, 0, 25);
        entities.push_back(a);
    }
    p = new player();
    p->settings(sPlayerAnim, W / 2, H - 50, 270, 20);
    entities.push_back(p);

    gameClock.restart();
    score = 0;
    frameCounter = 0;
    hitStreak = 0;
    lastHitTime = -10.0f;

    // --- PAUSE MENU SETUP ---
    bool isPaused = false;
    bool shouldRestart = false;
    RectangleShape pauseOverlay(Vector2f(W, H));
    pauseOverlay.setFillColor(Color(0, 0, 0, 150));

    Font menuFont;
    menuFont.loadFromFile("fred.ttf");

    Text pauseTitle("PAUSED", menuFont, 80);
    pauseTitle.setFillColor(Color::White);
    pauseTitle.setStyle(Text::Bold);
    FloatRect pauseRect = pauseTitle.getLocalBounds();
    pauseTitle.setOrigin(pauseRect.width / 2, pauseRect.height / 2);
    pauseTitle.setPosition(W / 2, H / 2 - 150);

    RectangleShape restartButton(Vector2f(200, 60));
    restartButton.setPosition(W / 2 - 220, H / 2);
    restartButton.setFillColor(Color(50, 50, 50, 200));

    Text restartText("Restart", menuFont, 30);
    restartText.setFillColor(Color::White);
    restartText.setPosition(restartButton.getPosition().x + 50, restartButton.getPosition().y + 15);

    RectangleShape exitButton(Vector2f(200, 60));
    exitButton.setPosition(W / 2 + 20, H / 2);
    exitButton.setFillColor(Color(50, 50, 50, 200));

    Text exitText("Exit", menuFont, 30);
    exitText.setFillColor(Color::White);
    exitText.setPosition(exitButton.getPosition().x + 70, exitButton.getPosition().y + 15);

    // --- Main Loop ---
    while (app.isOpen()) {
        Event event;
        while (app.pollEvent(event)) {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::P) {
                    isPaused = !isPaused;

                    if (isPaused) {
                        fightMusic.pause();  // Pause the fight music
                    }
                    else {
                        fightMusic.play();   // Resume the fight music
                    }
                }

                if (!isPaused && event.key.code == Keyboard::Space) {
                    fireSound.play();
                    bullet* b = new bullet();
                    b->settings(sBullet, p->x, p->y, 270, 10);
                    entities.push_back(b);
                }
            }

            if (isPaused && event.type == Event::MouseButtonPressed) {
                Vector2f mouse = app.mapPixelToCoords(Mouse::getPosition(app));
                if (restartButton.getGlobalBounds().contains(mouse)) {
                    shouldRestart = true;
                    isPaused = false;
                }
                if (exitButton.getGlobalBounds().contains(mouse)) {
                    app.close();
                }
            }
        }

        // If paused, show pause menu and skip update/draw of gameplay
        if (isPaused) {
            app.clear();
            app.draw(background);
            app.draw(pauseOverlay);
            app.draw(pauseTitle);
            app.draw(restartButton);
            app.draw(restartText);
            app.draw(exitButton);
            app.draw(exitText);
            app.display();
            continue;
        }

        p->moveLeft = Keyboard::isKeyPressed(Keyboard::Left);
        p->moveRight = Keyboard::isKeyPressed(Keyboard::Right);
        p->moveUp = Keyboard::isKeyPressed(Keyboard::Up);
        p->moveDown = Keyboard::isKeyPressed(Keyboard::Down);

        for (auto e : entities) e->update();

        entities.remove_if([](Entity* e) { if (!e->life) { delete e; return true; } return false; });

        int timeLeft = timeLimit - gameClock.getElapsedTime().asSeconds();
        if (timeLeft < 0) timeLeft = 0;
        scoreText.setString("Score: " + std::to_string(score));
        timerText.setString("Time: " + std::to_string(timeLeft) + " s");

        int asteroidCount = 0;
        for (auto e : entities)
            if (e->name == "asteroid")
                asteroidCount++;

        frameCounter++;
        if (frameCounter >= 180) {
            if (asteroidCount < 2) {
                int numToSpawn = rand() % 4 + 2;
                for (int i = 0; i < numToSpawn && asteroidCount + i < 5; i++) {
                    asteroid* a = new asteroid();
                    a->settings(sRock, rand() % W, 0, 0, 25);
                    entities.push_back(a);
                }
            }
            frameCounter = 0;
        }

        for (auto a : entities)
            for (auto b : entities) {
                if (a->name == "asteroid" && b->name == "bullet")
                    if (isCollide(a, b)) {

                        // Play collision sound
                        collisionSound.play();

                        a->life = false;
                        b->life = false;
                        float currentTime = gameClock.getElapsedTime().asSeconds();
                        if (currentTime - lastHitTime <= 3.0f) {
                            hitStreak++;
                            score += 20;
                        }
                        else {
                            hitStreak = 1;
                            score += 10;
                        }
                        lastHitTime = currentTime;
                        Entity* e = new Entity();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                    }
                if (a->name == "player" && b->name == "asteroid")
                    if (isCollide(a, b)) {

                        // Play collision sound
                        collisionSound.play();

                        // Penalty
                        score -= 5;
                        if (score < 0) score = 0;

                        b->life = false;
                        Entity* e = new Entity();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);
                        p->settings(sPlayerAnim, W / 2, H - 50, 270, 20);
                        p->dx = 0; p->dy = 0;
                    }
            }

        p->anim = sPlayerAnim;

        for (auto e : entities)
            if (e->name == "explosion")
                if (e->anim.isEnd()) e->life = 0;

        for (auto i = entities.begin(); i != entities.end();) {
            Entity* e = *i;
            e->update();
            e->anim.update();
            if (e->life == false) { i = entities.erase(i); delete e; }
            else i++;
        }

        // End screen logic
        if (gameClock.getElapsedTime().asSeconds() > timeLimit) {
            bool replay = false;
            bool isGameOver = (score < 500);
            string endMessage = isGameOver ? "GAME OVER" : " SUCCESSFULLY\n    DEFENDED\n   THE REALM";

            Font font;
            font.loadFromFile("fred.ttf");

            // Add a dark overlay behind the end text
            RectangleShape endOverlay(Vector2f(W, H));
            endOverlay.setFillColor(Color(0, 0, 0, 150)); // 150 alpha for darkening effect
            app.draw(endOverlay);

            Text endText(endMessage, font, 70);
            endText.setStyle(Text::Bold);
            endText.setFillColor(Color::White); // Always white
            FloatRect textRect = endText.getLocalBounds();
            endText.setOrigin(textRect.width / 2, textRect.height / 2);
            endText.setPosition(W / 2, H / 2 - 100);

            Text scoreEndText("Final Score: " + std::to_string(score), font, 50);
            scoreEndText.setFillColor(Color::White);
            FloatRect scoreRect = scoreEndText.getLocalBounds();
            scoreEndText.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
            scoreEndText.setPosition(W / 2 - 700, H / 2 - 450);

            RectangleShape replayButton, exitButton;
            Text replayText, exitText;

            if (isGameOver) {
                replayButton.setSize(Vector2f(200, 50));
                replayButton.setPosition(W / 2 - 220, H / 2 + 50);
                replayButton.setFillColor(Color(0, 0, 0, 150));
                replayText.setFont(font);
                replayText.setString("Replay");
                replayText.setCharacterSize(24);
                replayText.setFillColor(Color::White);
                replayText.setPosition(replayButton.getPosition().x + 60, replayButton.getPosition().y + 10);

                exitButton.setSize(Vector2f(200, 50));
                exitButton.setPosition(W / 2 + 20, H / 2 + 50);
                exitButton.setFillColor(Color(0, 0, 0, 150));
                exitText.setFont(font);
                exitText.setString("Exit");
                exitText.setCharacterSize(24);
                exitText.setFillColor(Color::White);
                exitText.setPosition(exitButton.getPosition().x + 70, exitButton.getPosition().y + 10);
            }
            else {
                exitButton.setSize(Vector2f(200, 50));
                exitButton.setPosition(W / 2 - 100, H / 2 + 100);
                exitButton.setFillColor(Color(0, 0, 0, 150));
                exitText.setFont(font);
                exitText.setString("Exit");
                exitText.setCharacterSize(24);
                exitText.setFillColor(Color::White);
                exitText.setPosition(exitButton.getPosition().x + 70, exitButton.getPosition().y + 10);
            }

            while (app.isOpen()) {
                Event event;
                while (app.pollEvent(event)) {
                    if (event.type == Event::Closed)
                        app.close();
                    if (event.type == Event::MouseButtonPressed) {
                        Vector2f mouse = app.mapPixelToCoords(Mouse::getPosition(app));
                        if (isGameOver && replayButton.getGlobalBounds().contains(mouse))
                            replay = true;
                        if (exitButton.getGlobalBounds().contains(mouse))
                            app.close();
                    }
                }

                app.clear();
                app.draw(background);
                app.draw(endOverlay);
                app.draw(endText);
                app.draw(scoreEndText);
                if (isGameOver) {
                    app.draw(replayButton);
                    app.draw(replayText);
                }
                app.draw(exitButton);
                app.draw(exitText);
                app.display();

                if (replay) {
                    fightMusic.play();

                    entities.clear();
                    score = 0;
                    gameClock.restart();
                    hitStreak = 0;
                    lastHitTime = -10.0f;
                    for (int i = 0; i < 10; i++) {
                        asteroid* a = new asteroid();
                        a->settings(sRock, rand() % W, 0, 0, 25);
                        entities.push_back(a);
                    }
                    p = new player();
                    p->settings(sPlayerAnim, W / 2, H - 50, 270, 20);
                    entities.push_back(p);
                    break;
                }
            }
        }


        app.clear();
        app.draw(background);
        for (auto i : entities) i->draw(app);
        app.draw(uiBackground);
        app.draw(scoreText);
        app.draw(timerText);
        app.display();

        // Restart game if requested
        if (shouldRestart) {
            fightMusic.play();

            entities.clear();
            score = 0;
            gameClock.restart();
            hitStreak = 0;
            lastHitTime = -10.0f;
            for (int i = 0; i < 10; i++) {
                asteroid* a = new asteroid();
                a->settings(sRock, rand() % W, 0, 0, 25);
                entities.push_back(a);
            }
            p = new player();
            p->settings(sPlayerAnim, W / 2, H - 50, 270, 20);
            entities.push_back(p);
            shouldRestart = false;
        }
    }
}