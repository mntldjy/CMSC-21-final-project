#include "landElement.h"
#include <iostream>

landElement::landElement(float width, float height)
{
    if (!font.loadFromFile("select.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Optional: Load a background image
    if (bgTexture.loadFromFile("images/lupa.jpg")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            width / bgTexture.getSize().x,
            height / bgTexture.getSize().y
        );
    }

    // Setup the "START" button
    startButton.setFont(font);
    startButton.setFillColor(Color(101, 67, 33));
    startButton.setString("START");
    startButton.setCharacterSize(70);
    startButton.setPosition(875, 930);
}

landElement::~landElement() {}

void landElement::draw(sf::RenderWindow& window) {
    // Draw background if loaded
    if (bgTexture.getSize().x > 0 && bgTexture.getSize().y > 0)
        window.draw(bgSprite);
    window.draw(startButton);
}