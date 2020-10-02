#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

using namespace sf;
using namespace std;

struct Bullet
{
	int damage;
	int direction;
	float speed = 7;
	float angle;
	Texture BulletTex;
	RectangleShape shape;
	

	void Convert_to_radian(float angle_in_degree)
	{
		angle = (angle_in_degree * 3.14159) / 180;
	}

	
};
extern vector<Bullet> bullet;