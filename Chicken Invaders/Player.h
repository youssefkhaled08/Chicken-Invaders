#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Weapon.h"

using namespace sf;
using namespace std;

extern int FPSDiffRatio;

struct Player
{
	Keyboard::Key moveUp, moveDown, moveLeft, moveRight, shootKey;
	float speed,limitedWeaponTime = 30;
	long long score = 0;
	int Lives = 0;
	RectangleShape body,ShieldBody;
	bool died = false,Exploded=false, secondry=false,right=false,left=false, shoot=false, one = true, Shield = false;
	string name;
	Weapon primaryWeapon, secondryWeapon;
	int ImagesCounter , numberOfImages=1 , singleImageWidth, ExplodeXCounter=0,ExplodeYCounter=0 , shieldTime = 5;
	float refreshRate;
	Clock clock,timer,dieclock,shootclock, LaserTimer, ShieldTimer;
	Texture Explosion,PlayerTexture,ShieldTex;
	SoundBuffer ExplosionSoundBuffer;
	Sound ExplosionSound;
	


	void Activate_Shield()
	{
		if (ShieldTimer.getElapsedTime().asSeconds() >= shieldTime)
		{
			Shield = false;
		}
		else
		{
			Shield = true;
		}

	}
	void move(float windowWidth, float windowHeight)
	{
		if (Shield)
		{
			Activate_Shield();
		}

		if (died && !Exploded)
		{
			Animate("die");
			return;
		}
		if (secondry && timer.getElapsedTime().asSeconds() >= limitedWeaponTime)
			secondry = false;
		
		
		if (clock.getElapsedTime().asSeconds() > refreshRate * 2)
		{
			Animate("idle");
			clock.restart();
		}

		if (Keyboard::isKeyPressed(moveUp) && body.getPosition().y > 0)
		{
			body.move(0.0f, -speed * FPSDiffRatio);
		}
		if (Keyboard::isKeyPressed(moveDown) && body.getPosition().y + body.getSize().y < windowHeight)
		{
			body.move(0.0f, speed * FPSDiffRatio);
		}
		if (Keyboard::isKeyPressed(moveLeft) && body.getPosition().x > 0)
		{
			if (right)
				Animate("idle");
			else
				Animate("left");
			body.move(-speed * FPSDiffRatio, 0.0f);
		}
		else if (Keyboard::isKeyPressed(moveRight) && body.getPosition().x + body.getSize().x < windowWidth)
		{
			if (left)
				Animate("idle");
			else
				Animate("right");
			body.move(speed * FPSDiffRatio, 0.0f);
		}
		else
		{
			Animate("Idle");
		}
		if (Shield)
		{
			ShieldBody.setSize(Vector2f(body.getSize().x+20, body.getSize().y+10));
			ShieldBody.setPosition(body.getPosition().x-10, body.getPosition().y-10);
			ShieldBody.setTexture(&ShieldTex);

		}
	}
	void can_shoot()
	{
		
		
		if (Keyboard::isKeyPressed(shootKey))
		{

			if (!secondry)
			{
				primaryWeapon.Shoot(body.getPosition().x + (body.getSize().x / 2), body.getPosition().y);
				
			}
			else
			{
				secondryWeapon.Shoot(body.getPosition().x + (body.getSize().x / 2) - 4, body.getPosition().y);
				shoot = secondryWeapon.fired;
			}
			

		}
		else
		{
			primaryWeapon.fired = false;
			secondryWeapon.fired = false;
		}
		

	}


	void die()
	{
		if (!Shield)
		{
			primaryWeapon.overHeat = 0;
			secondryWeapon.overHeat = 0;
			if (secondry)
				secondry = false;

			if (primaryWeapon.Level > 1)
				primaryWeapon.Level -= 1;

			died = true;
			Exploded = false;
			body.setTexture(&Explosion);
			//body.setTextureRect(IntRect(0, 0, Explosion.getSize().x / 5, Explosion.getSize().y / 5));
			if (SoundVFX)
			ExplosionSound.play();
			ExplodeXCounter = 0;
			ExplodeYCounter = 0;
			Animate("die");
			dieclock.restart();
			if (Lives == 0)
			{
				//call the score board (game over)
			}
			else
			{
				revive();
			}
		}
	}
	void revive()
	{
		if (Exploded && died && Lives)
		{
			body.setTexture(&PlayerTexture);
			body.setPosition(500, 800);
			shieldTime = 3;
			ShieldTimer.restart();
			Activate_Shield();
			died = false;
			Exploded = false;
			Lives--;
		}


	}
	void gain_score(float gained_score)
	{
		score += gained_score;

	}

	

	void Change_Weapon(Weapon newWeapon, bool limitOfWeapon = false)
	{
		primaryWeapon.overHeat = 0;
		secondryWeapon.overHeat = 0;
		if (!secondry)
		{
			if (newWeapon.Name == primaryWeapon.Name)
			{
				Improve_Weapon();
				return;
			}
		}
		else
		{
			if (newWeapon.Name == secondryWeapon.Name)
			{
				Improve_Weapon();
				timer.restart();
				return;
			}
		}

		if (limitOfWeapon == true)
		{
			timer.restart();
			secondry = true;
			secondryWeapon = newWeapon;
		}
		if (limitOfWeapon == false)
		{
			primaryWeapon = newWeapon;
			secondry = false;
		}
	}

	void Improve_Weapon()
	{
		primaryWeapon.overHeat = 0;
		secondryWeapon.overHeat = 0;
		if (!secondry)
		{
			if (primaryWeapon.Level < 4)
				primaryWeapon.Level++;
			else
				primaryWeapon.damage += 10;
		}
		else
		{
			if (secondryWeapon.Level < 4)
				secondryWeapon.Level++;
			else
				secondryWeapon.damage += 10;
		}
	}

	void Animate(string animation)
	{
		if (animation == "left")
		{
			if (clock.getElapsedTime().asSeconds() >= refreshRate && !right)
			{
				body.setTextureRect(IntRect(singleImageWidth * ImagesCounter, (int)(body.getTexture()->getSize().y / 5) * 4 + 10, singleImageWidth - 2, (int)(body.getTexture()->getSize().y / 5)));
				ImagesCounter++;
				if (ImagesCounter >= numberOfImages)
					ImagesCounter = 9;
				clock.restart();
				left = true;
			}
		}
		else if (animation == "right")
		{
			if (clock.getElapsedTime().asSeconds() >= refreshRate && !left)
			{
				body.setTextureRect(IntRect(singleImageWidth *(ImagesCounter+1) - 2, (int)(body.getTexture()->getSize().y / 5) * 4 + 10, -(singleImageWidth), (int)(body.getTexture()->getSize().y / 5)));
				
				ImagesCounter++;
				if (ImagesCounter >= numberOfImages)
					ImagesCounter = 9;
				clock.restart();
				right = true;
			}
		}
		else if (animation == "idle")
		{
			if (clock.getElapsedTime().asSeconds() >= refreshRate/2)
			{
				if (right)
				{
					body.setTextureRect(IntRect(singleImageWidth * (ImagesCounter+1) - 2, (int)(body.getTexture()->getSize().y / 5) * 4 + 10, -(singleImageWidth), (int)(body.getTexture()->getSize().y / 5)));
					
					ImagesCounter--;
					if (ImagesCounter <= 0)
					{
						ImagesCounter = 0;
						right = false;
					}
				}
				if (left)
				{
					body.setTextureRect(IntRect(singleImageWidth * ImagesCounter, (int)(body.getTexture()->getSize().y / 5) * 4 + 10, singleImageWidth - 2, (int)(body.getTexture()->getSize().y / 5)));
					
					ImagesCounter--;
					if (ImagesCounter <= 0)
					{
						ImagesCounter = 0;
						left = false;
					}
				}
				if (!left && !right)
				{
					body.setTextureRect(IntRect(singleImageWidth * ImagesCounter, (int)(body.getTexture()->getSize().y / 5) * 4 + 10, singleImageWidth - 2, (int)(body.getTexture()->getSize().y / 5)));
				}
				clock.restart();
			}
		}
		else if (animation == "die")
		{
			
			if (dieclock.getElapsedTime().asSeconds() >= 0.07)
			{
				body.setTextureRect(IntRect(ExplodeXCounter * Explosion.getSize().x / 5, ExplodeYCounter * Explosion.getSize().y / 5, Explosion.getSize().x / 5, Explosion.getSize().y / 5));
				ExplodeXCounter++;
				if (ExplodeXCounter >= 5)
				{
					ExplodeXCounter = 0;
					ExplodeYCounter++;
				}
				if (ExplodeYCounter >= 5)
				{
					Exploded = true;
					revive();
				}

				
				dieclock.restart();
			}
		}
	
	}



};