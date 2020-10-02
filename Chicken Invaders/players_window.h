#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "MainVariables.h"
#include "Enemy.h"
#include "startButton.h"
#include "Level1.h"
#include "Level2.h"
#include "WinOrLose.h"
using namespace sf;
using namespace std;

bool solo()
{

	bool oneplayer = false;
	Text solo;
	Font No_playersfont;
	No_playersfont.loadFromFile("CollegiateBlackFLF.ttf");
	solo.setString("Solo");
	solo.setFont(No_playersfont);
	solo.setCharacterSize(100);
	solo.Bold;
	solo.setPosition(400, 200);
	window.draw(solo);


	Text duo;
	duo.setString("Duo");
	duo.setFont(No_playersfont);
	duo.setCharacterSize(100);
	duo.Bold;
	duo.setPosition(400, 400);
	window.draw(duo);

	Button soloCheckBox, duoCheckBox;
	soloCheckBox.button.setSize(Vector2f(100, 50));
	soloCheckBox.button.setPosition(600, 200);
	soloCheckBox.btnWidth = 100;
	soloCheckBox.btnHeight = 50;
	window.draw(soloCheckBox.button);

	duoCheckBox.button.setSize(Vector2f(100, 50));
	duoCheckBox.button.setPosition(600, 400);
	duoCheckBox.btnWidth = 100;
	duoCheckBox.btnHeight = 50;
	window.draw(duoCheckBox.button);
	window.display();

	Event evnt;
	while (!oneplayer)
	{
		
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::MouseButtonPressed:
				if (soloCheckBox.IsMouseOver())
				{
					oneplayer = true;
					Wave_1_Level_3();
					return oneplayer;

				}
				else if (duoCheckBox.IsMouseOver())
				{

					return 0;
				}
				break;
			}
		}
	}
}