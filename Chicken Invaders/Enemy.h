#pragma once
#include<iostream>
#include<vector>
#include"Bullet.h"
using namespace sf;
using namespace std;

struct ScoreDrop
{
	RectangleShape body;
	Texture DropTex;
	int score=0;
	bool coin = false;
};
extern int FPSDiffRatio;
extern bool GiftChecker;
extern vector<Weapon> PrimaryWeapons,SecondryWeapons;
extern vector<ScoreDrop> coinsAndChicken;
extern vector<Texture> WeaponGiftTextures;
extern int giftsCounter;
extern int EnemiesSize;
extern int enemiesdied;
struct WeaponDrop
{
	RectangleShape body;
	bool levelUp=false;
	int Secondryindex = rand() % SecondryWeapons.size(), PrimaryIndex = rand()% PrimaryWeapons.size();
	Weapon SecondryGiftWeapon = SecondryWeapons[Secondryindex], PrimaryGiftWeapon = PrimaryWeapons[PrimaryIndex];
	int SecondryOrPrimary = rand() % 101;

};

extern vector<WeaponDrop> WeaponGifts;

struct enemyStructure
{
	bool Died = false;
	bool rightBorder = 0;

	int level = 1;
	int healthPoints = 100;
	int damage = 50;
	int speedCounter = 0;
	float speed=0.5f;

	float speedIncrease = 0;
	int imagesXcounter = 0;
	int imageYcounter = 3;
	Clock enemytimer;
	float nowtimer = 0.03;
	bool explosion = false;

	RectangleShape enemyShape;
	Texture enemyExplosion, eggTex;
	Clock AnimationTimer;
	int xCounter = 0;
	bool minus = false;
	Sound ChickenHit;
	//Wave 1 Variables
	int endPoint, startPoint, downposition, upPosition;
	bool down = 0;
	//Wave 2 Variables
	bool wave2level2test = true;
	bool wave1level2test = false;

	int lowery;
	int highery;
	int leftx;
	int rightx;

	bool higher_y = false;
	bool lower_y = false;
	bool leftedgetest = false;
	bool rightedgetest = false;
	bool bosslevel = true;
	//Waves 3 Variables
	float difference = 0;
	bool loop = false, TopLeft = false,TopRight = false,BottomRight = false,BottomLeft = false;
	float currectPosition;
	bool Boss = false;

	void move(float xEnemyPosition)
	{

		if (AnimationTimer.getElapsedTime().asSeconds() >= 0.15f && !explosion && !Boss)
		{
			enemyShape.setTextureRect(IntRect(xCounter*enemyShape.getTexture()->getSize().x/3,0, enemyShape.getTexture()->getSize().x/3, enemyShape.getTexture()->getSize().y));
			if (!minus)
			{
				xCounter++;
				if (xCounter >= 3)
				{
					xCounter-=2;
					minus = true;
				}
			}
			else
			{
				xCounter--;
				if (xCounter < 0)
				{
					xCounter += 2;
					minus = false;
				}
			}
			AnimationTimer.restart();
		}

		if (explosion)
		{
			enemyShape.setTexture(&enemyExplosion);
			animation();
		}

		if (xEnemyPosition >= endPoint)
		{
			rightBorder = 1;
			down = 1;
		}
		if (!rightBorder)
		{
			if(xEnemyPosition + (speed * FPSDiffRatio) <= endPoint)
				enemyShape.move(speed * FPSDiffRatio, 0.0f);
			else
				enemyShape.setPosition(endPoint, enemyShape.getPosition().y);

		}

		if (rightBorder)
			if (xEnemyPosition - (speed * FPSDiffRatio) >= startPoint)
				enemyShape.move(-speed * FPSDiffRatio, 0.0f);
			else
				enemyShape.setPosition(startPoint, enemyShape.getPosition().y);

		if (xEnemyPosition <= startPoint)
		{
			rightBorder = 0;
			down = 0;
			speedCounter++;
		}

		if (speedCounter == 5)
		{
			speedIncrease += 0.1;
			speedCounter = 0;
		}

	}
	void moveUPandDown(float yEnemyPosition)
	{

		if (explosion)
		{
			enemyShape.setTexture(&enemyExplosion);
			animation();
		}

		if (yEnemyPosition >= downposition)
		{
			down = 1;
		}
		if (!down)
			enemyShape.move(2.5f, speed * FPSDiffRatio);

		if (down)
			enemyShape.move(0.0f, -speed * FPSDiffRatio);

		if (yEnemyPosition <= upPosition)
		{

			down = 0;
			speedCounter++;
		}

		if (speedCounter == 5)
		{
			speedIncrease += 0.1;
			speedCounter = 0;
		}

	}
	void wave2Enteringmove(float yEnemyPosition)
	{

		if (explosion)
		{
			enemyShape.setTexture(&enemyExplosion);
			animation();
		}

		if (yEnemyPosition >= downposition)
		{
			down = 1;
		}
		if (!down)
			enemyShape.move(0.0f, speed * FPSDiffRatio);

		//	if (down)
				//enemyShape.move(0.0f, -speed * FPSDiffRatio);

		if (yEnemyPosition <= upPosition)
		{

			down = 0;
			speedCounter++;
		}

		if (speedCounter == 5)
		{
			speedIncrease += 0.1;
			speedCounter = 0;
		}

	}

	void shoot(float xEnemyPosition, float yEnemyPosition)
	{
		Bullet newEgg;
		newEgg.shape.setSize(Vector2f(25, 30));
		newEgg.shape.setPosition(Vector2f(xEnemyPosition, yEnemyPosition));
		newEgg.direction = 1;
		newEgg.Convert_to_radian(90);
		newEgg.speed = 1;
		newEgg.BulletTex = eggTex;
		newEgg.shape.setTexture(&newEgg.BulletTex);
		bullet.push_back(newEgg);
	}

	void take_Damage(int Damage)
	{
		healthPoints -= Damage;

		if (healthPoints <= 0)
		{
			enemiesdied++;
			explosion = true;
			ScoreDrop Coin;
			
			Coin.coin = true;
			ScoreDrop Chick;
		
			WeaponDrop Gift1;
			WeaponDrop Gift2;
			Gift2.levelUp = true;
			Coin.score = rand() % 1000 + 500;
			Chick.score = rand() % 100 + 50;
			//coins setup
			if (rand() % 30 > 20)
			{
				Coin.body.setPosition(Vector2f(enemyShape.getPosition().x, enemyShape.getPosition().y));
				Coin.body.setSize(Vector2f(25,25));
				Coin.body.setOrigin(Vector2f(12.5,12.5));
				//Coin.body.setFillColor(Color::Yellow);
				coinsAndChicken.push_back(Coin);
			}
			//chicken setup
			Chick.body.setPosition(Vector2f(enemyShape.getPosition().x + 50, enemyShape.getPosition().y));
			Chick.body.setSize(Vector2f(40, 40));
			Chick.body.setOrigin(Vector2f(15, 15));
			
			coinsAndChicken.push_back(Chick);
			//WeaponGift setup
			if (rand() % 30 > 20 && giftsCounter <= 1 && EnemiesSize < 20 && !GiftChecker)
			{
				Gift1.body.setPosition(Vector2f(enemyShape.getPosition().x + 70, enemyShape.getPosition().y + 20));
				Gift1.body.setSize(Vector2f(40,40));
				Gift1.body.setOrigin(Vector2f(20,20));
				if(Gift1.SecondryOrPrimary <= 50)
					Gift1.body.setTexture(&WeaponGiftTextures[Gift1.Secondryindex+4]);
				else
					Gift1.body.setTexture(&WeaponGiftTextures[Gift1.PrimaryIndex]);

				WeaponGifts.push_back(Gift1);
				giftsCounter++;
				GiftChecker = true;
			}
			if (rand() % 30 > 20 && giftsCounter <= 1 && EnemiesSize < 10 && GiftChecker)
			{
				Gift2.body.setPosition(Vector2f(enemyShape.getPosition().x + 70, enemyShape.getPosition().y + 50));
				Gift2.body.setSize(Vector2f(40, 40));
				Gift2.body.setOrigin(Vector2f(20, 20));
				
				WeaponGifts.push_back(Gift2);
				giftsCounter++;
			}

		}
		else
		{
			if(rand()%5 >= 2)
				if (SoundVFX)
					ChickenHit.play();
		}
	}

	void animation()
	{
		if (explosion)
		{
			if (enemytimer.getElapsedTime().asSeconds() > nowtimer)
			{
				imagesXcounter++;
				
				enemyShape.setTextureRect(IntRect(imagesXcounter * enemyExplosion.getSize().x / 8, imageYcounter * enemyExplosion.getSize().y / 6, enemyExplosion.getSize().x / 8, enemyExplosion.getSize().y / 6));
				if (imagesXcounter >= 7)
				{
					imageYcounter++;
					imagesXcounter = 0;
				}
				if (imageYcounter > 6)
				{
					Died = true;
				}
				enemytimer.restart();
			}
		}

	}
};
extern vector<enemyStructure> Enemies;