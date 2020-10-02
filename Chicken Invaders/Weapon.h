#pragma once
#include "Bullet.h"
#include <vector>

using namespace sf;
using namespace std;

extern bool SoundVFX;
	extern int FPSDiffRatio;
struct Weapon
{
	string type;
	int damage, Level;
	String Name;
	float rateOfFire, MaximumOverHeat, overHeat, BulletSpeed, HeatRate, CalmingRate, MaximumCalmingRate;
	bool fired = false, HeatCalmed = true;
	Clock rateOfFireClock;
	SoundBuffer BulletBuffer;
	Sound BulletSound;
	RectangleShape Laser;
	float LaserSize = 10;
	bool found = false, loopsound = false;
	int index = 0;
	Texture weaponTex;
	int xSize, ySize,rightAngle2,leftAngle2,rightAngle3,leftAngle3,rightAngle4,leftAngle4;

	void Shoot(float x, float y)
	{
		x -= 15;
		if (overHeat >= MaximumOverHeat)
			HeatCalmed = false;
		if (overHeat <= 0)
			HeatCalmed = true;

		if (HeatCalmed && rateOfFireClock.getElapsedTime().asSeconds() >= rateOfFire)
		{
			rateOfFireClock.restart();
			if (type == "Bullets")
			{
				if (Level == 1)
				{
					Bullet newBullet;
					newBullet.shape.setPosition(x, y);
					newBullet.shape.setSize(Vector2f(xSize, ySize));
					newBullet.direction = -1;
					newBullet.damage = damage;
					newBullet.speed = BulletSpeed;
					newBullet.Convert_to_radian(90);
					newBullet.BulletTex = weaponTex;
					//newBullet.shape.setFillColor(BulletColor);
					bullet.push_back(newBullet);
					overHeat += HeatRate;
					if (SoundVFX)
					BulletSound.play();
					fired = true;
				}
				else if (Level == 2)
				{
					Bullet newBullet;
					newBullet.shape.setPosition(x, y);
					newBullet.shape.setSize(Vector2f(xSize, ySize));
					newBullet.direction = -1;
					newBullet.damage = damage;
					newBullet.speed = BulletSpeed;
					newBullet.Convert_to_radian(leftAngle2);
					newBullet.BulletTex = weaponTex;
					//newBullet.shape.setFillColor(BulletColor);
					Bullet newBullet2 = newBullet;
					newBullet2.Convert_to_radian(rightAngle2);
					newBullet.shape.setPosition(x - 10, y);
					newBullet2.shape.setPosition(x + 10, y);
					newBullet.shape.rotate(-7);
					newBullet2.shape.rotate(7);

					bullet.push_back(newBullet);
					bullet.push_back(newBullet2);
					overHeat += HeatRate;
					if(SoundVFX)
						BulletSound.play();
					fired = true;
				}
				else if(Level == 3)
				{
					Bullet newBullet1, newBullet2, newBullet3;
					newBullet1.shape.setPosition(x, y);
					newBullet1.shape.setSize(Vector2f(xSize, ySize));
					newBullet1.direction = -1;
					newBullet1.damage = damage;
					newBullet1.speed = BulletSpeed;
					newBullet1.Convert_to_radian(90);
					newBullet1.BulletTex = weaponTex;
					//newBullet1.shape.setFillColor(BulletColor);
					newBullet2 = newBullet1;
					newBullet3 = newBullet1;
					newBullet2.shape.setPosition(x - 10, y);
					newBullet3.shape.setPosition(x + 10, y);
					newBullet2.Convert_to_radian(leftAngle3);
					newBullet3.Convert_to_radian(rightAngle3);
					newBullet2.shape.rotate(-10);
					newBullet3.shape.rotate(10);
					bullet.push_back(newBullet1);
					bullet.push_back(newBullet2);
					bullet.push_back(newBullet3);
					overHeat += HeatRate;
					if (SoundVFX)
					BulletSound.play();
					fired = true;
				}
				else
				{
					Bullet newBullet;
					newBullet.shape.setPosition(x-35, y);
					newBullet.shape.setSize(Vector2f(xSize, ySize));
					newBullet.direction = -1;
					newBullet.damage = damage;
					newBullet.speed = BulletSpeed;
					newBullet.Convert_to_radian(leftAngle4);
					newBullet.BulletTex = weaponTex;
					newBullet.shape.rotate(-5);
					Bullet newBullet2 = newBullet, newBullet3 = newBullet, newBullet4 = newBullet;
					newBullet3.Convert_to_radian(rightAngle4);
					newBullet3.shape.rotate(10);
					newBullet4 = newBullet3;
					newBullet2.shape.setPosition(x - 10, y);
					newBullet3.shape.setPosition(x + 10, y);
					newBullet4.shape.setPosition(x + 35, y);
					bullet.push_back(newBullet);
					bullet.push_back(newBullet2);
					bullet.push_back(newBullet3);
					bullet.push_back(newBullet4);
					overHeat += HeatRate;
					if (SoundVFX)
					BulletSound.play();
					fired = true;
				}
			}
			else if (type == "Laser")
			{
				if (Level == 1)
				{
					LaserSize = 10;
					damage = 3;
				}
				else if (Level == 2)
				{
					LaserSize = 20;
					damage = 5;
				}
				else if (Level == 3)
				{
					LaserSize = 30;
					damage = 7;
				}
				Laser.setPosition(x + 10, y);
				if (!found && -Laser.getSize().y <= y)
					Laser.setSize(Vector2f(LaserSize, Laser.getSize().y - 100));

				fired = true;
				overHeat += HeatRate;
			}
			else if (type == "Electericity")
			{
				if (Level == 1)
				{
					LaserSize = 30;
					damage = 2;
				}
				else if (Level == 2)
				{
					LaserSize = 35;
					damage = 7;
				}
				else if (Level == 3)
				{
					LaserSize = 40;
					damage = 10;
				}
				else
				{
					LaserSize = 40;
					damage = 15;
				}
				Laser.setPosition(x + 10, y);
				if (!found && -Laser.getSize().y <= y)
					Laser.setSize(Vector2f(LaserSize, Laser.getSize().y - 100));

				fired = true;
				if (!loopsound)
				{
					BulletSound.setLoop(true);
					if (SoundVFX)
					BulletSound.play();
					loopsound = true;
				}
				overHeat += HeatRate;
			}

		}
		

	}

	void Calming_Down()
	{
		overHeat -= MaximumCalmingRate * FPSDiffRatio;
	}

	

};