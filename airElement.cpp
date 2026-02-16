#include "airElement.h"
#include <iostream>

airElement::airElement(float width, float height)
{
    if (!font.loadFromFile("select.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Optional: Load a background image
    if (bgTexture.loadFromFile("images/bgs/hangin.jpg")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            width / bgTexture.getSize().x,
            height / bgTexture.getSize().y
        );
    }

    // Setup the "START" button
    startButton.setFont(font);
    startButton.setFillColor(Color(0, 0, 139));
    startButton.setString("START");
    startButton.setCharacterSize(70);
    startButton.setPosition(875, 1035);
}

airElement::~airElement() {}

void airElement::draw(sf::RenderWindow& window) {
    // Draw background if loaded
    if (bgTexture.getSize().x > 0 && bgTexture.getSize().y > 0)
        window.draw(bgSprite);
    window.draw(startButton);
}