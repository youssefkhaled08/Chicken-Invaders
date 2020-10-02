#pragma once
#include "MainVariables.h"
#include "LeaderBoard.h"
	float gravity = 2;
	vector<Color> Colors;
struct Particle
{
	float size = rand() % 10 + 1, color = rand()%8, force = -(rand()%100 + 3) , curForce;

	CircleShape body;

	void move()
	{
		body.move(0, curForce);
		curForce += gravity;
	}
};
void Win()
{	
	window.clear();
	window.setFramerateLimit(60);
	Colors.push_back(Color::Blue);
	Colors.push_back(Color::Red);
	Colors.push_back(Color::Green);
	Colors.push_back(Color::Yellow);
	Colors.push_back(Color::Magenta);
	Colors.push_back(Color::Cyan);
	Colors.push_back(Color::White);
	Colors.push_back(Color::Black);

	RectangleShape WinBackground(Vector2f(window.getSize().x, window.getSize().y));
	window.clear();

	Particle Particles[400];
	for (int i = 0 ; i<400;i++)
	{
		Particles[i].body.setFillColor(Colors[Particles[i].color]);
		Particles[i].body.setPosition(rand()%1900, window.getSize().y);
		Particles[i].body.setRadius(Particles[i].size);
		Particles[i].curForce = Particles[i].force;
	}
	Color color(0, 0, 0, 0), textColor(0, 0, 255, 0);
	WinBackground.setFillColor(color);
	Text Winner;
	Winner.setString("Winner Winner Chicken Dinner!!");
	Font WinnerFont;
	WinnerFont.loadFromFile("FunSized.ttf");
	Winner.setFont(WinnerFont);
	Winner.setCharacterSize(40);
	//Winner.setOrigin(Vector2f(Winner.getCharacterSize()/2, Winner.getCharacterSize()/2));
	Winner.setFillColor(textColor);
	Winner.setPosition(600, 500);

	int score=0;
	string name;
	for (int i = 0; i < Players.size(); i++)
	{
		score += Players[i].score;
		name = Players[i].name;
	}
	

	for (int i = 0; i < PlayersForLeaderBoard.size(); i++)
	{
		score += PlayersForLeaderBoard[i].score;
		name = PlayersForLeaderBoard[i].name;
	}

	Add_Score(name, score);

	while (true)
	{
		window.clear();
		if (color.a < 150)
			color.a++;
		else if (textColor.a < 255)
			textColor.a += 3;

		window.draw(WinBackground);
		for (int i = 0; i < 100; i++)
		{
			//cout << "Particle " << i << "  " << Particles[i].body.getPosition().x << "x" << Particles[i].body.getPosition().y << "    " <<Particles[i].color << endl;
			Particles[i].move();
			window.draw(Particles[i].body);
			if (Particles[i].body.getPosition().y > window.getSize().y)
			{
				Particles[i].body.setPosition(Particles[i].body.getPosition().x, window.getSize().y);
				Particles[i].curForce = Particles[i].force;
			}
		}
		WinBackground.setFillColor(color);
		Winner.setFillColor(textColor);
		window.draw(WinBackground);
		window.draw(Winner);

		window.display();

	}
}
void Lose()
{
	Color color(0,0,0, 0),textColor(255,0,0,0);
	RectangleShape WinBackground(Vector2f(window.getSize().x, window.getSize().y));
	Text Winner;
	Winner.setString("Game Over!");
	Font WinnerFont;
	WinnerFont.loadFromFile("FunSized.ttf");
	Winner.setFont(WinnerFont);
	Winner.setCharacterSize(70);
	Clock Timer;
	//Winner.setOrigin(Vector2f(Winner.getCharacterSize()/2, Winner.getCharacterSize()/2));

	int score = 0;
	string name;

	for (int i = 0; i < PlayersForLeaderBoard.size(); i++)
	{
		score += PlayersForLeaderBoard[i].score;
		name = PlayersForLeaderBoard[i].name;
	}

	Add_Score(name, score);

	Winner.setFillColor(textColor);
	Winner.setPosition(720, 500);
	while (true)
	{
		if (color.a < 200)
			color.a++;
		else if (textColor.a < 255)
			textColor.a+=3;

		
		WinBackground.setFillColor(color);
		Winner.setFillColor(textColor);
		window.draw(WinBackground);
		window.draw(Winner);
		window.display();

		if (Timer.getElapsedTime().asSeconds() >= 10)
			return;
	}
}