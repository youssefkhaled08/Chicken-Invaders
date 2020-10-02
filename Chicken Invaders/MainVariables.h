#pragma once
#include "Player.h";
#include "Enemy.h";
#include <SFML/Graphics.hpp>;
#include <iomanip>
#include <sstream>
using namespace std; 
using namespace sf;

extern RenderWindow window;
extern RectangleShape Background;
extern Texture BackgroundTexture;
extern vector<enemyStructure> Enemies;
extern vector<ScoreDrop> coinsAndChicken;
extern vector<WeaponDrop> WeaponGifts;
extern vector<Texture> WeaponGiftTextures;
extern vector<Weapon> Weapons;
extern vector<Player> Players, PlayersForLeaderBoard;
extern int giftsCounter;
extern int EnemiesSize;
extern Texture bulletTex;
extern bool GiftChecker = false;
extern int FPS, FPSDiffRatio;
extern vector<Bullet> bullet;
extern float EnemyRateOfFire = 0.5;
extern Clock EnemyTimer;
extern Clock FPSClock;
extern Texture ChickenTex;
extern Texture EnemyExplosion, EggTex, CoinTex, ChickTex;
extern Texture playerTex, LaserTex, PlasmaTex;
extern Texture UpgradeTex;
extern Texture ionBlasterGift, neutronGunGift, hyperGunGift, vulcanGunGift, PlasmaGift, LaserGift;
extern Texture BarBack, BarBlue, BarOrange, BarGreen, BarCyan, BarPurble, BarRed, BarPink , UIBackTex;
extern Texture Button1, Button2,XButton1,XButton2;
extern bool SoundVFX, MusicVFX;
extern Texture BackgroundLevel1, BackgroundLevel2, BackgroundLevel3;
extern Texture MidEggCrack, HighEggCrack;