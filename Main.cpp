#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<math.h>
#include "Player.h"
#include "Enemy.h"
#include <sstream>

using namespace sf;
using namespace std;


RenderWindow window(VideoMode(1000, 1000), "Chicken Invaders", Style::Close | Style::Resize);

Weapon Plasma = { "Electericity",5,1,"Plasma",0.01,70,0,2,0.06,0.02,0.1 },Laser = { "Laser",5,1,"Laser",0.01,70,0,2,0.06,0.02,0.1 }, ionBlaster = { "Bullets",30,1,"ionBlaster",0.4,20,0,8,0.6,0.002,0.06 }, neutronGun = { "Bullets",30,1,"neutronGun",0.5,20,0,8,1,0.002,0.07 }, hypergun = { "Bullets",30,1,"hypergun",0.5,20,0,8,1,0.02,0.07 }, vulcangun = { "Bullets",30,1,"vulcangun",0.5,20,0,8,1,0.02,0.07 };


vector<enemyStructure> Enemies;
vector<ScoreDrop> coinsAndChicken;
vector<WeaponDrop> WeaponGifts;
vector<Texture> WeaponGiftTextures;
vector<Weapon> Weapons;
vector<Player>Players;

int giftsCounter = 0;
int EnemiesSize;
Texture bulletTex;
bool GiftChecker = false;

void Bullet_Move()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		Vector2f bulletPosition = bullet[i].shape.getPosition();
		bullet[i].shape.move(bullet[i].speed * cos(bullet[i].angle), bullet[i].speed * bullet[i].direction * sin(bullet[i].angle));
		bullet[i].shape.setTexture(&bullet[i].BulletTex);
		if (bulletPosition.y < 0 || bulletPosition.y > window.getSize().y)
		{
			bullet.erase(bullet.begin() + i);
			i--;
		}
		else
		{
			window.draw(bullet[i].shape);
		}
	}

}
bool one = true;
Clock LaserTimer;
void Laser_Check()
{
	for (int x = 0; x < Players.size(); x++)
	{

		for (int i = 0; i < Enemies.size(); i++)
		{
			if (!Players[x].secondry)
			{
				if (Players[x].primaryWeapon.Laser.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					if (!Players[x].primaryWeapon.found)
					{
						Players[x].primaryWeapon.found = true;
						Players[x].primaryWeapon.index = i;
					}
					else
					{
						if (Enemies[i].enemyShape.getPosition().y > Enemies[Players[x].primaryWeapon.index].enemyShape.getPosition().y)
						{
							Players[x].primaryWeapon.index = i;
						}

					}
				}
			}
			else
			{
				if (Players[x].secondryWeapon.Laser.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds()) && !Enemies[i].explosion)
				{
					if (!Players[x].secondryWeapon.found)
					{
						Players[x].secondryWeapon.found = true;
						Players[x].secondryWeapon.index = i;
					}
					else
					{
						if (Enemies[i].enemyShape.getPosition().y > Enemies[Players[x].secondryWeapon.index].enemyShape.getPosition().y)
						{
							Players[x].secondryWeapon.index = i;
						}

					}
				}
			}
		}
		if (!Players[x].secondry)
		{
			if (Players[x].primaryWeapon.found)
			{
				Players[x].primaryWeapon.Laser.setSize(Vector2f(Players[x].primaryWeapon.Laser.getSize().x, Enemies[Players[x].primaryWeapon.index].enemyShape.getPosition().y + Enemies[Players[x].primaryWeapon.index].enemyShape.getSize().y - 10 - Players[x].body.getPosition().y));
				Enemies[Players[x].primaryWeapon.index].take_Damage(Players[x].primaryWeapon.damage);
				Players[x].primaryWeapon.found = false;
			}
			if (Players[x].primaryWeapon.type == "Electericity")
			{
				if (LaserTimer.getElapsedTime().asSeconds() >= 0.1)
					if (one)
					{
						Players[x].primaryWeapon.Laser.setTextureRect(IntRect(Players[x].primaryWeapon.Laser.getTexture()->getSize().x, Players[x].primaryWeapon.Laser.getTexture()->getSize().y, -124, -512));
						one = false;
						if (Players[x].primaryWeapon.Laser.getRotation() > 353 || Players[x].primaryWeapon.Laser.getRotation() < 15)
							Players[x].primaryWeapon.Laser.rotate(-rand() % 7);
						LaserTimer.restart();
					}
					else
					{
						Players[x].primaryWeapon.Laser.setTextureRect(IntRect(0, 0, Players[x].primaryWeapon.Laser.getTexture()->getSize().x, Players[x].primaryWeapon.Laser.getTexture()->getSize().y));
						one = true;
						if (Players[x].primaryWeapon.Laser.getRotation() > 345 || Players[x].primaryWeapon.Laser.getRotation() < 5)
							Players[x].primaryWeapon.Laser.rotate(rand() % 7);
						LaserTimer.restart();

					}
			}
		}
		else
		{
			if (Players[x].secondryWeapon.found)
			{
				Players[x].secondryWeapon.Laser.setSize(Vector2f(Players[x].secondryWeapon.Laser.getSize().x, Enemies[Players[x].secondryWeapon.index].enemyShape.getPosition().y + Enemies[Players[x].secondryWeapon.index].enemyShape.getSize().y - 10 - Players[x].body.getPosition().y));
				Enemies[Players[x].secondryWeapon.index].take_Damage(Players[x].secondryWeapon.damage);
				Players[x].secondryWeapon.found = false;
			}
			if (Players[x].secondryWeapon.type == "Electericity")
			{
				if (LaserTimer.getElapsedTime().asSeconds() >= 0.1)
					if (one)
					{
						Players[x].secondryWeapon.Laser.setTextureRect(IntRect(Players[x].secondryWeapon.Laser.getTexture()->getSize().x, Players[x].secondryWeapon.Laser.getTexture()->getSize().y, -124, -512));
						one = false;
						if (Players[x].secondryWeapon.Laser.getRotation() > 353 || Players[x].secondryWeapon.Laser.getRotation() < 15)
							Players[x].secondryWeapon.Laser.rotate(-rand() % 7);
						LaserTimer.restart();
					}
					else
					{
						Players[x].secondryWeapon.Laser.setTextureRect(IntRect(0, 0, Players[x].secondryWeapon.Laser.getTexture()->getSize().x, Players[x].secondryWeapon.Laser.getTexture()->getSize().y));
						one = true;
						if (Players[x].secondryWeapon.Laser.getRotation() > 345 || Players[x].secondryWeapon.Laser.getRotation() < 5)
							Players[x].secondryWeapon.Laser.rotate(rand() % 7);
						LaserTimer.restart();

					}
			}
		}
	}

}
vector<Bullet> bullet;
float EnemyRateOfFire = 0.5;
Clock EnemyTimer;

int main()
{

	window.setFramerateLimit(120);
	Texture ChickenTex;
	ChickenTex.loadFromFile("Chicken.png");
	
	Texture EnemyExplosion, EggTex, CoinTex, ChickTex;
	EnemyExplosion.loadFromFile("smoke.png");
	EggTex.loadFromFile("egg.png");
	CoinTex.loadFromFile("Coin.png");
	ChickTex.loadFromFile("Chick.png");
	
	//Enemy Setup
	for (int i = 0; i < 10; i++)
	{
		enemyStructure enemy;
		Enemies.push_back(enemy);
		Enemies[i].enemyShape.setTexture(&ChickenTex);
		Enemies[i].enemyShape.setPosition((i) * 98, 10);
		Enemies[i].enemyShape.setSize(Vector2f(75, 75));
		Enemies[i].startPoint = (i) * 98; Enemies[i].endPoint = (1 + i) * 98;
		Enemies[i].eggTex = EggTex;
		Enemies[i].enemyExplosion = EnemyExplosion;



	}
	for (int i = 10; i < 20; i++)
	{
		enemyStructure enemy;
		Enemies.push_back(enemy);
		Enemies[i].enemyShape.setTexture(&ChickenTex);
		Enemies[i].enemyShape.setPosition((i - 10) * 98, 100);
		Enemies[i].enemyShape.setSize(Vector2f(75, 75));
		Enemies[i].startPoint = (i - 10) * 98; Enemies[i].endPoint = (1 + i - 10) * 98;
		Enemies[i].eggTex = EggTex;
		Enemies[i].enemyExplosion = EnemyExplosion;

	}
	for (int i = 20; i < 30; i++)
	{
		enemyStructure enemy;
		Enemies.push_back(enemy);
		Enemies[i].enemyShape.setTexture(&ChickenTex);
		Enemies[i].enemyShape.setPosition((i - 20) * 98, 190);
		Enemies[i].enemyShape.setSize(Vector2f(75, 75));
		Enemies[i].startPoint = (i - 20) * 98; Enemies[i].endPoint = (1 + i - 20) * 98;
		Enemies[i].eggTex = EggTex;
		Enemies[i].enemyExplosion = EnemyExplosion;

	}


	//Background Setup
	RectangleShape Background(Vector2f(window.getSize().x, window.getSize().y));
	Texture BackgroundTexture;
	BackgroundTexture.loadFromFile("SpaceBackground.jpg");
	Background.setTexture(&BackgroundTexture);


	//Players Setup
	Player player1 = { Keyboard::Key::W,Keyboard::Key::S,Keyboard::Key::A,Keyboard::Key::D,Keyboard::Key::Space },
		player2 = { Keyboard::Key::Up,Keyboard::Key::Down,Keyboard::Key::Left,Keyboard::Key::Right,Keyboard::Key::RShift };

	
	


	Texture playerTex, LaserTex , PlasmaTex;
	playerTex.loadFromFile("SpaceShipAnimated.png");
	LaserTex.loadFromFile("laser_purple.png");
	PlasmaTex.loadFromFile("PlasmaRifle.png");
	Texture UpgradeTex;
	UpgradeTex.loadFromFile("CI5Atom.png");
	//LaserTex.setRepeated(true);
	ionBlaster.weaponTex.loadFromFile("ion blaster single.png");
	ionBlaster.xSize = 25;
	ionBlaster.ySize = 100;
	ionBlaster.rightAngle2 = 80;
	ionBlaster.leftAngle2 = 100;
	ionBlaster.rightAngle3 = 80;
	ionBlaster.leftAngle3 = 100;
	ionBlaster.rightAngle4 = 85;
	ionBlaster.leftAngle4 = 95;

	neutronGun.weaponTex.loadFromFile("neutron gun medium.png");
	neutronGun.xSize = 25;
	neutronGun.ySize = 100;
	neutronGun.rightAngle2 = 80;
	neutronGun.leftAngle2 = 100;
	neutronGun.rightAngle3 = 70;
	neutronGun.leftAngle3 = 110;
	neutronGun.rightAngle4 = 85;
	neutronGun.leftAngle4 = 95;

	hypergun.weaponTex.loadFromFile("hyper gun weak.png");
	hypergun.xSize = 25;
	hypergun.ySize = 100;
	hypergun.rightAngle2 = 85;
	hypergun.leftAngle2 = 95;
	hypergun.rightAngle3 = 75;
	hypergun.leftAngle3 = 105;
	hypergun.rightAngle4 = 85;
	hypergun.leftAngle4 = 95;

	vulcangun.weaponTex.loadFromFile("SuperVulcanChaingunWeak.png");
	vulcangun.xSize = 25;
	vulcangun.ySize = 100;
	vulcangun.rightAngle2 = 85;
	vulcangun.leftAngle2 = 95;
	vulcangun.rightAngle3 = 85;
	vulcangun.leftAngle3 = 95;
	vulcangun.rightAngle4 = 85;
	vulcangun.leftAngle4 = 95;
	Plasma.Laser.setTexture(&PlasmaTex);
	Laser.Laser.setTexture(&LaserTex);
	Weapons.push_back(ionBlaster);
	Weapons.push_back(neutronGun);
	Weapons.push_back(hypergun);
	Weapons.push_back(vulcangun);
	Weapons.push_back(Plasma);
	Weapons.push_back(Laser);
	Texture ionBlasterGift, neutronGunGift, hyperGunGift,vulcanGunGift,PlasmaGift,LaserGift;
	ionBlasterGift.loadFromFile("GIFTIonBlaster.png");
	neutronGunGift.loadFromFile("GIFTNeutronGun.png");
	hyperGunGift.loadFromFile("GIFTHypergun.png");
	vulcanGunGift.loadFromFile("GIFTVulcanChaingun.png");
	PlasmaGift.loadFromFile("GIFTPlasmaRifle.png");
	LaserGift.loadFromFile("GIFTLightningFryer.png");
	WeaponGiftTextures.push_back(ionBlasterGift);
	WeaponGiftTextures.push_back(neutronGunGift);
	WeaponGiftTextures.push_back(hyperGunGift);
	WeaponGiftTextures.push_back(vulcanGunGift);
	WeaponGiftTextures.push_back(PlasmaGift);
	WeaponGiftTextures.push_back(LaserGift);

	//Player 1 PreSetup
	{
		player1.body.setSize(Vector2f(90.0f, 115.0f));
		player1.body.setTexture(&playerTex);
		//player1.body.setFillColor(Color::Cyan);
		player1.body.setPosition(700, 800);
		player1.primaryWeapon = neutronGun;
		player1.primaryWeapon.Level = 4;
		player1.speed = 3.5f;
		player1.primaryWeapon.LaserSize = 30;
		player1.Lives = 3;
		player1.name = "Youssef";
		player1.refreshRate = 0.015f;
		player1.numberOfImages = 10;
		player1.ImagesCounter = 0;
		player1.singleImageWidth = playerTex.getSize().x / 10;
		player1.Explosion.loadFromFile("Explosion.png");
		player1.PlayerTexture = playerTex;
		player1.ExplosionSoundBuffer.loadFromFile("Explosion+3.wav");
		player1.ExplosionSound.setBuffer(player1.ExplosionSoundBuffer);
		Players.push_back(player1);
	}
	//Player 2 PreSetup
	{
		player2.body.setSize(Vector2f(90.0f, 115.0f));
		player2.body.setTexture(&playerTex);
		player2.body.setPosition(300, 800);
		player2.primaryWeapon = Plasma;
		player2.primaryWeapon.LaserSize = 30;
		player2.speed = 3.5f;
		player2.name = "Player 2";
		player2.Lives = 3;
		player2.refreshRate = 0.02f;
		player2.numberOfImages = 10;
		player2.ImagesCounter = 0;
		player2.singleImageWidth = playerTex.getSize().x / 10;
		player2.Explosion.loadFromFile("Explosion.png");
		player2.PlayerTexture = playerTex;
		player2.ExplosionSoundBuffer.loadFromFile("Explosion+5.wav");
		player2.ExplosionSound.setBuffer(player2.ExplosionSoundBuffer);
		Players.push_back(player2);
	}



	
	while (window.isOpen())
	{
		EnemiesSize = Enemies.size();
		window.clear();
		window.draw(Background);


		Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case Event::Closed:
				window.close();
				break;
			}

		}

		
		//Player System
		for (int i = 0; i < Players.size(); i++)
		{
			Font playerscore;
			playerscore.loadFromFile("CollegiateBlackFLF.ttf");
			Text score;
			stringstream stringscore;
			stringscore << Players[i].score;
			score.setString(stringscore.str());
			score.setFillColor(Color::White);
			score.setFont(playerscore);
			if (i == 0)
				score.setPosition(1, 10);
			else
				score.setPosition(900, 10);
			window.draw(score);
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
				}
					Players[i].move(window.getSize().x, window.getSize().y);
				if (!Players[i].died)
				{
					Players[i].can_shoot();
				}
				if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
				{
					Laser_Check();
					window.draw(Players[i].primaryWeapon.Laser);
				}
				else
				{
					Players[i].primaryWeapon.Laser.setSize(Vector2f(10,0));
				}
				if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
				{
					Laser_Check();
					window.draw(Players[i].secondryWeapon.Laser);
				}
				else
				{
					Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
				}
				

				window.draw(Players[i].body);
			}
		}

		//Eggs System
		for (int i = 0; i < 2; i++)
		{
			long long randchicken = rand();
			if (randchicken < Enemies.size() && EnemyTimer.getElapsedTime().asSeconds() >= EnemyRateOfFire)
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
				coinsAndChicken[i].body.move(Vector2f(0.f, 1.0f));
				window.draw(coinsAndChicken[i].body);
				if (coinsAndChicken[i].coin)
				{
					coinsAndChicken[i].body.setTexture(&CoinTex);
					coinsAndChicken[i].body.rotate(1);
				}
				else
				{
					coinsAndChicken[i].body.setTexture(&ChickTex);
					coinsAndChicken[i].body.rotate(1);
				}
				for (int x=0 ; x < Players.size(); x++)
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
				coinsAndChicken.erase(coinsAndChicken.begin() + i);

		}
		for (int i = 0; i < WeaponGifts.size(); i++)
		{
			window.draw(WeaponGifts[i].body);
			if (WeaponGifts[i].levelUp)
				WeaponGifts[i].body.setTexture(&UpgradeTex);
			WeaponGifts[i].body.move(0.0f, 1.0f);
			WeaponGifts[i].body.rotate(1);
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
						Players[x].Change_Weapon(WeaponGifts[i].giftWeapon, true);
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
					if (bullet[j].direction == -1 && bullet[j].shape.getGlobalBounds().intersects(Enemies[i].enemyShape.getGlobalBounds())&& !Enemies[i].explosion )
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
			window.draw(Players[1].primaryWeapon.Laser);
		Bullet_Move();


		window.display();
	}
}