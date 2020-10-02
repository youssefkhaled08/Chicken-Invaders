#pragma once

#include "MainVariables.h"
#include "WinOrLose.h"
extern void Bullet_Move();
extern void Laser_Check();

extern bool pause();



bool Wave_2_Level_3();
bool Wave_3_Level_3();
int enemiesdied = 0;

bool Wave_1_Level_3()
{
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Wave 7");
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
	int dist = -100;
	SoundBuffer ChickenHitBuffer;
	ChickenHitBuffer.loadFromFile("ChickenHit.wav");
	for (int i = 0; i < 57; i++)
	{
		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		newEnemy.enemyShape.setPosition(dist, 100);
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 5;
		newEnemy.ChickenHit.setBuffer(ChickenHitBuffer);
		Enemies.push_back(newEnemy);
		dist -= 120;
	}
	bool highSpeed = true;
	enemiesdied = 0;


	//Making Player Ready
	for (int i = 0; i < Players.size(); i++)
	{
		Players[i].shieldTime = 3;
		Players[i].ShieldTimer.restart();
		Players[i].Lives += 1;
		Players[i].Activate_Shield();
	}

	//Change Direction Points (1720,100) Down , (1720 ,220) Left , (250, 220) Down , (250, 340) Right , (1720,340) Down
	// (1720, 460) Left , (250,460) 

	Thread nextWave(Wave_2_Level_3);
	nextWave.wait();

	while (waves > 0 && window.isOpen())
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

			if (highSpeed)
				Enemies[i].speed = 20;
			else
				Enemies[i].speed = 4 * FPSDiffRatio;

			//Moveeeeee
			if (Enemies[i].enemyShape.getPosition().x < 1720 && Enemies[i].enemyShape.getPosition().y <= 100)
			{
				if (Enemies[i].loop)
					highSpeed = false;
				Enemies[i].loop = false;
				if (Enemies[i].enemyShape.getPosition().x + Enemies[i].speed > 1720)
				{
					Enemies[i].enemyShape.setPosition(1720, 100);
					Enemies[i].difference = Enemies[i].enemyShape.getPosition().x + Enemies[i].speed - 1720;
				}
				else
				{
					Enemies[i].enemyShape.move(Enemies[i].speed + abs(Enemies[i].difference), 0);
					Enemies[i].difference = 0;
				}
			}
			else if (Enemies[i].enemyShape.getPosition().x >= 1720 && Enemies[i].enemyShape.getPosition().y < 220 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().y + Enemies[i].speed > 220)
				{
					Enemies[i].enemyShape.setPosition(1720, 220);
					Enemies[i].difference = Enemies[i].enemyShape.getPosition().y + Enemies[i].speed - 220;
				}
				else
				{
					Enemies[i].enemyShape.move(0, Enemies[i].speed + abs(Enemies[i].difference));
					Enemies[i].difference = 0;
				}
			}
			else if (Enemies[i].enemyShape.getPosition().x > 250 && Enemies[i].enemyShape.getPosition().y >= 220 && Enemies[i].enemyShape.getPosition().y < 340 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().x - Enemies[i].speed < 250)
				{
					Enemies[i].enemyShape.setPosition(250, 220);
					Enemies[i].difference = 250 - Enemies[i].enemyShape.getPosition().x - Enemies[i].speed;
				}
				else
				{
					Enemies[i].enemyShape.move(-(Enemies[i].speed + abs(Enemies[i].difference)), 0);
					Enemies[i].difference = 0;
				}
			}
			else if (Enemies[i].enemyShape.getPosition().x <= 250 && Enemies[i].enemyShape.getPosition().y >= 220 && Enemies[i].enemyShape.getPosition().y < 340 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().y + Enemies[i].speed > 340)
				{
					Enemies[i].enemyShape.setPosition(250, 340);
					Enemies[i].difference = Enemies[i].enemyShape.getPosition().y + Enemies[i].speed - 340;
				}
				else
				{
					Enemies[i].enemyShape.move(0, Enemies[i].speed + abs(Enemies[i].difference));
					Enemies[i].difference = 0;
				}
			}
			else if (Enemies[i].enemyShape.getPosition().x < 1720 && Enemies[i].enemyShape.getPosition().y >= 340 && Enemies[i].enemyShape.getPosition().y < 450 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().x + Enemies[i].speed > 1720)
				{
					Enemies[i].enemyShape.setPosition(1720, 340);
					Enemies[i].difference = Enemies[i].enemyShape.getPosition().x + Enemies[i].speed - 1720;
				}
				else
				{
					Enemies[i].enemyShape.move(Enemies[i].speed + abs(Enemies[i].difference), 0);
					Enemies[i].difference = 0;
				}
			}
			else if (Enemies[i].enemyShape.getPosition().x >= 1720 && Enemies[i].enemyShape.getPosition().y >= 340 && Enemies[i].enemyShape.getPosition().y < 460 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().y + Enemies[i].speed > 460)
				{
					Enemies[i].enemyShape.setPosition(1720, 460);
					Enemies[i].difference = Enemies[i].enemyShape.getPosition().y + Enemies[i].speed - 460;
				}
				else
				{
					Enemies[i].enemyShape.move(0, Enemies[i].speed + abs(Enemies[i].difference));
					Enemies[i].difference = 0;

				}
			}
			else if (Enemies[i].enemyShape.getPosition().x > 130 && Enemies[i].enemyShape.getPosition().y >= 460 && !Enemies[i].loop)
			{
				if (Enemies[i].enemyShape.getPosition().x - Enemies[i].speed < 130)
				{
					Enemies[i].enemyShape.setPosition(130, 460);
					Enemies[i].difference = 130 - Enemies[i].enemyShape.getPosition().x - Enemies[i].speed;
				}
				else
				{
					Enemies[i].enemyShape.move(-(Enemies[i].speed + abs(Enemies[i].difference)), 0);
					Enemies[i].difference = 0;

				}
			}
			else if (Enemies[i].enemyShape.getPosition().x >= 130 && Enemies[i].enemyShape.getPosition().y > 100 && Enemies[i].enemyShape.getPosition().y <= 500)
			{
				Enemies[i].loop = true;
				if (Enemies[i].enemyShape.getPosition().y - Enemies[i].speed < 100)
				{
					Enemies[i].enemyShape.setPosition(130, 100);
					Enemies[i].difference = 100 - Enemies[i].enemyShape.getPosition().y - Enemies[i].speed;
				}
				else
				{
					Enemies[i].enemyShape.move(0, -(Enemies[i].speed + abs(Enemies[i].difference)));
					Enemies[i].difference = 0;
				}
			}




			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{
					if (!Players[x].Shield && !Players[x].Exploded && !Players[x].died && Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died)
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
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x + (Enemies[randchicken].enemyShape.getSize().x / 2), Enemies[randchicken].enemyShape.getPosition().y + Enemies[randchicken].enemyShape.getSize().y);
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
		if (enemiesdied >= 57 && waves > 0)
		{
			waves--;
			enemiesdied = 0;
			int dist = -100;
			for (int i = 0; i < 57; i++)
			{
				enemyStructure newEnemy;
				newEnemy.healthPoints = 100 * (3 - waves);
				newEnemy.enemyShape.setTexture(&ChickenTex);
				newEnemy.enemyShape.setPosition(dist, 100);
				newEnemy.enemyShape.setSize(Vector2f(100, 100));
				newEnemy.eggTex = EggTex;
				newEnemy.enemyExplosion = EnemyExplosion;
				newEnemy.speed = 5;
				Enemies.push_back(newEnemy);
				dist -= 120;
			}
			highSpeed = true;
		}
		Bullet_Move();
		window.display();
	}

	if (Players.size() > 0)
	{
		Wave_2_Level_3();
	}
	else
	{
		Lose();
	}
	return 1;

}

bool Wave_2_Level_3()
{
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Wave 8");
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
	float dist = -100;
	float xtopleft = 1570;
	float xbottomright = 2170;
	//Enemy Setup
	for (int i = 0; i < 7; i++)
	{

		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		newEnemy.enemyShape.setPosition(dist, 70);
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 1;
		newEnemy.TopLeft = false;
		newEnemy.TopRight = true;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 70;
		Enemies.push_back(newEnemy);
		dist -= 120;
		newEnemy.enemyShape.setPosition(dist, 70);
		newEnemy.TopLeft = true;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = xtopleft - 240 * i;  //50
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist, 190);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = true;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 190;
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist + 120, 190);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = true;
		Enemies.push_back(newEnemy);
		newEnemy.currectPosition = 410 - 240 * i;
		dist -= 120;
	}
	dist = -100;
	for (int i = 0; i < 7; i++)
	{

		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		newEnemy.enemyShape.setPosition(dist, 310);
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 1;
		newEnemy.TopLeft = false;
		newEnemy.TopRight = true;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 310;
		Enemies.push_back(newEnemy);
		dist -= 120;
		newEnemy.enemyShape.setPosition(dist, 310);
		newEnemy.TopLeft = true;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = xtopleft - 240 * i;  //50
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist, 430);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = true;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 430;
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist + 120, 430);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = true;
		Enemies.push_back(newEnemy);
		newEnemy.currectPosition = 410 - 240 * i;
		dist -= 120;
	}
	dist = -100;
	for (int i = 0; i < 7; i++)
	{

		enemyStructure newEnemy;
		newEnemy.healthPoints = 100;
		newEnemy.enemyShape.setTexture(&ChickenTex);
		newEnemy.enemyShape.setPosition(dist, 550);
		newEnemy.enemyShape.setSize(Vector2f(100, 100));
		newEnemy.eggTex = EggTex;
		newEnemy.enemyExplosion = EnemyExplosion;
		newEnemy.speed = 1;
		newEnemy.TopLeft = false;
		newEnemy.TopRight = true;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 550;
		Enemies.push_back(newEnemy);
		dist -= 120;
		newEnemy.enemyShape.setPosition(dist, 550);
		newEnemy.TopLeft = true;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = xtopleft - 240 * i;  //50
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist, 670);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = true;
		newEnemy.BottomRight = false;
		newEnemy.currectPosition = 670;
		Enemies.push_back(newEnemy);
		newEnemy.enemyShape.setPosition(dist + 120, 670);
		newEnemy.TopLeft = false;
		newEnemy.TopRight = false;
		newEnemy.BottomLeft = false;
		newEnemy.BottomRight = true;
		Enemies.push_back(newEnemy);
		newEnemy.currectPosition = 410 - 240 * i;
		dist -= 120;
	}
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



			//Moveeeeee
			if (Enemies[i].TopLeft && Enemies[i].enemyShape.getPosition().x <= xtopleft && !Entered)
			{

				Enemies[i].enemyShape.move(5, 0);
				Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().x;
			}
			else if (Enemies[i].TopRight && Enemies[i].enemyShape.getPosition().x <= xbottomright && !Entered)
			{
				Enemies[i].enemyShape.move(5, 0);
				Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().y;
			}

			else if (Enemies[i].BottomLeft && Enemies[i].enemyShape.getPosition().x <= xtopleft && !Entered)
			{
				Enemies[i].enemyShape.move(5, 0);
				Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().y;
			}

			else if (Enemies[i].BottomRight && Enemies[i].enemyShape.getPosition().x <= xbottomright && !Entered)
			{
				Enemies[i].enemyShape.move(5, 0);
				Enemies[i].currectPosition = Enemies[i].enemyShape.getPosition().x;
			}

			else
			{
				Entered = true;
			}
			//Enemies[i].enemyShape.move(5, 0);


			if (Entered)
			{

				if (Enemies[i].TopLeft)
				{
					if (Enemies[i].enemyShape.getPosition().x < Enemies[i].currectPosition + 120)
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
					if (Enemies[i].enemyShape.getPosition().y < Enemies[i].currectPosition + 120)
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
					if (Enemies[i].enemyShape.getPosition().x > Enemies[i].currectPosition - 120)
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
					if (Enemies[i].enemyShape.getPosition().y > Enemies[i].currectPosition - 120)
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
					if (!Players[x].Shield && !Players[x].Exploded && !Players[x].died && Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()) && !Players[x].died)
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
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x + (Enemies[randchicken].enemyShape.getSize().x / 2), Enemies[randchicken].enemyShape.getPosition().y + Enemies[randchicken].enemyShape.getSize().y);
				EnemyTimer.restart();
			}
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


		if (Enemies.size() == 0 && waves > 0)
		{
			waves--;
			dist = -100;
			xtopleft = 1570;
			xbottomright = 2170;
			//Enemy Setup
			for (int i = 0; i < 7; i++)
			{

				enemyStructure newEnemy;
				newEnemy.healthPoints = 100;
				newEnemy.enemyShape.setTexture(&ChickenTex);
				newEnemy.enemyShape.setPosition(dist, 70);
				newEnemy.enemyShape.setSize(Vector2f(100, 100));
				newEnemy.eggTex = EggTex;
				newEnemy.enemyExplosion = EnemyExplosion;
				newEnemy.speed = 1;
				newEnemy.TopLeft = false;
				newEnemy.TopRight = true;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 70;
				Enemies.push_back(newEnemy);
				dist -= 120;
				newEnemy.enemyShape.setPosition(dist, 70);
				newEnemy.TopLeft = true;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = xtopleft - 240 * i;  //50
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist, 190);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = true;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 190;
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist + 120, 190);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = true;
				Enemies.push_back(newEnemy);
				newEnemy.currectPosition = 410 - 240 * i;
				dist -= 120;
			}
			dist = -100;
			for (int i = 0; i < 7; i++)
			{

				enemyStructure newEnemy;
				newEnemy.healthPoints = 100;
				newEnemy.enemyShape.setTexture(&ChickenTex);
				newEnemy.enemyShape.setPosition(dist, 310);
				newEnemy.enemyShape.setSize(Vector2f(100, 100));
				newEnemy.eggTex = EggTex;
				newEnemy.enemyExplosion = EnemyExplosion;
				newEnemy.speed = 1;
				newEnemy.TopLeft = false;
				newEnemy.TopRight = true;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 310;
				Enemies.push_back(newEnemy);
				dist -= 120;
				newEnemy.enemyShape.setPosition(dist, 310);
				newEnemy.TopLeft = true;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = xtopleft - 240 * i;  //50
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist, 430);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = true;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 430;
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist + 120, 430);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = true;
				Enemies.push_back(newEnemy);
				newEnemy.currectPosition = 410 - 240 * i;
				dist -= 120;
			}
			dist = -100;
			for (int i = 0; i < 7; i++)
			{

				enemyStructure newEnemy;
				newEnemy.healthPoints = 100;
				newEnemy.enemyShape.setTexture(&ChickenTex);
				newEnemy.enemyShape.setPosition(dist, 550);
				newEnemy.enemyShape.setSize(Vector2f(100, 100));
				newEnemy.eggTex = EggTex;
				newEnemy.enemyExplosion = EnemyExplosion;
				newEnemy.speed = 1;
				newEnemy.TopLeft = false;
				newEnemy.TopRight = true;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 550;
				Enemies.push_back(newEnemy);
				dist -= 120;
				newEnemy.enemyShape.setPosition(dist, 550);
				newEnemy.TopLeft = true;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = xtopleft - 240 * i;  //50
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist, 670);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = true;
				newEnemy.BottomRight = false;
				newEnemy.currectPosition = 670;
				Enemies.push_back(newEnemy);
				newEnemy.enemyShape.setPosition(dist + 120, 670);
				newEnemy.TopLeft = false;
				newEnemy.TopRight = false;
				newEnemy.BottomLeft = false;
				newEnemy.BottomRight = true;
				Enemies.push_back(newEnemy);
				newEnemy.currectPosition = 410 - 240 * i;
				dist -= 120;
			}

			Entered = false;
		}
		window.display();
	}

	if (Players.size() > 0)
	{
		Wave_3_Level_3();
	}
	else
	{
		Lose();
	}
	return 1;
}

bool Wave_3_Level_3()
{
	Background.setTexture(&BackgroundLevel3);
	Background.setTextureRect(IntRect(0, 0, BackgroundLevel3.getSize().x, BackgroundLevel3.getSize().y));
	window.setFramerateLimit(100);
	Text load;
	Color WaveColor(255, 255, 255, 0);
	load.setString("Boss III Wave");
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

	enemyStructure enemy;
	Enemies.push_back(enemy);
	Enemies[0].enemyShape.setPosition(100, -900);
	Enemies[0].enemyShape.setSize(Vector2f(450, 575));
	Enemies[0].enemyShape.setTexture(&ChickenTex);
	Enemies[0].startPoint = (960); Enemies[0].endPoint = (1520);

	if (Players.size() == 1)
	{
		Enemies[0].upPosition = 0; Enemies[0].downposition = (Players[0].body.getPosition().y - 200);
	}


	Enemies[0].eggTex = EggTex;
	Enemies[0].enemyExplosion = EnemyExplosion;
	Enemies[0].healthPoints = 10000;
	Enemies[0].speed = 2;

	Enemies.push_back(enemy);
	Enemies[1].enemyShape.setTexture(&ChickenTex);
	Enemies[1].enemyShape.setPosition(100, -900);
	Enemies[1].enemyShape.setSize(Vector2f(450, 575));
	Enemies[1].startPoint = (0); Enemies[1].endPoint = (560);
	Enemies[1].upPosition = 0; Enemies[1].downposition = (Players[0].body.getPosition().y) - 200;
	Enemies[1].eggTex = EggTex;
	Enemies[1].enemyExplosion = EnemyExplosion;
	Enemies[1].healthPoints = 10000;
	Enemies[1].speed = 2;

	Enemies[0].Boss = true;
	Enemies[1].Boss = true;

	//Making Player Ready
	for (int i = 0; i < Players.size(); i++)
	{
		Players[i].shieldTime = 3;
		Players[i].ShieldTimer.restart();
		Players[i].Lives += 1;
		Players[i].Activate_Shield();
	}


	for (int i = 0; i < 4;i++)
	{
		WeaponDrop LevelUp;
		LevelUp.levelUp = true;
		LevelUp.body.setPosition(rand() % 1500 + 200, -(rand() % 300));
		LevelUp.body.setSize(Vector2f(60,60));


	}
	while (Players.size() > 0 && Enemies.size() > 0)
	{

		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);
		Time FPSTime = FPSClock.getElapsedTime();
		FPSClock.restart();
		Event evnt;
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

		long long randchicken = rand() % 200;
		if (randchicken < Enemies.size())
		{
			if (Enemies[randchicken].healthPoints <= 5000)
				Enemies[randchicken].shoot(Enemies[randchicken].enemyShape.getPosition().x + (Enemies[randchicken].enemyShape.getSize().x / 2), Enemies[randchicken].enemyShape.getPosition().y + Enemies[randchicken].enemyShape.getSize().y);
			EnemyTimer.restart();
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
			Enemies[i].move(Enemies[i].enemyShape.getPosition().x);
			Enemies[i].moveUPandDown(Enemies[i].enemyShape.getPosition().y);
			for (int x = 0; x < Players.size(); x++)
			{
				if (!Enemies[i].explosion)
				{
					if (!Players[x].Shield && !Players[x].Exploded && !Players[x].died && Enemies[i].enemyShape.getGlobalBounds().intersects(Players[x].body.getGlobalBounds()))
					{
						Players[x].die();

						Enemies[i].take_Damage(300);//low damage given by the players
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
			{
				if (Enemies[i].healthPoints >= 8000)
				{
					Enemies[i].enemyShape.setTexture(&EggTex);
					Enemies[i].enemyShape.setTextureRect(IntRect(0, 0, EggTex.getSize().x, EggTex.getSize().y));
				}
				else if (Enemies[i].healthPoints >= 6000)
				{
					//Enemies[i].enemyShape.setTexture(&MidEggCrack);
					Enemies[i].enemyShape.setTextureRect(IntRect(0, 0, EggTex.getSize().x, EggTex.getSize().y));
				}
				else if (Enemies[i].healthPoints >= 5000)
				{
					//Enemies[i].enemyShape.setTexture(&HighEggCrack);
					Enemies[i].enemyShape.setTextureRect(IntRect(0, 0, EggTex.getSize().x, EggTex.getSize().y));
				}
				else
				{
					Enemies[i].enemyShape.setTexture(&ChickenTex);
					Enemies[i].Boss = false;
					Enemies[i].enemyShape.setSize(Vector2f(400, 400));
				}

				window.draw(Enemies[i].enemyShape);



			}

			else
				Enemies.erase(Enemies.begin() + i);

		}

		Bullet_Move();


		window.display();

	}

	if (Players.size() > 0)
		Win();
	else
		Lose();



	return 1;
}
