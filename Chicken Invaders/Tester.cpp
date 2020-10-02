////Player System
//for (int i = 0; i < Players.size(); i++)
//{
//	RectangleShape UIBack;
//	UIBack.setSize(Vector2f(450, 90));
//
//	UIBack.setTexture(&UIBackTex);
//	UIBack.setFillColor(Color(0, 0, 100));
//	//displaying score;
//	Font playerscore;
//	playerscore.loadFromFile("CollegiateBlackFLF.ttf");
//	Text score;
//	score.setOrigin(0, 0);
//	stringstream stringscore;
//
//	if (i == 0)
//	{
//		UIBack.setPosition(-3, 10);
//		window.draw(UIBack);
//		stringscore << Players[i].score << setw(10);
//		score.setString(stringscore.str());
//		score.setPosition(255, 37);
//	}
//	else
//	{
//		UIBack.setPosition(window.getSize().x + 3, 10);
//		UIBack.setSize(Vector2f(-450, 90));
//		window.draw(UIBack);
//		stringscore << Players[i].score << setw(10);
//		score.setString(stringscore.str());
//		score.setPosition(window.getSize().x - 415, 37);
//	}
//	score.setFillColor(Color::White);
//	score.setFont(playerscore);
//
//
//	window.draw(score);
//
//	//number of lives
//	Texture Heart;
//	Heart.loadFromFile("Heart.png");
//	RectangleShape heartshape;
//	heartshape.setSize(Vector2f(40, 40));
//	heartshape.setTexture(&Heart);
//	Font lives;
//	lives.loadFromFile("CollegiateBlackFLF.ttf");
//	stringstream number_of_lives_to_string;
//	number_of_lives_to_string << Players[i].Lives;
//	Text numberoflives;
//	numberoflives.setFont(lives);
//	numberoflives.setString(number_of_lives_to_string.str());
//	if (i == 0)
//	{
//		heartshape.setPosition(10, window.getSize().y - 50);
//		numberoflives.setPosition(60, window.getSize().y - 46);
//	}
//	else
//	{
//		heartshape.setPosition(window.getSize().x - 50, window.getSize().y - 50);
//		numberoflives.setPosition(window.getSize().x - 80, window.getSize().y - 46);
//	}
//	window.draw(heartshape);
//	window.draw(numberoflives);
//
//	if (Keyboard::isKeyPressed(Keyboard::J))
//	{
//		Players[0].score += 10000;
//		Players[1].score += 10000;
//	}
//	if (!Players[i].Exploded)
//	{
//
//		if (!Players[i].secondry)
//		{
//			if (Players[i].primaryWeapon.overHeat > 0)
//			{
//				Players[i].primaryWeapon.overHeat -= Players[i].primaryWeapon.CalmingRate;
//			}
//			else
//			{
//				Players[i].primaryWeapon.overHeat = 0;
//			}
//
//			if (!Players[i].primaryWeapon.HeatCalmed)
//			{
//
//				Players[i].primaryWeapon.Calming_Down();
//			}
//
//			//overheat progress bar
//			RectangleShape OverHeatBar;
//
//
//
//			if (i == 0)
//			{
//				if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
//					OverHeatBar.setTexture(&BarRed);
//				else
//					OverHeatBar.setTexture(&BarPurble);
//				RectangleShape BarBackground;
//				BarBackground.setTexture(&BarBack);
//				BarBackground.setSize(Vector2f(220, 20));
//				BarBackground.setPosition(15, 45);
//				OverHeatBar.setSize(Vector2f((Players[i].primaryWeapon.overHeat * BarBackground.getSize().x) / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
//				OverHeatBar.setPosition(15, 45);
//				window.draw(BarBackground);
//
//			}
//			else
//			{
//				if (Players[i].primaryWeapon.overHeat >= Players[i].primaryWeapon.MaximumOverHeat * 75 / 100)
//					OverHeatBar.setTexture(&BarRed);
//				else
//					OverHeatBar.setTexture(&BarCyan);
//				RectangleShape BarBackground;
//				BarBackground.setTexture(&BarBack);
//				BarBackground.setSize(Vector2f(-220, 20));
//				BarBackground.setPosition(window.getSize().x - 15, 45);
//				OverHeatBar.setSize(Vector2f(Players[i].primaryWeapon.overHeat * BarBackground.getSize().x / Players[i].primaryWeapon.MaximumOverHeat, 20.0f));
//				OverHeatBar.setPosition(window.getSize().x - 15, 45);
//				window.draw(BarBackground);
//			}
//			window.draw(OverHeatBar);
//		}
//		else
//		{
//			if (Players[i].secondryWeapon.overHeat > 0)
//			{
//				Players[i].secondryWeapon.overHeat -= Players[i].secondryWeapon.CalmingRate;
//			}
//			else
//			{
//				Players[i].secondryWeapon.overHeat = 0;
//			}
//
//			if (!Players[i].secondryWeapon.HeatCalmed)
//			{
//
//				Players[i].secondryWeapon.Calming_Down();
//			}
//
//			//overheat progress bar
//			RectangleShape OverHeatBar;
//
//
//
//			if (i == 0)
//			{
//				if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
//					OverHeatBar.setTexture(&BarRed);
//				else
//					OverHeatBar.setTexture(&BarGreen);
//				RectangleShape BarBackground;
//				BarBackground.setTexture(&BarBack);
//				BarBackground.setSize(Vector2f(220, 20));
//				BarBackground.setPosition(10, 30);
//				OverHeatBar.setSize(Vector2f((Players[i].secondryWeapon.overHeat * BarBackground.getSize().x) / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
//				OverHeatBar.setPosition(10, 30);
//				window.draw(BarBackground);
//
//			}
//			else
//			{
//				if (Players[i].secondryWeapon.overHeat >= Players[i].secondryWeapon.MaximumOverHeat * 75 / 100)
//					OverHeatBar.setTexture(&BarRed);
//				else
//					OverHeatBar.setTexture(&BarBlue);
//				RectangleShape BarBackground;
//				BarBackground.setTexture(&BarBack);
//				BarBackground.setSize(Vector2f(-220, 20));
//				BarBackground.setPosition(window.getSize().x - 10, 30);
//				OverHeatBar.setSize(Vector2f(Players[i].secondryWeapon.overHeat * BarBackground.getSize().x / Players[i].secondryWeapon.MaximumOverHeat, 20.0f));
//				OverHeatBar.setPosition(window.getSize().x - 10, 30);
//				window.draw(BarBackground);
//			}
//			window.draw(OverHeatBar);
//
//			// timer for the secondary weapon
//			RectangleShape TimerBar;
//			if (Players[i].timer.getElapsedTime().asSeconds() >= 20)
//				TimerBar.setTexture(&BarPink);
//			else
//				TimerBar.setTexture(&BarOrange);
//
//			if (i == 0)
//			{
//				RectangleShape TimerBackground;
//				TimerBackground.setTexture(&BarBack);
//				TimerBackground.setSize(Vector2f(220, 20));
//				TimerBackground.setPosition(10, 60);
//				TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
//				TimerBar.setPosition(10, 60);
//				window.draw(TimerBackground);
//			}
//			else
//			{
//				RectangleShape TimerBackground;
//				TimerBackground.setTexture(&BarBack);
//				TimerBackground.setSize(Vector2f(-220, 20));
//				TimerBackground.setPosition(window.getSize().x - 10, 60);
//				TimerBar.setSize(Vector2f(TimerBackground.getSize().x - (Players[i].timer.getElapsedTime().asSeconds() / 30 * TimerBackground.getSize().x), 20.0f));
//				TimerBar.setPosition(window.getSize().x - 10, 60);
//				window.draw(TimerBackground);
//			}
//			window.draw(TimerBar);
//
//		}
//		Players[i].move(window.getSize().x, window.getSize().y);
//		if (!Players[i].died)
//		{
//			Players[i].can_shoot();
//		}
//		if (!Players[i].secondry)
//		{
//			if (Players[i].primaryWeapon.fired && Players[i].primaryWeapon.HeatCalmed)
//			{
//				Laser_Check();
//				window.draw(Players[i].primaryWeapon.Laser);
//			}
//			else
//			{
//				Players[i].primaryWeapon.Laser.setSize(Vector2f(10, 0));
//			}
//		}
//		else
//		{
//			if (Players[i].secondryWeapon.fired && Players[i].secondryWeapon.HeatCalmed)
//			{
//				Laser_Check();
//				Players[i].secondryWeapon.Laser.setTexture(&PlasmaTex);
//				window.draw(Players[i].secondryWeapon.Laser);
//			}
//			else
//			{
//				Players[i].secondryWeapon.Laser.setSize(Vector2f(10, 0));
//				Players[i].secondryWeapon.BulletSound.stop();
//				Players[i].secondryWeapon.loopsound = false;
//			}
//		}
//
//
//		window.draw(Players[i].body);
//		if (Players[i].Shield)
//		{
//			window.draw(Players[i].ShieldBody);
//		}
//	}
//
//	if (Players[i].Exploded && Players[i].died)
//		Players.erase(Players.begin() + i);
//
//}