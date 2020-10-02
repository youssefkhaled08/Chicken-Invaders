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
#include <iomanip>
#include "Level1.h"
#include "startButton.h"
#include "players_window.h"
#include "LeaderBoard.h"

using namespace sf;
using namespace std;


RenderWindow window(VideoMode(1920, 1080), "Chicken Invaders" ,Style::Titlebar);

Weapon Plasma = { "Electericity",5,1,"Plasma",0.01,70,0,2,0.06,0.02,0.1 },Laser = { "Laser",5,1,"Laser",0.01,70,0,2,0.06,0.02,0.1 }, ionBlaster = { "Bullets",100,1,"ionBlaster",0.4,20,0,8,0.6,0.002,0.06 }, neutronGun = { "Bullets",30,1,"neutronGun",0.5,20,0,8,1,0.002,0.07 }, hypergun = { "Bullets",30,1,"hypergun",0.5,20,0,8,1,0.02,0.07 }, vulcangun = { "Bullets",30,1,"vulcangun",0.5,20,0,8,1,0.02,0.07 };

Person LeaderBoard[10];


//Main Variables
vector<enemyStructure> Enemies;
vector<ScoreDrop> coinsAndChicken;
vector<WeaponDrop> WeaponGifts;
vector<Texture> WeaponGiftTextures;
vector<Weapon> PrimaryWeapons, SecondryWeapons;
vector<Player> Players, PlayersForLeaderBoard;
int giftsCounter = 0;
int EnemiesSize;
Texture bulletTex;
//bool GiftChecker = false;
int FPS,FPSDiffRatio;
extern 
void Bullet_Move()
{
	for (int i = 0; i < bullet.size(); i++)
	{
		Vector2f bulletPosition = bullet[i].shape.getPosition();
		bullet[i].speed *= FPSDiffRatio;
		bullet[i].shape.move(bullet[i].speed * cos(bullet[i].angle), bullet[i].speed * bullet[i].direction * sin(bullet[i].angle));
		bullet[i].shape.setTexture(&bullet[i].BulletTex);
		bullet[i].speed /= FPSDiffRatio;
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

void Laser_Check()
{
	for (int x = 0; x < Players.size(); x++)
	{
		if (!Players[x].Exploded && !Players[x].died)
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
					if (Players[x].LaserTimer.getElapsedTime().asSeconds() >= 0.1)
						if (Players[x].one)
						{
							Players[x].primaryWeapon.Laser.setTextureRect(IntRect(Players[x].primaryWeapon.Laser.getTexture()->getSize().x, Players[x].primaryWeapon.Laser.getTexture()->getSize().y, -124, -512));
							Players[x].one = false;
							if (Players[x].primaryWeapon.Laser.getRotation() > 353 || Players[x].primaryWeapon.Laser.getRotation() < 15)
								Players[x].primaryWeapon.Laser.rotate(-rand() % 7);
							Players[x].LaserTimer.restart();
						}
						else
						{
							Players[x].primaryWeapon.Laser.setTextureRect(IntRect(0, 0, Players[x].primaryWeapon.Laser.getTexture()->getSize().x, Players[x].primaryWeapon.Laser.getTexture()->getSize().y));
							Players[x].one = true;
							if (Players[x].primaryWeapon.Laser.getRotation() > 345 || Players[x].primaryWeapon.Laser.getRotation() < 5)
								Players[x].primaryWeapon.Laser.rotate(rand() % 7);
							Players[x].LaserTimer.restart();

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
					if (Players[x].LaserTimer.getElapsedTime().asSeconds() >= 0.1)
						if (Players[x].one)
						{
							Players[x].secondryWeapon.Laser.setTextureRect(IntRect(Players[x].secondryWeapon.Laser.getTexture()->getSize().x, Players[x].secondryWeapon.Laser.getTexture()->getSize().y, -124, -512));
							Players[x].one = false;
							if (Players[x].secondryWeapon.Laser.getRotation() > 353 || Players[x].secondryWeapon.Laser.getRotation() < 15)
								Players[x].secondryWeapon.Laser.rotate(-rand() % 7);
							Players[x].LaserTimer.restart();
						}
						else
						{
							Players[x].secondryWeapon.Laser.setTextureRect(IntRect(0, 0, Players[x].secondryWeapon.Laser.getTexture()->getSize().x, Players[x].secondryWeapon.Laser.getTexture()->getSize().y));
							Players[x].one = true;
							if (Players[x].secondryWeapon.Laser.getRotation() > 345 || Players[x].secondryWeapon.Laser.getRotation() < 5)
								Players[x].secondryWeapon.Laser.rotate(rand() % 7);
							Players[x].LaserTimer.restart();

						}
				}
			}
		}
	}
}


// pause 


Music musicSoundEffect;
SoundBuffer musicBuffer;
vector<Bullet> bullet;
//float EnemyRateOfFire = 0.5;
Clock EnemyTimer;
Clock FPSClock;
Texture ChickenTex;
Texture EnemyExplosion, EggTex, CoinTex, ChickTex;
Texture playerTex, LaserTex , PlasmaTex;
Texture UpgradeTex;
RectangleShape Background(Vector2f(window.getSize().x, window.getSize().y));
Texture BackgroundTexture;
Texture ionBlasterGift, neutronGunGift, hyperGunGift,vulcanGunGift,PlasmaGift,LaserGift;
Texture Button1, Button2, XButton1, XButton2, CheckBoxChecked, CheckBoxUnChecked;
Texture BarBack, BarBlue, BarOrange, BarGreen, BarCyan, BarPurble, BarRed, BarPink, UIBackTex;
Texture CursorTex;
Texture BackgroundLevel1, BackgroundLevel2, BackgroundLevel3;
Texture MidEggCrack, HighEggCrack;
bool SoundVFX = true, MusicVFX = false;
SoundBuffer ButtonBuffer;

bool pause()
{
	window.setMouseCursorVisible(true);
	bool screen1 = true;
	bool screen2 = false;
	RectangleShape pause_back_ground;
	Texture pausebackground;
	pausebackground.loadFromFile("Window.png");
	pause_back_ground.setTexture(&pausebackground);
	pause_back_ground.setOrigin(200, 200);
	pause_back_ground.setSize(Vector2f(400, 400));
	pause_back_ground.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	Texture buttontexture;
	buttontexture.loadFromFile("Button1.png");

	
	Button resume;
	resume.button.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 - 100);
	resume.btnHeight = 60;
	resume.btnWidth = 220;
	resume.button.setSize(Vector2f(resume.btnWidth, resume.btnHeight));
	resume.button.setTexture(&Button1);
	resume.ButtonSound.setBuffer(ButtonBuffer);

	Font pausefont;
	pausefont.loadFromFile("FunSized.ttf");
	Text resumetext;
	resumetext.setString("resume");
	resumetext.setFont(pausefont);
	resumetext.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 - 90);

	Button options;
	options.button.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2);
	options.btnHeight = 60;
	options.btnWidth = 220;
	options.button.setSize(Vector2f(options.btnWidth, options.btnHeight));
	options.button.setTexture(&Button1);
	options.ButtonSound.setBuffer(ButtonBuffer);

	Text optiontext;
	optiontext.setString("options");
	optiontext.setFont(pausefont);
	optiontext.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 + 10);


	Button back;
	back.btnHeight = 60;
	back.btnWidth = 220;
	back.button.setSize(Vector2f(back.btnWidth, back.btnHeight));
	back.button.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 100);

	back.button.setTexture(&buttontexture);
	Text backtext;
	backtext.setString("back");
	backtext.setFont(pausefont);
	backtext.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 + 110);

	Button back_to_menue;
	back_to_menue.btnHeight = 60;
	back_to_menue.btnWidth = 220;
	back_to_menue.button.setSize(Vector2f(back_to_menue.btnWidth, back_to_menue.btnHeight));
	back_to_menue.button.setPosition(window.getSize().x / 2 - 100, window.getSize().y / 2 + 100);
	back_to_menue.button.setTexture(&buttontexture);
	Text menuetext;
	menuetext.setString("give up");
	menuetext.setFont(pausefont);
	menuetext.setPosition(window.getSize().x / 2 - 60, window.getSize().y / 2 + 110);
	Text sound;
	sound.setString("Sound");
	sound.setFont(pausefont);
	sound.setCharacterSize(45);
	sound.setFillColor(Color::White);
	sound.Bold;
	sound.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 120);

	Button soundCheckBox;
	soundCheckBox.button.setSize(Vector2f(50, 50));
	soundCheckBox.button.setPosition(window.getSize().x / 4 + 600, window.getSize().y / 2 - 117);
	soundCheckBox.button.setTexture(&CheckBoxChecked);
	soundCheckBox.btnWidth = 50;
	soundCheckBox.btnHeight = 50;
	soundCheckBox.CheckBox = true;

	Text music;

	music.setString("Music");
	music.setFont(pausefont);
	music.setCharacterSize(45);
	music.setFillColor(Color::White);
	music.Bold;
	music.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 40);

	Button musicCheckBox;
	musicCheckBox.button.setSize(Vector2f(50, 50));
	musicCheckBox.button.setPosition(window.getSize().x / 4 + 600, window.getSize().y / 2 - 37);
	musicCheckBox.button.setTexture(&CheckBoxUnChecked);
	musicCheckBox.btnWidth = 50;
	musicCheckBox.btnHeight = 50;
	musicCheckBox.CheckBox = true;


	bool exit = false;

	while (true)
	{
		Event event;
		window.clear();
		window.draw(Background);
		window.draw(pause_back_ground);

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::MouseMoved:
				{
				if (screen1)
				{
					resume.IsMouseOver();
					options.IsMouseOver();
					back_to_menue.IsMouseOver();
				}
				else
				{
					back.IsMouseOver();
				}
				break;
				}

			case Event::MouseButtonPressed:
			{
				if (screen1 && resume.IsMouseOver() )
				{
					exit = true;
					break;
				}
				else if (screen1 && options.IsMouseOver())
				{
					screen1 = false;
					screen2 = true;
					continue;
				}
				else if (screen1 && back_to_menue.IsMouseOver())
				{
					return 0;
				}
				else if (screen2 && back.IsMouseOver())
				{
					screen1 = true;
					screen2 = false;
				}
				else if (soundCheckBox.IsMouseOver() && SoundVFX == false)
				{
					SoundVFX = true;
					soundCheckBox.button.setTexture(&CheckBoxChecked);
				}
				else if (soundCheckBox.IsMouseOver() && SoundVFX == true)
				{
					SoundVFX = false;
					soundCheckBox.button.setTexture(&CheckBoxUnChecked);
				}else if (musicCheckBox.IsMouseOver() && MusicVFX == false)
				{
						MusicVFX = true;
						musicCheckBox.button.setTexture(&CheckBoxChecked);
						musicSoundEffect.play();
						musicSoundEffect.setLoop(true);

				}
				else if (musicCheckBox.IsMouseOver() && MusicVFX == true)
				{
					
						MusicVFX = false;
						musicCheckBox.button.setTexture(&CheckBoxUnChecked);
						musicSoundEffect.stop();
					


				}
				break;
			}
			}
		}
		


		if (exit)
			break;

		if (screen1 == true)
		{
			resume.IsMouseOver();
			options.IsMouseOver();
			back_to_menue.IsMouseOver();
			window.draw(resume.button);
			window.draw(resumetext);
			window.draw(options.button);
			window.draw(optiontext);
			window.draw(back_to_menue.button);
			window.draw(menuetext);


		}
		else if (screen2 == true)
		{
			back.IsMouseOver();
			window.draw(sound);
			window.draw(soundCheckBox.button);
			window.draw(music);
			window.draw(musicCheckBox.button);
			window.draw(back.button);
			window.draw(backtext);
		}



		window.display();


	}
	return 1;
}

int main()
{
	window.setFramerateLimit(100);
	window.setMouseCursorVisible(false);
	RectangleShape Cursor(Vector2f(25,90));
	Cursor.rotate(-50);
	CursorTex.loadFromFile("Cursor.png");
	Cursor.setTexture(&CursorTex);
	//Loading Textures
	window.setFramerateLimit(100);
	ChickenTex.loadFromFile("Chicken Sprite.png");
	EnemyExplosion.loadFromFile("smoke.png");
	EggTex.loadFromFile("egg.png");
	CoinTex.loadFromFile("Coin.png");
	ChickTex.loadFromFile("Chick.png");
	playerTex.loadFromFile("SpaceShipAnimated.png");
	LaserTex.loadFromFile("laser_purple.png");
	PlasmaTex.loadFromFile("PlasmaRifle.png");
	UpgradeTex.loadFromFile("CI5Atom.png");
	BackgroundTexture.loadFromFile("SpaceBackground.jpg");
	ionBlasterGift.loadFromFile("GIFTIonBlaster.png");
	neutronGunGift.loadFromFile("GIFTNeutronGun.png");
	hyperGunGift.loadFromFile("GIFTHypergun.png");
	vulcanGunGift.loadFromFile("GIFTVulcanChaingun.png");
	PlasmaGift.loadFromFile("GIFTPlasmaRifle.png");
	LaserGift.loadFromFile("GIFTLightningFryer.png");
	Button1.loadFromFile("Button1.png");
	Button2.loadFromFile("Button2.png");
	BarBack.loadFromFile("ProgressBarBacground.png");
	BarBlue.loadFromFile("ProgressBar Bar Blue.png");
	BarOrange.loadFromFile("ProgressBar Bar Orange and Purple.png");
	BarCyan.loadFromFile("ProgressBar Bar Green and Cyan.png");
	BarGreen.loadFromFile("ProgressBar Bar Green.png");
	BarPurble.loadFromFile("ProgressBar Bar Purple.png");
	BarRed.loadFromFile("ProgressBar Bar Red.png");
	BarPink.loadFromFile("ProgressBar Bar Pink.png");
	UIBackTex.loadFromFile("UI Back.png");
	XButton1.loadFromFile("XButton1.png");
	XButton2.loadFromFile("XButton2.png");
	CheckBoxChecked.loadFromFile("CheckBox Checked.png");
	CheckBoxUnChecked.loadFromFile("CheckBox Unchecked.png");
	BackgroundLevel1.loadFromFile("Level1Background.jpg");
	BackgroundLevel2.loadFromFile("Level2Background.jpg");
	BackgroundLevel3.loadFromFile("Level3Background.jpg");
	musicSoundEffect.openFromFile("Music.wav");
	MidEggCrack.loadFromFile("egg mid crack.png");
	HighEggCrack.loadFromFile("egg high crack.png");
	//Background Setup
	Texture MenuBackground;
	MenuBackground.loadFromFile("MenuBackGround.jpg");
	Background.setTexture(&MenuBackground);


	//Players Setup
	Player player1 = { Keyboard::Key::W,Keyboard::Key::S,Keyboard::Key::A,Keyboard::Key::D,Keyboard::Key::Space },
		player2 = { Keyboard::Key::Up,Keyboard::Key::Down,Keyboard::Key::Left,Keyboard::Key::Right,Keyboard::Key::RShift };

	
	


	//Weapons Setup
	ionBlaster.weaponTex.loadFromFile("ion blaster single.png");
	ionBlaster.xSize = 25;
	ionBlaster.ySize = 100;
	ionBlaster.rightAngle2 = 80;
	ionBlaster.leftAngle2 = 100;
	ionBlaster.rightAngle3 = 80;
	ionBlaster.leftAngle3 = 100;
	ionBlaster.rightAngle4 = 85;
	ionBlaster.leftAngle4 = 95;
	SoundBuffer ionBlasterBuffer;
	ionBlasterBuffer.loadFromFile("ionBlaster.wav");
	ionBlaster.BulletSound.setBuffer(ionBlasterBuffer);

	neutronGun.weaponTex.loadFromFile("neutron gun medium.png");
	neutronGun.xSize = 25;
	neutronGun.ySize = 100;
	neutronGun.rightAngle2 = 80;
	neutronGun.leftAngle2 = 100;
	neutronGun.rightAngle3 = 70;
	neutronGun.leftAngle3 = 110;
	neutronGun.rightAngle4 = 85;
	neutronGun.leftAngle4 = 95;
	SoundBuffer neutronGunBuffer;
	neutronGunBuffer.loadFromFile("neutronGun.wav");
	neutronGun.BulletSound.setBuffer(neutronGunBuffer);

	hypergun.weaponTex.loadFromFile("hyper gun weak.png");
	hypergun.xSize = 25;
	hypergun.ySize = 100;
	hypergun.rightAngle2 = 85;
	hypergun.leftAngle2 = 95;
	hypergun.rightAngle3 = 75;
	hypergun.leftAngle3 = 105;
	hypergun.rightAngle4 = 85;
	hypergun.leftAngle4 = 95;
	SoundBuffer hypergunBuffer;
	hypergunBuffer.loadFromFile("hypergun.wav");
	hypergun.BulletSound.setBuffer(hypergunBuffer);

	vulcangun.weaponTex.loadFromFile("SuperVulcanChaingunWeak.png");
	vulcangun.xSize = 25;
	vulcangun.ySize = 100;
	vulcangun.rightAngle2 = 85;
	vulcangun.leftAngle2 = 95;
	vulcangun.rightAngle3 = 85;
	vulcangun.leftAngle3 = 95;
	vulcangun.rightAngle4 = 85;
	vulcangun.leftAngle4 = 95;
	SoundBuffer vulcangunBuffer;
	vulcangunBuffer.loadFromFile("vulcangun.wav");
	vulcangun.BulletSound.setBuffer(vulcangunBuffer);

	SoundBuffer PlasmaBuffer;
	PlasmaBuffer.loadFromFile("Plasma.wav");
	Plasma.Laser.setTexture(&PlasmaTex);
	Plasma.BulletSound.setBuffer(PlasmaBuffer);
	Laser.Laser.setTexture(&LaserTex);
	Laser.BulletSound.setBuffer(PlasmaBuffer);


	//PrimaryWeapons
	PrimaryWeapons.push_back(ionBlaster);
	PrimaryWeapons.push_back(neutronGun);
	PrimaryWeapons.push_back(hypergun);
	PrimaryWeapons.push_back(vulcangun);

	//SecondryWeapons
	SecondryWeapons.push_back(Plasma);
	SecondryWeapons.push_back(Laser);

	//PrimaryWeapons
	WeaponGiftTextures.push_back(ionBlasterGift);
	WeaponGiftTextures.push_back(neutronGunGift);
	WeaponGiftTextures.push_back(hyperGunGift);
	WeaponGiftTextures.push_back(vulcanGunGift);

	//SecondryWeapons
	WeaponGiftTextures.push_back(PlasmaGift);
	WeaponGiftTextures.push_back(LaserGift);

	//Player 1 PreSetup
	{
		player1.body.setSize(Vector2f(90.0f, 115.0f));
		player1.body.setTexture(&playerTex);
		//player1.body.setFillColor(Color::Cyan);
		player1.body.setPosition(700, 800);
		player1.primaryWeapon = PrimaryWeapons[0];
		player1.speed = 3.5f;
		player1.primaryWeapon.LaserSize = 30;
		player1.Lives = 3;
		player1.name = "Player 1";
		player1.refreshRate = 0.02f;
		player1.numberOfImages = 10;
		player1.ImagesCounter = 0;
		player1.singleImageWidth = playerTex.getSize().x / 10;
		player1.Explosion.loadFromFile("Explosion.png");
		player1.PlayerTexture = playerTex;
		player1.ExplosionSoundBuffer.loadFromFile("Explosion+3.wav");
		player1.ShieldTex.loadFromFile("Shield.png");
		player1.ExplosionSound.setBuffer(player1.ExplosionSoundBuffer);
		Players.push_back(player1);
	}
	//Player 2 PreSetup
	{
		player2.body.setSize(Vector2f(90.0f, 115.0f));
		player2.body.setTexture(&playerTex);
		player2.body.setPosition(300, 800);
		player2.primaryWeapon = PrimaryWeapons[1];
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
		player2.ShieldTex.loadFromFile("Shield.png");
		player2.ExplosionSound.setBuffer(player2.ExplosionSoundBuffer);
	}


	Font MenuFont;
	MenuFont.loadFromFile("CollegiateBlackFLF.ttf");
	
	ButtonBuffer.loadFromFile("ButtonClick.wav");
	Texture WindowTex;
	WindowTex.loadFromFile("Window.png");
	//******************************MENU Setup***********************************
	
	bool SoloMenu = false, SoundMenu = false;
	RectangleShape soloORdouWindow;
	soloORdouWindow.setSize(Vector2f(400, 470));
	soloORdouWindow.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 - 200);
	soloORdouWindow.setTexture(&WindowTex);
	RectangleShape wallOfLegendsWindow;
	wallOfLegendsWindow.setSize(Vector2f(750, 1000));
	wallOfLegendsWindow.setPosition(window.getSize().x / 2 - 375, window.getSize().y / 2 - 500);
	wallOfLegendsWindow.setTexture(&WindowTex);
	RectangleShape NewGameWindow;
	NewGameWindow.setSize(Vector2f(750, 400));
	NewGameWindow.setPosition(window.getSize().x / 2 - 375, window.getSize().y / 2 - 200);
	NewGameWindow.setTexture(&WindowTex);
	bool NewGameMenu = false;
	RectangleShape TextBox;
	TextBox.setSize(Vector2f(600, 60));
	TextBox.setPosition(window.getSize().x / 2 - 300, window.getSize().y / 2 - 140);
	TextBox.setTexture(&WindowTex);

	Button Xbutton;
	Xbutton.button.setSize(Vector2f(250, 100));
	Xbutton.button.setPosition(window.getSize().x / 2 - 125, window.getSize().y / 2 + 105);
	Xbutton.button.setTexture(&Button1);
	Xbutton.btnWidth = 250;
	Xbutton.btnHeight = 100;
	Xbutton.ButtonSound.setBuffer(ButtonBuffer);

	Button Xbutton2;
	Xbutton2.button.setSize(Vector2f(250, 100));
	Xbutton2.button.setPosition(window.getSize().x / 2 - 125, window.getSize().y / 2 + 350);
	Xbutton2.button.setTexture(&Button1);
	Xbutton2.btnWidth = 250;
	Xbutton2.btnHeight = 100;
	Xbutton2.ButtonSound.setBuffer(ButtonBuffer);

	Button Xbutton3;
	Xbutton3.button.setSize(Vector2f(370, 100));
	Xbutton3.button.setPosition(window.getSize().x / 2 - 185, 610);
	Xbutton3.button.setTexture(&Button1);
	Xbutton3.btnWidth = 250;
	Xbutton3.btnHeight = 100;
	Xbutton3.ButtonSound.setBuffer(ButtonBuffer);

	Text solo;
	Font No_playersfont;
	No_playersfont.loadFromFile("CollegiateBlackFLF.ttf");
	solo.setString("Solo");
	solo.setFont(No_playersfont);
	solo.setCharacterSize(45);
	solo.setFillColor(Color::White);
	solo.Bold;
	solo.setPosition(window.getSize().x / 2 - 53, window.getSize().y / 2 - 120);

	Text duo;
	duo.setString("Duo");
	duo.setFont(No_playersfont);
	duo.setCharacterSize(45);
	duo.setFillColor(Color::White);
	duo.Bold;
	duo.setPosition(window.getSize().x / 2 - 42, window.getSize().y / 2);

	Text Back;
	Back.setString("Back");
	Back.setFont(No_playersfont);
	Back.setCharacterSize(45);
	Back.setFillColor(Color::White);
	Back.Bold;
	Back.setPosition(window.getSize().x / 2 - 55, window.getSize().y / 2 + 125);


	Text Back2;
	Back2.setString("Back");
	Back2.setFont(No_playersfont);
	Back2.setCharacterSize(45);
	Back2.setFillColor(Color::White);
	Back2.Bold;
	Back2.setPosition(window.getSize().x / 2 - 55, 915);


	Text Back3;
	Back3.setString("Back");
	Back3.setFont(No_playersfont);
	Back3.setCharacterSize(45);
	Back3.setFillColor(Color::White);
	Back3.Bold;
	Back3.setPosition(window.getSize().x / 2 - 55, window.getSize().y / 2 + 90);


	Text sound, music;
	sound.setString("Sound");
	sound.setFont(No_playersfont);
	sound.setCharacterSize(45);
	sound.setFillColor(Color::White);
	sound.Bold;
	sound.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 120);

	music.setString("Music");
	music.setFont(No_playersfont);
	music.setCharacterSize(45);
	music.setFillColor(Color::White);
	music.Bold;
	music.setPosition(window.getSize().x / 2 - 150, window.getSize().y / 2 - 40);




	Button soloCheckBox, duoCheckBox;
	soloCheckBox.button.setSize(Vector2f(250, 100));
	soloCheckBox.button.setPosition(window.getSize().x / 2 - 125, window.getSize().y / 2 - 140);
	soloCheckBox.btnWidth = 250;
	soloCheckBox.btnHeight = 100;
	soloCheckBox.button.setTexture(&Button1);
	soloCheckBox.ButtonSound.setBuffer(ButtonBuffer);
	duoCheckBox.button.setSize(Vector2f(250, 100));
	duoCheckBox.button.setPosition(window.getSize().x / 2 - 125, window.getSize().y / 2 - 20);
	duoCheckBox.btnWidth = 250;
	duoCheckBox.btnHeight = 100;
	duoCheckBox.button.setTexture(&Button1);
	duoCheckBox.ButtonSound.setBuffer(ButtonBuffer);

	Button soundCheckBox;
	soundCheckBox.button.setSize(Vector2f(50, 50));
	soundCheckBox.button.setPosition(window.getSize().x / 4 + 600, window.getSize().y / 2 - 117);
	soundCheckBox.button.setTexture(&CheckBoxChecked);
	soundCheckBox.btnWidth = 50;
	soundCheckBox.btnHeight = 50;
	soundCheckBox.CheckBox = true;

	Button musicCheckBox;
	musicCheckBox.button.setSize(Vector2f(50, 50));
	musicCheckBox.button.setPosition(window.getSize().x / 4 + 600, window.getSize().y / 2 - 37);
	musicCheckBox.button.setTexture(&CheckBoxUnChecked);
	musicCheckBox.btnWidth = 50;
	musicCheckBox.btnHeight = 50;
	musicCheckBox.CheckBox = true;


	Button start;
	Text startText;
	startText.setFont(MenuFont);
	startText.setPosition(915, 580);
	startText.setString("Start");
	start.button.setSize(Vector2f(370, 100));
	start.button.setPosition(window.getSize().x / 2 - 185, 550);
	start.button.setTexture(&Button1);
	start.ButtonSound.setBuffer(ButtonBuffer);
	start.btnWidth = 370;
	start.btnHeight = 100;

	
	Button options;
	Text optionText;
	optionText.setFont(MenuFont);
	optionText.setPosition(905, 700);
	optionText.setString("Options");
	options.button.setSize(Vector2f(370, 100));
	options.button.setPosition(window.getSize().x /2 - 185, 670);
	options.button.setTexture(&Button1);
	options.btnWidth = 370;
	options.btnHeight = 100;
	options.ButtonSound.setBuffer(ButtonBuffer);

	Button StartGame;
	Text StartGameText;
	StartGameText.setFont(MenuFont);
	StartGameText.setPosition(830, 530);
	StartGameText.setString("Save The World");
	StartGame.button.setSize(Vector2f(370, 100));
	StartGame.button.setPosition(window.getSize().x / 2 - 185, 500);
	StartGame.button.setTexture(&Button1);
	StartGame.btnWidth = 370;
	StartGame.btnHeight = 100;
	StartGame.ButtonSound.setBuffer(ButtonBuffer);

	Button Leaderboard;
	Text LeaderboardText;
	LeaderboardText.setFont(MenuFont);
	LeaderboardText.setPosition(830, 820);
	LeaderboardText.setString("Wall Of Legends");
	Leaderboard.button.setSize(Vector2f(370, 100));
	Leaderboard.button.setPosition(window.getSize().x / 2 - 185, 790);
	Leaderboard.button.setTexture(&Button1);
	Leaderboard.btnWidth = 370;
	Leaderboard.btnHeight = 100;
	Leaderboard.ButtonSound.setBuffer(ButtonBuffer);

	Button quit;
	Text quitText;
	quitText.setFont(MenuFont);
	quitText.setPosition(115, 1000);
	quitText.setString("Quit");
	quit.button.setSize(Vector2f(200, 75));
	quit.button.setPosition(50, 980);
	quit.button.setTexture(&Button1);
	quit.btnWidth = 200;
	quit.btnHeight = 100;
	quit.ButtonSound.setBuffer(ButtonBuffer);

	Texture GameName;
	GameName.loadFromFile("Logo.png");
	RectangleShape gamename;
	gamename.setTexture(&GameName);
	gamename.setSize(Vector2f(900, 600));
	gamename.setPosition(window.getSize().x / 2 - 450, window.getSize().y/2 - 540);
	//window.clear();
	
	bool LeaderboardMenu = false;
	window.setKeyRepeatEnabled(false);

	fstream leaderboardFile("LeaderBoard.txt");
	for (int i = 0; i < 10 && !leaderboardFile.eof(); i++)
	{
		string name; int score;
		leaderboardFile >> LeaderBoard[i].name >> LeaderBoard[i].score;
	}
	leaderboardFile.close();

	Sort_Scores();

	sf::Text text;
	std::string playerinput;
	sf::Text playertext;
	playertext.setCharacterSize(45);
	playertext.setPosition(700,400);
	playertext.setFont(MenuFont);

	while (window.isOpen())
	{
		
		Event evnt;
		while (window.pollEvent(evnt))
		{
			Cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
			switch (evnt.type)
			{
			case Event::Closed:
				window.close();
				break;

			case Event::MouseMoved:
				if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && start.IsMouseOver())
				{
				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && quit.IsMouseOver())
				{
					break;
				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && options.IsMouseOver())
				{
					break;
				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && Leaderboard.IsMouseOver())
				{
					break;
				}
				else if (SoloMenu && (soloCheckBox.IsMouseOver() ||duoCheckBox.IsMouseOver() || Xbutton.IsMouseOver()))
				{
					break;
				}
				else if (SoundMenu && Xbutton.IsMouseOver())
				{
					break;
				}	
				else if (LeaderboardMenu && Xbutton2.IsMouseOver())
				{
					break;
				}
				else if (NewGameMenu && (Xbutton3.IsMouseOver() || StartGame.IsMouseOver()))
				{
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && start.IsMouseOver())
				{
					SoloMenu = true;
					continue;
				}
				else if ((Xbutton.IsMouseOver() || Xbutton2.IsMouseOver() || Xbutton3.IsMouseOver())&&  (SoloMenu || SoundMenu || NewGameMenu || LeaderboardMenu))
				{
					SoloMenu = false;
					SoundMenu = false;
					LeaderboardMenu = false;
					NewGameMenu = false;
					continue;

				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && options.IsMouseOver())
				{

					SoundMenu = true;
					continue;
				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && Leaderboard.IsMouseOver())
				{

					LeaderboardMenu = true;
					continue;
				}
				else if (!NewGameMenu && !SoloMenu && !SoundMenu && !LeaderboardMenu && quit.IsMouseOver())
				{
					window.close();
				}
				if (SoloMenu)
				{
					if (soloCheckBox.IsMouseOver())
					{
						Players.clear();
						Players.push_back(player1);
						NewGameMenu = true;
						SoloMenu = false;
						SoundMenu = false;
						LeaderboardMenu = false;
					}
					else if (duoCheckBox.IsMouseOver())
					{
						Players.clear();
						Players.push_back(player1);
						Players.push_back(player2);
						NewGameMenu = true;
						SoloMenu = false;
						SoundMenu = false;
						LeaderboardMenu = false;
					}
				}
				else if (SoundMenu)
				{
					if (soundCheckBox.IsMouseOver() && SoundVFX == false)
					{
						SoundVFX = true;
						soundCheckBox.button.setTexture(&CheckBoxChecked);



					}
					else if (soundCheckBox.IsMouseOver() && SoundVFX == true)
					{
						SoundVFX = false;
						soundCheckBox.button.setTexture(&CheckBoxUnChecked);

					}

					if (musicCheckBox.IsMouseOver() && MusicVFX == false)
					{
						MusicVFX = true;
						musicCheckBox.button.setTexture(&CheckBoxChecked);
						musicSoundEffect.play();
						musicSoundEffect.setLoop(true);


					}
					else if (musicCheckBox.IsMouseOver() && MusicVFX == true)
					{
						MusicVFX = false;
						musicCheckBox.button.setTexture(&CheckBoxUnChecked);
						musicSoundEffect.stop();


					}
				}
				else if (LeaderboardMenu)
				{
					if (Xbutton2.IsMouseOver())
						LeaderboardMenu = false;
				}
				else if (NewGameMenu && playerinput.size() > 2)
				{
					for (int i = 0; i < Players.size(); i++)
					{
						Players[i].name = playerinput;
					}
					if (StartGame.IsMouseOver())
					{
						NewGameMenu = false;
						Wave_1_Level_1();
					}


				}
				break;
			case sf::Event::TextEntered:
				if (NewGameMenu)
				{
					if (evnt.text.unicode < 128 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace))
					{
						if (playerinput.size() < 18)
						{
							playerinput += evnt.text.unicode;
							playertext.setString(playerinput);
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace) && !playerinput.empty())
					{
						playerinput.pop_back();
						playertext.setString(playerinput);
					}
				}
			}


		}
		Background.setTexture(&MenuBackground);
		Background.setTextureRect(IntRect(0, 0, MenuBackground.getSize().x, MenuBackground.getSize().y));
		window.draw(Background);
		window.draw(start.button);
		window.draw(startText);
		window.draw(quit.button);
		window.draw(quitText);
		window.draw(options.button);
		window.draw(optionText);
		window.draw(Leaderboard.button);
		window.draw(LeaderboardText);
		window.draw(gamename);

		if (SoloMenu)
		{
			window.draw(soloORdouWindow);
			window.draw(Xbutton.button);
			window.draw(Back);
			window.draw(duoCheckBox.button);
			window.draw(soloCheckBox.button);
			window.draw(solo);
			window.draw(duo);
		}
		else if (SoundMenu)
		{
			window.draw(soloORdouWindow);
			window.draw(Xbutton.button);
			window.draw(Back);
			window.draw(sound);
			window.draw(music);
			window.draw(musicCheckBox.button);
			window.draw(soundCheckBox.button);
			window.draw(music);


		}
		else if (LeaderboardMenu)
		{
			window.draw(wallOfLegendsWindow);
			for (int i = 0; i < 10; i++)
			{
				Text FirstName, FirstScore;
				stringstream score;
				score << LeaderBoard[i].score << setw(10);
				FirstName.setFont(MenuFont);
				FirstScore.setFont(MenuFont);
				FirstName.setString(LeaderBoard[i].name);
				FirstScore.setString(score.str());
				FirstName.setPosition(650, 100+(i*80));
				FirstScore.setPosition(1000, 100 + (i * 80));
				window.draw(FirstName);
				window.draw(FirstScore);
				
			}
			window.draw(Xbutton2.button);
			window.draw(Back2);
		}
		else if (NewGameMenu)
		{
			window.draw(NewGameWindow);
			window.draw(StartGame.button);
			window.draw(StartGameText);
			window.draw(Xbutton3.button);
			window.draw(Back3);
			window.draw(TextBox);
			window.draw(playertext);
		}

		window.draw(Cursor);
		window.display();
	}
}