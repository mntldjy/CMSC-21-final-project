#include "fireElement.h"
#include <iostream>

fireElement::fireElement(float width, float height)
{
    if (!font.loadFromFile("select.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Optional: Load a background image
    if (bgTexture.loadFromFile("images/bgs/apoy.jpg")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            width / bgTexture.getSize().x,
            height / bgTexture.getSize().y
        );
    }

    // Setup the "START" button
    startButton.setFont(font);
    startButton.setFillColor(Color(128, 0, 0)); 
    startButton.setString("START");
    startButton.setCharacterSize(70);
    startButton.setPosition(875, 1035);
}

fireElement::~fireElement() {}

void fireElement::draw(sf::RenderWindow& window) {
    // Draw background if loaded
    if (bgTexture.getSize().x > 0 && bgTexture.getSize().y > 0)
        window.draw(bgSprite);
    window.draw(startButton);
}
