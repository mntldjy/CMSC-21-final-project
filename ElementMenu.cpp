#include "ElementMenu.h"

ElementMenu::ElementMenu(float width, float height)
{
	if (!font.loadFromFile("select.ttf"))
	{
		cout << "Error loading font" << endl;
	}

	bgSprite.setTexture(bgTexture);
	bgSprite.setScale(
		width / bgTexture.getSize().x,
		height / bgTexture.getSize().y
	);

	std::string names[ELEMENT_COUNT] = {
		"Fire",
		"Air",
		"Water",
		"Earth"
		//"Lightning"
	};

	elements[0].setFont(font);
	elements[0].setFillColor(Color::White);
	elements[0].setString("APOY");
	elements[0].setCharacterSize(70);
	elements[0].setPosition(160, 850);

	elements[1].setFont(font);
	elements[1].setFillColor(Color::White);
	elements[1].setString("HANGIN");
	elements[1].setCharacterSize(70);
	elements[1].setPosition(620, 850);

	elements[2].setFont(font);
	elements[2].setFillColor(Color::White);
	elements[2].setString("TUBIG");
	elements[2].setCharacterSize(70);
	elements[2].setPosition(1125, 850);

	elements[3].setFont(font);
	elements[3].setFillColor(Color::White);
	elements[3].setString("LUPA");
	elements[3].setCharacterSize(70);
	elements[3].setPosition(1620, 850);

	//elements[4].setFont(font);
	//elements[4].setFillColor(Color::White);
	//elements[4].setString("LIGHTNING");
	//elements[4].setCharacterSize(70);
	//elements[4].setPosition(1600, 80);

	selectedIndex = -1;
}

ElementMenu::~ElementMenu() {}

void ElementMenu::draw(sf::RenderWindow& window) {
	for (int i = 0; i < ELEMENT_COUNT; ++i) {
		window.draw(elements[i]);
	}
}

void ElementMenu::MoveRight() {
	if (selectedIndex + 1 <= ELEMENT_COUNT)
	{
		elements[selectedIndex].setFillColor(Color::White);
		selectedIndex++;

		if (selectedIndex == 4) {
			selectedIndex = 0;
		}
		elements[selectedIndex].setFillColor(Color::Black);
	}
}

void ElementMenu::MoveLeft() {
	if (selectedIndex - 1 >= 0)
	{
		elements[selectedIndex].setFillColor(Color::White);
		selectedIndex--;

		if (selectedIndex == -1) {
			selectedIndex = 2;
		}
		elements[selectedIndex].setFillColor(Color::Black);
	}
}