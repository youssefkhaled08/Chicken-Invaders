#pragma once
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "MainVariables.h"
using namespace sf;
using namespace std;


extern RenderWindow window;
struct  Button {

	int btnWidth;
	int btnHeight;
	RectangleShape button;
	Sound ButtonSound;
	bool in , CheckBox = false;
	bool IsMouseOver()
	{
		int mousex = Mouse::getPosition(window).x;
		int mousey = Mouse::getPosition(window).y;
		int btnPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;
		int btnxPosWidth = button.getPosition().x + btnWidth;
		int btnyPosHeight = button.getPosition().y + btnHeight;

		if (mousex < btnxPosWidth && mousex > btnPosX && mousey < btnyPosHeight && mousey > btnPosY)
		{
			if (!in)
			{
				if (SoundVFX)
				ButtonSound.play();
				in = true;
			}

			if (!CheckBox)
				button.setTexture(&Button2);

			return true;
		}
		else
		{
			if(!CheckBox)
				button.setTexture(&Button1);

			in = false;
			return false;

		}
	}
	void menu()
	{
		
	}


};

//bool isMouseOver() {
//	int mouseX = sf::Mouse::getPosition(window).x;
//	int mouseY = sf::Mouse::getPosition(window).y;

//	int btnPosX = button.getPosition().x;
//	int btnPosY = button.getPosition().y;

//	int btnxPosWidth = button.getPosition().x + btnWidth;
//	int btnyPosHeight = button.getPosition().y + btnHeight;

//	if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
//		return true;
//	}


	//button.setSize(buttonSize);
	//button.setFillColor(bgColor);
	//
	//// Get these for later use:
	//btnWidth = buttonSize.x;
	//btnHeight = buttonSize.y;

	//text.setString(btnText);
	//text.setCharacterSize(charSize);
	//text.setFillColor(textColor);



// Pass font by reference:
//void setFont(sf::Font& fonts) {
//	text.setFont(fonts);
//}

//void setBackColor(sf::Color color) {
//	button.setFillColor(color);
//}

//void setTextColor(sf::Color color) {
//	text.setFillColor(color);
//}

//void setPosition(sf::Vector2f point) {
//	button.setPosition(point);

//	// Center text on button:
//	float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
//	float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
//	text.setPosition(xPos, yPos);
//}

//void drawTo() {
//	window.draw(button);
//	window.draw(text);
//}

//// Check if the mouse is within the bounds of the button:
//	return false;
//}

