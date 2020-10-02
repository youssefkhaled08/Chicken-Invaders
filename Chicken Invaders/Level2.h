#pragma once
#include<iomanip>
#include<sstream>
#include "MainVariables.h"
#include "WinOrLose.h"
#include "Level3.h"
extern void Bullet_Move();
extern void Laser_Check();
//extern bool backtomenu = false;
extern bool pause();


bool Wave_2_Level_2();
bool Boss_Level_2();


bool Wave_1_Level_2()
{
	Background.setTexture(&BackgroundLevel2);
	Background.setTextureRect(IntRect(0, 0, BackgroundLevel2.getSize().x, BackgroundLevel2.getSize().y));
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Bonus Wave");
	load.setCharacterSize(60);
	load.setPosition(850, 510);
	load.setFillColor(WaveColor);
	Font font;
	font.loadFromFile("FunSized.ttf");
	load.setFont(font);


	while (WaveColor.a < 255)
	{
		WaveColor.a++;
		load.setFillColor(WaveColor);
		window.draw(Background);
		window.draw(load);
		window.display();
	}

	int waves = 1;


	//Making Sure that all variables are empty in the start of the wave
	Enemies.clear();
	//coinsAndChicken.clear();
	//WeaponGifts.clear();
	bullet.clear();
	window.clear();

	//Setup the background
	window.draw(Background);
	window.draw(load);
	window.display();

	//Making Sure that all variables are empty in the start of the wave
	Enemies.clear();
	coinsAndChicken.clear();
	WeaponGifts.clear();
	bullet.clear();
	window.clear();

	//Setup the background
	window.draw(Background);

	//Enemy Setup
	int dist = 300;
	for (int i = 0; i < 51; i++)
	{
		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		newEnemy.enemyShape.setPosition(dist, 300);
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 2;
		Enemies.push_back(newEnemy);
		dist -= 120;

	}
	bool wavenotfinished = true;


	//Making Player Ready
	for (int i = 0; i < Players.size(); i++)
	{
		Players[i].shieldTime = 3;
		Players[i].ShieldTimer.restart();
		Players[i].Lives += 1;
		Players[i].Activate_Shield();
	}


	while (Players.size() > 0 && Enemies.size() > 0 && window.isOpen() && wavenotfinished)
	{

		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);
		Time FPSTime = FPSClock.getElapsedTime();
		FPSClock.restart();

		Event eveent;
		while (window.pollEvent(eveent))
		{
			switch (eveent.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		}
		//pause 
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{


			if (pause() == 0)
				return 0;

		}

		FPS = 1.0f / FPSTime.asSeconds();
		FPSDiffRatio = 120 / (FPS + 1);

		//Player System
		for (int i = 0; i < Players.size(); i++)
		{
			RectangleShape UIBack;
			UIBack.setSize(Vector2f(450, 90));

			UIBack.setTexture(&UIBackTex);
			UIBack.setFillColor(Color(0, 0, 100));
			//displaying score;
			Font playerscore;
			playerscore.loadFromFile("CollegiateBlackFLF.ttf");
			Text score;
			score.setOrigin(0, 0);
			stringstream stringscore;

			if (i == 0)
			{
				UIBack.setPosition(-3, 10);
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(255, 37);
			}
			else
			{
				UIBack.setPosition(window.getSize().x + 3, 10);
				UIBack.setSize(Vector2f(-450, 90));
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(window.getSize().x - 415, 37);
			}
			score.setFillColor(Color::White);
			score.setFont(playerscore);


			window.draw(score);

			//number of lives
			Texture Heart;
			Heart.loadFromFile("Heart.png");
			RectangleShape heartshape;
			heartshape.setSize(Vector2f(40, 40));
			heartshape.setTexture(&Heart);
			Font lives;
			lives.loadFromFile("CollegiateBlackFLF.ttf");
			stringstream number_of_lives_to_string;
			number_of_lives_to_string << Players[i].Lives;
			Text numberoflives;
			numberoflives.setFont(lives);
			numberoflives.setString(number_of_lives_to_string.str());
			if (i == 0)
			{
				heartshape.setPosition(10, window.getSize().y - 50);
				numberoflives.setPosition(60, window.getSize().y - 46);
			}
			else
			{
				heartshape.setPosition(window.getSize().x - 50, window.getSize().y - 50);
				numberoflives.setPosition(window.getSize().x - 80, window.getSize().y - 46);
			}
			window.draw(heartshape);
			window.draw(numberoflives);

			if (Keyboard::isKeyPressed(Keyboard::J))
			{
				Players[0].score += 10000;
				Players[1].score += 10000;
			}
			if (!Players[i].Exploded)
			{

				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.overHeat > 0)
					{
						Players[i].primaryWeapon.overHeat -= Players[i].primaryWeapon.CalmingRate;
					}
					else
					{
						Players[i].primaryWeapon.overHeat = 0;
					}

					if (!Players[i].primaryWeapon.HeatCalmed)
					{

						Players[i].primaryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarPurble);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(15, 45);
						OverHeatBar.setSize(Vector2f((Players[i].primaryWeapon.overHeat * BarBackground.getSize().x) / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(15, 45);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarCyan);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 15, 45);
						OverHeatBar.setSize(Vector2f(Players[i].primaryWeapon.overHeat * BarBackground.getSize().x / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 15, 45);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);
				}
				else
				{
					if (Players[i].secondryWeapon.overHeat > 0)
					{
						Players[i].secondryWeapon.overHeat -= Players[i].secondryWeapon.CalmingRate;
					}
					else
					{
						Players[i].secondryWeapon.overHeat = 0;
					}

					if (!Players[i].secondryWeapon.HeatCalmed)
					{

						Players[i].secondryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarGreen);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(10, 30);
						OverHeatBar.setSize(Vector2f((Players[i].secondryWeapon.overHeat * BarBackground.getSize().x) / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(10, 30);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarBlue);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 10, 30);
						OverHeatBar.setSize(Vector2f(Players[i].secondryWeapon.overHeat * BarBackground.getSize().x / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 10, 30);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);

					// timer for the secondary weapon
					RectangleShape TimerBar;
					if (Players[i].timer.getElapsedTime().asSeconds() >= 20)
						TimerBar.setTexture(&BarPink);
					else
						TimerBar.setTexture(&BarOrange);

					if (i == 0)
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(220, 20));
						TimerBackground.setPosition(10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(10, 60);
						window.draw(TimerBackground);
					}
					else
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(-220, 20));
						TimerBackground.setPosition(window.getSize().x - 10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(window.getSize().x - 10, 60);
						window.draw(TimerBackground);
					}
					window.draw(TimerBar);

				}
				Players[i].move(window.getSize().x, window.getSize().y);
				if (!Players[i].died)
				{
					Players[i].can_shoot();
				}
				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
					{
						Laser_Check();
						window.draw(Players[i].primaryWeapon.Laser);
					}
					else
					{
						Players[i].primaryWeapon.Laser.setSize(Vector2f(10, 0));
					}
				}
				else
				{
					if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
					{
						Laser_Check();
						
						window.draw(Players[i].secondryWeapon.Laser);
					}
					else
					{
						Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
						Players[i].secondryWeapon.BulletSound.stop();
						Players[i].secondryWeapon.loopsound = false;
					}
				}


				window.draw(Players[i].body);
				if (Players[i].Shield)
				{
					window.draw(Players[i].ShieldBody);
				}
			}

			if (Players[i].Exploded && Players[i].died)
			{
				PlayersForLeaderBoard.push_back(Players[i]);
				Players.erase(Players.begin() + i);
			}

		}

		//Cheat
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			for (int i = 0; i < Enemies.size(); i++)
			{
				Enemies[i].take_Damage(100000);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num1))
		{
			Players[0].Change_Weapon(PrimaryWeapons[0]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num2))
		{
			Players[0].Change_Weapon(PrimaryWeapons[1]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num3))
		{
			Players[0].Change_Weapon(PrimaryWeapons[2]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num4))
		{
			Players[0].Change_Weapon(PrimaryWeapons[3]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num5))
		{
			Players[0].Change_Weapon(SecondryWeapons[0],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num6))
		{
			Players[0].Change_Weapon(SecondryWeapons[1],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::L))
		{
			Players[0].primaryWeapon.Level += 1;
			Players[0].secondryWeapon.Level += 1;
		}
		//Eggs System
		for (int i = 0; i < 3; i++)
		{
			long long randchicken = rand();
			if (randchicken < Enemies.size())
			{
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x, Enemies[randchicken].enemyShape.getPosition().y);
				EnemyTimer.restart();
			}
		}


		//Score and gifts System
		for (int i = 0; i < coinsAndChicken.size(); i++)
		{
			if (coinsAndChicken[i].body.getPosition().y <= window.getSize().y)
			{
				coinsAndChicken[i].body.move(Vector2f(0.f, 2.5f * FPSDiffRatio));
				if (coinsAndChicken[i].coin)
				{
					coinsAndChicken[i].body.setTexture(&CoinTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				else
				{
					coinsAndChicken[i].body.setTexture(&ChickTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				window.draw(coinsAndChicken[i].body);
				for (int x = 0; x < Players.size(); x++)
				{
					if (coinsAndChicken[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
					{
						Players[x].score += coinsAndChicken[i].score;
						coinsAndChicken.erase(coinsAndChicken.begin() + i);
						i++;
						break;
					}
				}
			}
			else
			{
				coinsAndChicken.erase(coinsAndChicken.begin() + i);
				i--;
			}

		}
		for (int i = 0; i < WeaponGifts.size(); i++)
		{
			window.draw(WeaponGifts[i].body);
			if (WeaponGifts[i].levelUp)
				WeaponGifts[i].body.setTexture(&UpgradeTex);
			WeaponGifts[i].body.move(0.0f, 1.0f * FPSDiffRatio);
			WeaponGifts[i].body.rotate(1 * FPSDiffRatio);
			for (int x = 0; x < Players.size(); x++)
			{
				if (WeaponGifts[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
				{

					if (WeaponGifts[i].levelUp)
					{
						Players[x].Improve_Weapon();
					}
					else
					{
						if (WeaponGifts[i].SecondryOrPrimary <= 50)
							Players[x].Change_Weapon(WeaponGifts[i].SecondryGiftWeapon, true);
						else
							Players[x].Change_Weapon(WeaponGifts[i].PrimaryGiftWeapon, false);
					}
					WeaponGifts.erase(WeaponGifts.begin() + i);
					i--;
					break;
				}
			}
		}
		//Enemy and Bullets System
		for (int i = 0; i < Enemies.size(); i++)
		{
			//Animator
			if (Enemies[i].AnimationTimer.getElapsedTime().asSeconds() >= 0.15f && !Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTextureRect(IntRect(Enemies[i].xCounter * Enemies[i].enemyShape.getTexture()->getSize().x / 3, 0, Enemies[i].enemyShape.getTexture()->getSize().x / 3, Enemies[i].enemyShape.getTexture()->getSize().y));
				if (!Enemies[i].minus)
				{
					Enemies[i].xCounter++;
					if (Enemies[i].xCounter >= 3)
					{
						Enemies[i].xCounter -= 2;
						Enemies[i].minus = true;
					}
				}
				else
				{
					Enemies[i].xCounter--;
					if (Enemies[i].xCounter < 0)
					{
						Enemies[i].xCounter += 2;
						Enemies[i].minus = false;
					}
				}
				Enemies[i].AnimationTimer.restart();
			}


			if (Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTexture(&Enemies[i].enemyExplosion);
				Enemies[i].animation();
			}

			//if (highSpeed)
			//	Enemies[i].speed = 20;
			//else
			//	Enemies[i].speed = 1;

			//Moveeeeee
			if (Enemies[i].wave1level2test == false)
			{
				Enemies[i].enemyShape.move(Enemies[i].speed, 0);
			}
			if (Enemies[i].enemyShape.getPosition().x == 300)
				Enemies[i].wave1level2test = true;
			if (i % 5 == 0 && Enemies[i].wave1level2test == true)
			{

				Enemies[i].enemyShape.move(Enemies[i].speed * cos(50), Enemies[i].speed * sin(50));
			}
			if (i % 5 == 1 && Enemies[i].wave1level2test == true)
			{

				Enemies[i].enemyShape.move(Enemies[i].speed * cos(45), Enemies[i].speed * sin(45));
			}
			if (i % 5 == 2 && Enemies[i].wave1level2test == true)
			{

				Enemies[i].enemyShape.move(Enemies[i].speed * cos(0), Enemies[i].speed * sin(0));
			}
			if (i % 5 == 3 && Enemies[i].wave1level2test == true)
			{

				Enemies[i].enemyShape.move(Enemies[i].speed * cos(-45), Enemies[i].speed * sin(-45));
			}
			if (i % 5 == 4 && Enemies[i].wave1level2test == true)
			{

				Enemies[i].enemyShape.move(Enemies[i].speed * cos(-50), Enemies[i].speed * sin(-50));
			}
			if ((Enemies.back().enemyShape.getPosition().x > window.getSize().x || Enemies.back().enemyShape.getPosition().y > window.getSize().y) &&
				(Enemies[Enemies.size() - 2].enemyShape.getPosition().x > window.getSize().x || Enemies[Enemies.size() - 2].enemyShape.getPosition().y > window.getSize().y) &&
				(Enemies[Enemies.size() - 3].enemyShape.getPosition().x > window.getSize().x || Enemies[Enemies.size() - 3].enemyShape.getPosition().y > window.getSize().y) &&
				(Enemies[Enemies.size() - 4].enemyShape.getPosition().x > window.getSize().x || Enemies[Enemies.size() - 4].enemyShape.getPosition().y > window.getSize().y) &&
				(Enemies[Enemies.size() - 5].enemyShape.getPosition().x > window.getSize().x || Enemies[Enemies.size() - 5].enemyShape.getPosition().y > window.getSize().y))
			{

				wavenotfinished = false;

			}







			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{
					if (Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died)
					{
						Players[x].die();

						Enemies[i].take_Damage(100000);
						break;
					}
				}
			}
			for (int j = 0; j < bullet.size(); j++)
			{
				if (bullet[j].direction == -1 && bullet[j].shape.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					Enemies[i].take_Damage(bullet[j].damage);
					bullet.erase(bullet.begin() + j);
					j--;
					continue;
				}
				for (int x = 0; x < Players.size(); x++)
				{
					if (bullet[j].shape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && bullet[j].direction == 1)
					{
						Players[x].die();

						bullet.erase(bullet.begin() + j);
						j--;
						break;
					}
				}
			}

			if (!Enemies[i].Died)
				window.draw(Enemies[i].enemyShape);
			else
				Enemies.erase(Enemies.begin() + i);

		}

		Bullet_Move();


		window.display();
	}
	if (wavenotfinished == false)
		Enemies.clear();

	if (Players.size() > 0)
	{
		Wave_2_Level_2();
	}
	else
	{
		Lose();
	}
	return 0;
}
bool Wave_2_Level_2()
{
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Wave 5");
	load.setCharacterSize(60);
	load.setPosition(850, 510);
	load.setFillColor(WaveColor);
	Font font;
	font.loadFromFile("FunSized.ttf");
	load.setFont(font);


	while (WaveColor.a < 255)
	{
		WaveColor.a++;
		load.setFillColor(WaveColor);
		window.draw(Background);
		window.draw(load);
		window.display();
	}

	int waves = 1;


	//Making Sure that all variables are empty in the start of the wave
	Enemies.clear();
	//coinsAndChicken.clear();
	//WeaponGifts.clear();
	bullet.clear();
	window.clear();

	//Setup the background
	window.draw(Background);
	window.draw(load);
	window.display();
	//Enemy Setup
	int y1 = 700, y2 = 600, y3 = 500, y4 = 600, y5 = 700;
	for (int i = 0; i < 15; i++)
	{
		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		if (i % 5 == 0)
		{
			newEnemy.enemyShape.setPosition(500, -100);
			newEnemy.lowery = y1;
			y1 -= 200;
			newEnemy.highery = y1;

		}
		if (i % 5 == 1)
		{
			newEnemy.enemyShape.setPosition(700, -100);
			newEnemy.lowery = y2;
			y2 -= 200;
			newEnemy.highery = y2;
		}
		if (i % 5 == 2)
		{
			newEnemy.enemyShape.setPosition(900, -100);
			newEnemy.lowery = y3;
			y3 -= 200;
			newEnemy.highery = y3;
		}
		if (i % 5 == 3)
		{
			newEnemy.enemyShape.setPosition(1100, -100);
			newEnemy.lowery = y4;
			y4 -= 200;
			newEnemy.highery = y4;
		}
		if (i % 5 == 4)
		{
			newEnemy.enemyShape.setPosition(1300, -100);
			newEnemy.lowery = y5;
			y5 -= 200;
			newEnemy.highery = y5;
		}
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 2;

		Enemies.push_back(newEnemy);


	}






	//Change Direction Points (1720,100) Down , (1720 ,220) Left , (250, 220) Down , (250, 340) Right , (1720,340) Down
	// (1720, 460) Left , (250,460) 

	while (Players.size() > 0 && Enemies.size() > 0 && window.isOpen())
	{

		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);
		Time FPSTime = FPSClock.getElapsedTime();
		FPSClock.restart();

		Event eveent;
		while (window.pollEvent(eveent))
		{
			switch (eveent.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		}
		//pause 
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{


		

			if (pause() == 0)
				return 0;

		}

		FPS = 1.0f / FPSTime.asSeconds();
		FPSDiffRatio = 120 / (FPS + 1);
		//Player System
		for (int i = 0; i < Players.size(); i++)
		{
			RectangleShape UIBack;
			UIBack.setSize(Vector2f(450, 90));

			UIBack.setTexture(&UIBackTex);
			UIBack.setFillColor(Color(0, 0, 100));
			//displaying score;
			Font playerscore;
			playerscore.loadFromFile("CollegiateBlackFLF.ttf");
			Text score;
			score.setOrigin(0, 0);
			stringstream stringscore;

			if (i == 0)
			{
				UIBack.setPosition(-3, 10);
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(255, 37);
			}
			else
			{
				UIBack.setPosition(window.getSize().x + 3, 10);
				UIBack.setSize(Vector2f(-450, 90));
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(window.getSize().x - 415, 37);
			}
			score.setFillColor(Color::White);
			score.setFont(playerscore);


			window.draw(score);

			//number of lives
			Texture Heart;
			Heart.loadFromFile("Heart.png");
			RectangleShape heartshape;
			heartshape.setSize(Vector2f(40, 40));
			heartshape.setTexture(&Heart);
			Font lives;
			lives.loadFromFile("CollegiateBlackFLF.ttf");
			stringstream number_of_lives_to_string;
			number_of_lives_to_string << Players[i].Lives;
			Text numberoflives;
			numberoflives.setFont(lives);
			numberoflives.setString(number_of_lives_to_string.str());
			if (i == 0)
			{
				heartshape.setPosition(10, window.getSize().y - 50);
				numberoflives.setPosition(60, window.getSize().y - 46);
			}
			else
			{
				heartshape.setPosition(window.getSize().x - 50, window.getSize().y - 50);
				numberoflives.setPosition(window.getSize().x - 80, window.getSize().y - 46);
			}
			window.draw(heartshape);
			window.draw(numberoflives);

			if (Keyboard::isKeyPressed(Keyboard::J))
			{
				Players[0].score += 10000;
				Players[1].score += 10000;
			}
			if (!Players[i].Exploded)
			{

				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.overHeat > 0)
					{
						Players[i].primaryWeapon.overHeat -= Players[i].primaryWeapon.CalmingRate;
					}
					else
					{
						Players[i].primaryWeapon.overHeat = 0;
					}

					if (!Players[i].primaryWeapon.HeatCalmed)
					{

						Players[i].primaryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarPurble);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(15, 45);
						OverHeatBar.setSize(Vector2f((Players[i].primaryWeapon.overHeat * BarBackground.getSize().x) / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(15, 45);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarCyan);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 15, 45);
						OverHeatBar.setSize(Vector2f(Players[i].primaryWeapon.overHeat * BarBackground.getSize().x / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 15, 45);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);
				}
				else
				{
					if (Players[i].secondryWeapon.overHeat > 0)
					{
						Players[i].secondryWeapon.overHeat -= Players[i].secondryWeapon.CalmingRate;
					}
					else
					{
						Players[i].secondryWeapon.overHeat = 0;
					}

					if (!Players[i].secondryWeapon.HeatCalmed)
					{

						Players[i].secondryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarGreen);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(10, 30);
						OverHeatBar.setSize(Vector2f((Players[i].secondryWeapon.overHeat * BarBackground.getSize().x) / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(10, 30);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarBlue);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 10, 30);
						OverHeatBar.setSize(Vector2f(Players[i].secondryWeapon.overHeat * BarBackground.getSize().x / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 10, 30);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);

					// timer for the secondary weapon
					RectangleShape TimerBar;
					if (Players[i].timer.getElapsedTime().asSeconds() >= 20)
						TimerBar.setTexture(&BarPink);
					else
						TimerBar.setTexture(&BarOrange);

					if (i == 0)
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(220, 20));
						TimerBackground.setPosition(10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(10, 60);
						window.draw(TimerBackground);
					}
					else
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(-220, 20));
						TimerBackground.setPosition(window.getSize().x - 10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(window.getSize().x - 10, 60);
						window.draw(TimerBackground);
					}
					window.draw(TimerBar);

				}
				Players[i].move(window.getSize().x, window.getSize().y);
				if (!Players[i].died)
				{
					Players[i].can_shoot();
				}
				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
					{
						Laser_Check();
						window.draw(Players[i].primaryWeapon.Laser);
					}
					else
					{
						Players[i].primaryWeapon.Laser.setSize(Vector2f(10, 0));
					}
				}
				else
				{
					if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
					{
						Laser_Check();

						window.draw(Players[i].secondryWeapon.Laser);
					}
					else
					{
						Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
						Players[i].secondryWeapon.BulletSound.stop();
						Players[i].secondryWeapon.loopsound = false;
					}
				}


				window.draw(Players[i].body);
				if (Players[i].Shield)
				{
					window.draw(Players[i].ShieldBody);
				}
			}

			if (Players[i].Exploded && Players[i].died)
			{
				PlayersForLeaderBoard.push_back(Players[i]);
				Players.erase(Players.begin() + i);
			}

		}

		//Cheat
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			for (int i = 0; i < Enemies.size(); i++)
			{
				Enemies[i].take_Damage(100000);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num1))
		{
			Players[0].Change_Weapon(PrimaryWeapons[0]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num2))
		{
			Players[0].Change_Weapon(PrimaryWeapons[1]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num3))
		{
			Players[0].Change_Weapon(PrimaryWeapons[2]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num4))
		{
			Players[0].Change_Weapon(PrimaryWeapons[3]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num5))
		{
			Players[0].Change_Weapon(SecondryWeapons[0],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num6))
		{
			Players[0].Change_Weapon(SecondryWeapons[1],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::L))
		{
			Players[0].primaryWeapon.Level += 1;
			Players[0].secondryWeapon.Level += 1;
		}
		//Eggs System
		for (int i = 0; i < 3; i++)
		{
			long long randchicken = rand();
			if (randchicken < Enemies.size())
			{
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x, Enemies[randchicken].enemyShape.getPosition().y);
				EnemyTimer.restart();
			}
		}


		//Score and gifts System
		for (int i = 0; i < coinsAndChicken.size(); i++)
		{
			if (coinsAndChicken[i].body.getPosition().y <= window.getSize().y)
			{
				coinsAndChicken[i].body.move(Vector2f(0.f, 2.5f * FPSDiffRatio));
				if (coinsAndChicken[i].coin)
				{
					coinsAndChicken[i].body.setTexture(&CoinTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				else
				{
					coinsAndChicken[i].body.setTexture(&ChickTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				window.draw(coinsAndChicken[i].body);
				for (int x = 0; x < Players.size(); x++)
				{
					if (coinsAndChicken[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
					{
						Players[x].score += coinsAndChicken[i].score;
						coinsAndChicken.erase(coinsAndChicken.begin() + i);
						i++;
						break;
					}
				}
			}
			else
			{
				coinsAndChicken.erase(coinsAndChicken.begin() + i);
				i--;
			}

		}
		for (int i = 0; i < WeaponGifts.size(); i++)
		{
			window.draw(WeaponGifts[i].body);
			if (WeaponGifts[i].levelUp)
				WeaponGifts[i].body.setTexture(&UpgradeTex);
			WeaponGifts[i].body.move(0.0f, 1.0f * FPSDiffRatio);
			WeaponGifts[i].body.rotate(1 * FPSDiffRatio);
			for (int x = 0; x < Players.size(); x++)
			{
				if (WeaponGifts[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
				{

					if (WeaponGifts[i].levelUp)
					{
						Players[x].Improve_Weapon();
					}
					else
					{
						if (WeaponGifts[i].SecondryOrPrimary <= 50)
							Players[x].Change_Weapon(WeaponGifts[i].SecondryGiftWeapon, true);
						else
							Players[x].Change_Weapon(WeaponGifts[i].PrimaryGiftWeapon, false);
					}
					WeaponGifts.erase(WeaponGifts.begin() + i);
					i--;
					break;
				}
			}
		}
		//Enemy and Bullets System

		for (int i = 0; i < Enemies.size(); i++)
		{
			//Animator
			if (Enemies[i].AnimationTimer.getElapsedTime().asSeconds() >= 0.15f && !Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTextureRect(IntRect(Enemies[i].xCounter * Enemies[i].enemyShape.getTexture()->getSize().x / 3, 0, Enemies[i].enemyShape.getTexture()->getSize().x / 3, Enemies[i].enemyShape.getTexture()->getSize().y));
				if (!Enemies[i].minus)
				{
					Enemies[i].xCounter++;
					if (Enemies[i].xCounter >= 3)
					{
						Enemies[i].xCounter -= 2;
						Enemies[i].minus = true;
					}
				}
				else
				{
					Enemies[i].xCounter--;
					if (Enemies[i].xCounter < 0)
					{
						Enemies[i].xCounter += 2;
						Enemies[i].minus = false;
					}
				}
				Enemies[i].AnimationTimer.restart();
			}


			if (Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTexture(&Enemies[i].enemyExplosion);
				Enemies[i].animation();
			}



			//Moveeeeee the fucking enemy!!!!!!!!!

			if (Enemies[i].wave2level2test == true)
			{

				Enemies[i].enemyShape.move(0, Enemies[i].speed);
			}
			if (Enemies[i].enemyShape.getPosition().y == Enemies[i].lowery)
			{
				Enemies[i].wave2level2test = false;
			}
			if (Enemies[i].wave2level2test == false)
			{
				Enemies[i].enemyShape.move(0, Enemies[i].speed * -1);
			}
			if (Enemies[i].enemyShape.getPosition().y == Enemies[i].highery)
			{
				Enemies[i].wave2level2test = true;
			}





			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{
					if (Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died)
					{
						Players[x].die();

						Enemies[i].take_Damage(100000);
						break;
					}
				}
			}
			for (int j = 0; j < bullet.size(); j++)
			{
				if (bullet[j].direction == -1 && bullet[j].shape.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					Enemies[i].take_Damage(bullet[j].damage);
					bullet.erase(bullet.begin() + j);
					j--;
					continue;
				}
				for (int x = 0; x < Players.size(); x++)
				{
					if (bullet[j].shape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && bullet[j].direction == 1)
					{
						Players[x].die();

						bullet.erase(bullet.begin() + j);
						j--;
						break;
					}
				}
			}

			if (!Enemies[i].Died)
				window.draw(Enemies[i].enemyShape);
			else
				Enemies.erase(Enemies.begin() + i);

		}


		Bullet_Move();


		window.display();
	}
	y1 = 700; y2 = 600; y3 = 500; y4 = 600; y5 = 700;
	for (int i = 0; i < 15; i++)
	{
		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		if (i % 5 == 0)
		{
			newEnemy.enemyShape.setPosition(500, -100);
			newEnemy.lowery = y1;
			y1 -= 200;
			newEnemy.highery = y1;

		}
		if (i % 5 == 1)
		{
			newEnemy.enemyShape.setPosition(700, -100);
			newEnemy.lowery = y2;
			y2 -= 200;
			newEnemy.highery = y2;
		}
		if (i % 5 == 2)
		{
			newEnemy.enemyShape.setPosition(900, -100);
			newEnemy.lowery = y3;
			y3 -= 200;
			newEnemy.highery = y3;
		}
		if (i % 5 == 3)
		{
			newEnemy.enemyShape.setPosition(1100, -100);
			newEnemy.lowery = y4;
			y4 -= 200;
			newEnemy.highery = y4;
		}
		if (i % 5 == 4)
		{
			newEnemy.enemyShape.setPosition(1300, -100);
			newEnemy.lowery = y5;
			y5 -= 200;
			newEnemy.highery = y5;
		}
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 2;

		Enemies.push_back(newEnemy);


	}
	while (Players.size() > 0 && Enemies.size() > 0 && window.isOpen())
	{

		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);
		Time FPSTime = FPSClock.getElapsedTime();
		FPSClock.restart();

		Event eveent;
		while (window.pollEvent(eveent))
		{
			switch (eveent.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		}
		//pause
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{


			

			if (pause() == 0)
				return 0;

		}

		FPS = 1.0f / FPSTime.asSeconds();
		FPSDiffRatio = 120 / (FPS + 1);
		//Player System
		for (int i = 0; i < Players.size(); i++)
		{
			RectangleShape UIBack;
			UIBack.setSize(Vector2f(450, 90));

			UIBack.setTexture(&UIBackTex);
			UIBack.setFillColor(Color(0, 0, 100));
			//displaying score;
			Font playerscore;
			playerscore.loadFromFile("CollegiateBlackFLF.ttf");
			Text score;
			score.setOrigin(0, 0);
			stringstream stringscore;

			if (i == 0)
			{
				UIBack.setPosition(-3, 10);
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(255, 37);
			}
			else
			{
				UIBack.setPosition(window.getSize().x + 3, 10);
				UIBack.setSize(Vector2f(-450, 90));
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(window.getSize().x - 415, 37);
			}
			score.setFillColor(Color::White);
			score.setFont(playerscore);


			window.draw(score);

			//number of lives
			Texture Heart;
			Heart.loadFromFile("Heart.png");
			RectangleShape heartshape;
			heartshape.setSize(Vector2f(40, 40));
			heartshape.setTexture(&Heart);
			Font lives;
			lives.loadFromFile("CollegiateBlackFLF.ttf");
			stringstream number_of_lives_to_string;
			number_of_lives_to_string << Players[i].Lives;
			Text numberoflives;
			numberoflives.setFont(lives);
			numberoflives.setString(number_of_lives_to_string.str());
			if (i == 0)
			{
				heartshape.setPosition(10, window.getSize().y - 50);
				numberoflives.setPosition(60, window.getSize().y - 46);
			}
			else
			{
				heartshape.setPosition(window.getSize().x - 50, window.getSize().y - 50);
				numberoflives.setPosition(window.getSize().x - 80, window.getSize().y - 46);
			}
			window.draw(heartshape);
			window.draw(numberoflives);


			if (!Players[i].Exploded)
			{

				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.overHeat > 0)
					{
						Players[i].primaryWeapon.overHeat -= Players[i].primaryWeapon.CalmingRate;
					}
					else
					{
						Players[i].primaryWeapon.overHeat = 0;
					}

					if (!Players[i].primaryWeapon.HeatCalmed)
					{

						Players[i].primaryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarPurble);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(15, 45);
						OverHeatBar.setSize(Vector2f((Players[i].primaryWeapon.overHeat * BarBackground.getSize().x) / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(15, 45);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarCyan);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 15, 45);
						OverHeatBar.setSize(Vector2f(Players[i].primaryWeapon.overHeat * BarBackground.getSize().x / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 15, 45);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);
				}
				else
				{
					if (Players[i].secondryWeapon.overHeat > 0)
					{
						Players[i].secondryWeapon.overHeat -= Players[i].secondryWeapon.CalmingRate;
					}
					else
					{
						Players[i].secondryWeapon.overHeat = 0;
					}

					if (!Players[i].secondryWeapon.HeatCalmed)
					{

						Players[i].secondryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarGreen);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(10, 30);
						OverHeatBar.setSize(Vector2f((Players[i].secondryWeapon.overHeat * BarBackground.getSize().x) / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(10, 30);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarBlue);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 10, 30);
						OverHeatBar.setSize(Vector2f(Players[i].secondryWeapon.overHeat * BarBackground.getSize().x / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 10, 30);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);

					// timer for the secondary weapon
					RectangleShape TimerBar;
					if (Players[i].timer.getElapsedTime().asSeconds() >= 20)
						TimerBar.setTexture(&BarPink);
					else
						TimerBar.setTexture(&BarOrange);

					if (i == 0)
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(220, 20));
						TimerBackground.setPosition(10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(10, 60);
						window.draw(TimerBackground);
					}
					else
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(-220, 20));
						TimerBackground.setPosition(window.getSize().x - 10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(window.getSize().x - 10, 60);
						window.draw(TimerBackground);
					}
					window.draw(TimerBar);

				}
				Players[i].move(window.getSize().x, window.getSize().y);
				if (!Players[i].died)
				{
					Players[i].can_shoot();
				}
				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
					{
						Laser_Check();
						window.draw(Players[i].primaryWeapon.Laser);
					}
					else
					{
						Players[i].primaryWeapon.Laser.setSize(Vector2f(10, 0));
					}
				}
				else
				{
					if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
					{
						Laser_Check();
						
						window.draw(Players[i].secondryWeapon.Laser);
					}
					else
					{
						Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
						Players[i].secondryWeapon.BulletSound.stop();
						Players[i].secondryWeapon.loopsound = false;
					}
				}


				window.draw(Players[i].body);
				if (Players[i].Shield)
				{
					window.draw(Players[i].ShieldBody);
				}
			}

			if (Players[i].Exploded && Players[i].died)
			{
				PlayersForLeaderBoard.push_back(Players[i]);
				Players.erase(Players.begin() + i);
			}

		}

		//Eggs System
		for (int i = 0; i < 3; i++)
		{
			long long randchicken = rand();
			if (randchicken < Enemies.size())
			{
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x, Enemies[randchicken].enemyShape.getPosition().y);
				EnemyTimer.restart();
			}
		}


		//Score and gifts System
		for (int i = 0; i < coinsAndChicken.size(); i++)
		{
			if (coinsAndChicken[i].body.getPosition().y <= window.getSize().y)
			{
				coinsAndChicken[i].body.move(Vector2f(0.f, 2.5f * FPSDiffRatio));
				if (coinsAndChicken[i].coin)
				{
					coinsAndChicken[i].body.setTexture(&CoinTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				else
				{
					coinsAndChicken[i].body.setTexture(&ChickTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				window.draw(coinsAndChicken[i].body);
				for (int x = 0; x < Players.size(); x++)
				{
					if (coinsAndChicken[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
					{
						Players[x].score += coinsAndChicken[i].score;
						coinsAndChicken.erase(coinsAndChicken.begin() + i);
						i++;
						break;
					}
				}
			}
			else
			{
				coinsAndChicken.erase(coinsAndChicken.begin() + i);
				i--;
			}

		}
		for (int i = 0; i < WeaponGifts.size(); i++)
		{
			window.draw(WeaponGifts[i].body);
			if (WeaponGifts[i].levelUp)
				WeaponGifts[i].body.setTexture(&UpgradeTex);
			WeaponGifts[i].body.move(0.0f, 1.0f * FPSDiffRatio);
			WeaponGifts[i].body.rotate(1 * FPSDiffRatio);
			for (int x = 0; x < Players.size(); x++)
			{
				if (WeaponGifts[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
				{

					if (WeaponGifts[i].levelUp)
					{
						Players[x].Improve_Weapon();
					}
					else
					{
						if (WeaponGifts[i].SecondryOrPrimary <= 50)
							Players[x].Change_Weapon(WeaponGifts[i].SecondryGiftWeapon, true);
						else
							Players[x].Change_Weapon(WeaponGifts[i].PrimaryGiftWeapon, false);
					}
					WeaponGifts.erase(WeaponGifts.begin() + i);
					i--;
					break;
				}
			}
		}
		//Enemy and Bullets System

		for (int i = 0; i < Enemies.size(); i++)
		{
			//Animator
			if (Enemies[i].AnimationTimer.getElapsedTime().asSeconds() >= 0.15f && !Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTextureRect(IntRect(Enemies[i].xCounter * Enemies[i].enemyShape.getTexture()->getSize().x / 3, 0, Enemies[i].enemyShape.getTexture()->getSize().x / 3, Enemies[i].enemyShape.getTexture()->getSize().y));
				if (!Enemies[i].minus)
				{
					Enemies[i].xCounter++;
					if (Enemies[i].xCounter >= 3)
					{
						Enemies[i].xCounter -= 2;
						Enemies[i].minus = true;
					}
				}
				else
				{
					Enemies[i].xCounter--;
					if (Enemies[i].xCounter < 0)
					{
						Enemies[i].xCounter += 2;
						Enemies[i].minus = false;
					}
				}
				Enemies[i].AnimationTimer.restart();
			}


			if (Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTexture(&Enemies[i].enemyExplosion);
				Enemies[i].animation();
			}



			//Moveeeeee the fucking enemy!!!!!!!!!

			if (Enemies[i].wave2level2test == true)
			{

				Enemies[i].enemyShape.move(0, Enemies[i].speed);
			}
			if (Enemies[i].enemyShape.getPosition().y == Enemies[i].lowery)
			{
				Enemies[i].wave2level2test = false;
			}
			if (Enemies[i].wave2level2test == false)
			{
				Enemies[i].enemyShape.move(0, Enemies[i].speed * -1);
			}
			if (Enemies[i].enemyShape.getPosition().y == Enemies[i].highery)
			{
				Enemies[i].wave2level2test = true;
			}





			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{
					if (Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died)
					{
						Players[x].die();

						Enemies[i].take_Damage(100000);
						break;
					}
				}
			}
			for (int j = 0; j < bullet.size(); j++)
			{
				if (bullet[j].direction == -1 && bullet[j].shape.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					Enemies[i].take_Damage(bullet[j].damage);
					bullet.erase(bullet.begin() + j);
					j--;
					continue;
				}
				for (int x = 0; x < Players.size(); x++)
				{
					if (bullet[j].shape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && bullet[j].direction == 1)
					{
						Players[x].die();

						bullet.erase(bullet.begin() + j);
						j--;
						break;
					}
				}
			}

			if (!Enemies[i].Died)
				window.draw(Enemies[i].enemyShape);
			else
				Enemies.erase(Enemies.begin() + i);

		}


		Bullet_Move();

		window.display();
	}
	if (Players.size() > 0)
		Boss_Level_2();
	else
		Lose();
	return 0;
}
bool Boss_Level_2()
{
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Boss II Wave!!");
	load.setCharacterSize(60);
	load.setPosition(850, 510);
	load.setFillColor(WaveColor);
	Font font;
	font.loadFromFile("FunSized.ttf");
	load.setFont(font);


	while (WaveColor.a < 255)
	{
		WaveColor.a++;
		load.setFillColor(WaveColor);
		window.draw(Background);
		window.draw(load);
		window.display();
	}

	int waves = 1;


	//Making Sure that all variables are empty in the start of the wave
	Enemies.clear();
	//coinsAndChicken.clear();
	//WeaponGifts.clear();
	bullet.clear();
	window.clear();

	//Setup the background
	window.draw(Background);
	window.draw(load);
	window.display();

	//Setup the background
	window.draw(Background);
	enemyStructure Boss1;
	Boss1.healthPoints = 10000;
	Boss1.enemyShape.setSize(Vector2f(400, 400));
	Boss1.Boss = true;
	Boss1.TopLeft = true;
	Boss1.speed = 5;
	Boss1.enemyShape.setPosition(100, 20);
	Boss1.enemyShape.setTexture(&ChickenTex);
	Boss1.currectPosition = 100;
	Boss1.enemyExplosion = EnemyExplosion;
	Boss1.eggTex = EggTex;
	int bossRangex = 1300, bossRangey = 400;

	Enemies.push_back(Boss1);



	Clock EnemyMovementTimer;
	bool Entered = false;



	//Making Player Ready
	for (int i = 0; i < Players.size(); i++)
	{
		Players[i].shieldTime = 3;
		Players[i].ShieldTimer.restart();
		Players[i].Lives += 1;
		Players[i].Activate_Shield();
	}


	while (Players.size() > 0 && Enemies.size() > 0 && window.isOpen())
	{

		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);
		Time FPSTime = FPSClock.getElapsedTime();
		FPSClock.restart();

		Event eveent;
		while (window.pollEvent(eveent))
		{
			switch (eveent.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}

		}
		//pause
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{


			if (pause() == 0)
				return 0;

		}

		FPS = 1.0f / FPSTime.asSeconds();
		FPSDiffRatio = 120 / (FPS + 1);


		//Enemy and Bullets System
		for (int i = 0; i < Enemies.size(); i++)
		{
			//Animator
			if (Enemies[i].AnimationTimer.getElapsedTime().asSeconds() >= 0.15f && !Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTextureRect(IntRect(Enemies[i].xCounter * Enemies[i].enemyShape.getTexture()->getSize().x / 3, 0, Enemies[i].enemyShape.getTexture()->getSize().x / 3, Enemies[i].enemyShape.getTexture()->getSize().y));
				if (!Enemies[i].minus)
				{
					Enemies[i].xCounter++;
					if (Enemies[i].xCounter >= 3)
					{
						Enemies[i].xCounter -= 2;
						Enemies[i].minus = true;
					}
				}
				else
				{
					Enemies[i].xCounter--;
					if (Enemies[i].xCounter < 0)
					{
						Enemies[i].xCounter += 2;
						Enemies[i].minus = false;
					}
				}
				Enemies[i].AnimationTimer.restart();

			}


			if (Enemies[i].explosion)
			{
				Enemies[i].enemyShape.setTexture(&Enemies[i].enemyExplosion);
				Enemies[i].animation();
			}

			{


				if (Enemies[i].TopLeft)
				{
					if (Enemies[i].enemyShape.getPosition().x < Enemies[i].currectPosition + bossRangex)
						Enemies[i].enemyShape.move(Enemies[i].speed, 0);
					else
					{
						Enemies[i].TopLeft = false;
						Enemies[i].TopRight = true;
						Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().y;
					}
				}
				if (Enemies[i].TopRight)
				{
					if (Enemies[i].enemyShape.getPosition().y < Enemies[i].currectPosition + bossRangey)
						Enemies[i].enemyShape.move(0, Enemies[i].speed);
					else
					{
						Enemies[i].TopRight = false;
						Enemies[i].BottomRight = true;
						Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().x;
					}
				}
				if (Enemies[i].BottomRight)
				{
					if (Enemies[i].enemyShape.getPosition().x > Enemies[i].currectPosition - bossRangex)
						Enemies[i].enemyShape.move(-Enemies[i].speed, 0);
					else
					{
						Enemies[i].BottomRight = false;
						Enemies[i].BottomLeft = true;
						Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().y;
					}
				}
				if (Enemies[i].BottomLeft)
				{
					if (Enemies[i].enemyShape.getPosition().y > Enemies[i].currectPosition - bossRangey)
						Enemies[i].enemyShape.move(0, -Enemies[i].speed);
					else
					{
						Enemies[i].BottomLeft = false;
						Enemies[i].TopLeft = true;
						Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().x;

					}

				}



			}

			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{

					if (!Players[x].Exploded && !Players[x].died && Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died && !Players[x].Exploded && !Players[i].Shield)
					{
						if (Enemies[i].Boss)
						{
							Players[x].die();
							Enemies[i].take_Damage(200);
							continue;
						}
						else
						{
							enemiesdied++;
							Players[x].die();
							Enemies[i].take_Damage(100000);
							continue;
						}
					}

				}
			}
			for (int j = 0; j < bullet.size(); j++)
			{
				if (bullet[j].direction == -1 && bullet[j].shape.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					Enemies[i].take_Damage(bullet[j].damage);
					bullet.erase(bullet.begin() + j);
					j--;
					continue;
				}
				for (int x = 0; x < Players.size(); x++)
				{
					if (bullet[j].shape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && bullet[j].direction == 1)
					{
						Players[x].die();

						bullet.erase(bullet.begin() + j);
						j--;
						break;
					}
				}
			}



			if (!Enemies[i].Died)
				window.draw(Enemies[i].enemyShape);
			else
				Enemies.erase(Enemies.begin() + i);

		}


		//Player System
		for (int i = 0; i < Players.size(); i++)
		{
			RectangleShape UIBack;
			UIBack.setSize(Vector2f(450, 90));

			UIBack.setTexture(&UIBackTex);
			UIBack.setFillColor(Color(0, 0, 100));
			//displaying score;
			Font playerscore;
			playerscore.loadFromFile("CollegiateBlackFLF.ttf");
			Text score;
			score.setOrigin(0, 0);
			stringstream stringscore;

			if (i == 0)
			{
				UIBack.setPosition(-3, 10);
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(255, 37);
			}
			else
			{
				UIBack.setPosition(window.getSize().x + 3, 10);
				UIBack.setSize(Vector2f(-450, 90));
				window.draw(UIBack);
				stringscore << Players[i].score << setw(10);
				score.setString(stringscore.str());
				score.setPosition(window.getSize().x - 415, 37);
			}
			score.setFillColor(Color::White);
			score.setFont(playerscore);


			window.draw(score);

			//number of lives
			Texture Heart;
			Heart.loadFromFile("Heart.png");
			RectangleShape heartshape;
			heartshape.setSize(Vector2f(40, 40));
			heartshape.setTexture(&Heart);
			Font lives;
			lives.loadFromFile("CollegiateBlackFLF.ttf");
			stringstream number_of_lives_to_string;
			number_of_lives_to_string << Players[i].Lives;
			Text numberoflives;
			numberoflives.setFont(lives);
			numberoflives.setString(number_of_lives_to_string.str());
			if (i == 0)
			{
				heartshape.setPosition(10, window.getSize().y - 50);
				numberoflives.setPosition(60, window.getSize().y - 46);
			}
			else
			{
				heartshape.setPosition(window.getSize().x - 50, window.getSize().y - 50);
				numberoflives.setPosition(window.getSize().x - 80, window.getSize().y - 46);
			}
			window.draw(heartshape);
			window.draw(numberoflives);

			if (Keyboard::isKeyPressed(Keyboard::J))
			{
				Players[0].score += 10000;
				Players[1].score += 10000;
			}
			if (!Players[i].Exploded)
			{

				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.overHeat > 0)
					{
						Players[i].primaryWeapon.overHeat -= Players[i].primaryWeapon.CalmingRate;
					}
					else
					{
						Players[i].primaryWeapon.overHeat = 0;
					}

					if (!Players[i].primaryWeapon.HeatCalmed)
					{

						Players[i].primaryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarPurble);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(15, 45);
						OverHeatBar.setSize(Vector2f((Players[i].primaryWeapon.overHeat * BarBackground.getSize().x) / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(15, 45);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarCyan);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 15, 45);
						OverHeatBar.setSize(Vector2f(Players[i].primaryWeapon.overHeat * BarBackground.getSize().x / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 15, 45);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);
				}
				else
				{
					if (Players[i].secondryWeapon.overHeat > 0)
					{
						Players[i].secondryWeapon.overHeat -= Players[i].secondryWeapon.CalmingRate;
					}
					else
					{
						Players[i].secondryWeapon.overHeat = 0;
					}

					if (!Players[i].secondryWeapon.HeatCalmed)
					{

						Players[i].secondryWeapon.Calming_Down();
					}

					//overheat progress bar
					RectangleShape OverHeatBar;



					if (i == 0)
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarGreen);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(220, 20));
						BarBackground.setPosition(10, 30);
						OverHeatBar.setSize(Vector2f((Players[i].secondryWeapon.overHeat * BarBackground.getSize().x) / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(10, 30);
						window.draw(BarBackground);

					}
					else
					{
						if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
							OverHeatBar.setTexture(&BarRed);
						else
							OverHeatBar.setTexture(&BarBlue);
						RectangleShape BarBackground;
						BarBackground.setTexture(&BarBack);
						BarBackground.setSize(Vector2f(-220, 20));
						BarBackground.setPosition(window.getSize().x - 10, 30);
						OverHeatBar.setSize(Vector2f(Players[i].secondryWeapon.overHeat * BarBackground.getSize().x / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
						OverHeatBar.setPosition(window.getSize().x - 10, 30);
						window.draw(BarBackground);
					}
					window.draw(OverHeatBar);

					// timer for the secondary weapon
					RectangleShape TimerBar;
					if (Players[i].timer.getElapsedTime().asSeconds() >= 20)
						TimerBar.setTexture(&BarPink);
					else
						TimerBar.setTexture(&BarOrange);

					if (i == 0)
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(220, 20));
						TimerBackground.setPosition(10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(10, 60);
						window.draw(TimerBackground);
					}
					else
					{
						RectangleShape TimerBackground;
						TimerBackground.setTexture(&BarBack);
						TimerBackground.setSize(Vector2f(-220, 20));
						TimerBackground.setPosition(window.getSize().x - 10, 60);
						TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
						TimerBar.setPosition(window.getSize().x - 10, 60);
						window.draw(TimerBackground);
					}
					window.draw(TimerBar);

				}
				Players[i].move(window.getSize().x, window.getSize().y);
				if (!Players[i].died)
				{
					Players[i].can_shoot();
				}
				if (!Players[i].secondry)
				{
					if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
					{
						Laser_Check();
						window.draw(Players[i].primaryWeapon.Laser);
					}
					else
					{
						Players[i].primaryWeapon.Laser.setSize(Vector2f(10, 0));
					}
				}
				else
				{
					if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
					{
						Laser_Check();
						
						window.draw(Players[i].secondryWeapon.Laser);
					}
					else
					{
						Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
						Players[i].secondryWeapon.BulletSound.stop();
						Players[i].secondryWeapon.loopsound = false;
					}
				}


				window.draw(Players[i].body);
				if (Players[i].Shield)
				{
					window.draw(Players[i].ShieldBody);
				}
			}

			if (Players[i].Exploded && Players[i].died)
			{
				PlayersForLeaderBoard.push_back(Players[i]);
				Players.erase(Players.begin() + i);
			}

		}

		//Cheat
		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			for (int i = 0; i < Enemies.size(); i++)
			{
				Enemies[i].take_Damage(100000);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num1))
		{
			Players[0].Change_Weapon(PrimaryWeapons[0]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num2))
		{
			Players[0].Change_Weapon(PrimaryWeapons[1]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num3))
		{
			Players[0].Change_Weapon(PrimaryWeapons[2]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num4))
		{
			Players[0].Change_Weapon(PrimaryWeapons[3]);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num5))
		{
			Players[0].Change_Weapon(SecondryWeapons[0],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Num6))
		{
			Players[0].Change_Weapon(SecondryWeapons[1],true);
		}
		else if (Keyboard::isKeyPressed(Keyboard::L))
		{
			Players[0].primaryWeapon.Level += 1;
			Players[0].secondryWeapon.Level += 1;
		}

		//Eggs System
		long long randchicken = rand() % 300;
		if (randchicken < Enemies.size())
		{
			Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x + (Enemies[randchicken].enemyShape.getSize().x / 2), Enemies[randchicken].enemyShape.getPosition().y + Enemies[randchicken].enemyShape.getSize().y);
			EnemyTimer.restart();
		}



		//Score and gifts System
		for (int i = 0; i < coinsAndChicken.size(); i++)
		{
			if (coinsAndChicken[i].body.getPosition().y <= window.getSize().y)
			{
				coinsAndChicken[i].body.move(Vector2f(0.f, 1.0f * FPSDiffRatio));
				if (coinsAndChicken[i].coin)
				{
					coinsAndChicken[i].body.setTexture(&CoinTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				else
				{
					coinsAndChicken[i].body.setTexture(&ChickTex);
					coinsAndChicken[i].body.rotate(1 * FPSDiffRatio);
				}
				window.draw(coinsAndChicken[i].body);
				for (int x = 0; x < Players.size(); x++)
				{
					if (coinsAndChicken[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
					{
						Players[x].score += coinsAndChicken[i].score;
						coinsAndChicken.erase(coinsAndChicken.begin() + i);
						i++;
						break;
					}
				}
			}
			else
			{
				coinsAndChicken.erase(coinsAndChicken.begin() + i);
				i--;
			}

		}
		for (int i = 0; i < WeaponGifts.size(); i++)
		{
			window.draw(WeaponGifts[i].body);
			if (WeaponGifts[i].levelUp)
				WeaponGifts[i].body.setTexture(&UpgradeTex);
			WeaponGifts[i].body.move(0.0f, 1.0f * FPSDiffRatio);
			WeaponGifts[i].body.rotate(1 * FPSDiffRatio);
			for (int x = 0; x < Players.size(); x++)
			{
				if (WeaponGifts[i].body.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
				{

					if (WeaponGifts[i].levelUp)
					{
						Players[x].Improve_Weapon();
					}
					else
					{
						if (WeaponGifts[i].SecondryOrPrimary <= 50)
							Players[x].Change_Weapon(WeaponGifts[i].SecondryGiftWeapon, true);
						else
							Players[x].Change_Weapon(WeaponGifts[i].PrimaryGiftWeapon, false);
					}
					WeaponGifts.erase(WeaponGifts.begin() + i);
					i--;
					break;
				}
			}
		}


		Bullet_Move();


		window.display();
	}
	if (Players.size() > 0)
		Wave_1_Level_3();
	else
		Lose();
	return 1;

}