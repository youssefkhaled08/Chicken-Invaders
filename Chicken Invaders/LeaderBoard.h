#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
struct Person
{
	string name = "____";
	int score = 0;
};

extern Person LeaderBoard[10];

void Sort_Scores()
{
	int max, index;
	for (int i = 0; i < 10; i++)
	{
		max = LeaderBoard[i].score;
		for (int j = i; j < 10; j++)
		{
			if (LeaderBoard[j].score >= max)
			{
				max = LeaderBoard[j].score;
				index = j;
			}
		}
		if (i != index)
		{
			Person rem = LeaderBoard[i];
			LeaderBoard[i] = LeaderBoard[index];
			LeaderBoard[index] = rem;
		}
	}

	fstream File("LeaderBoard.txt");
	for (int i = 0; i < 10; i++)
	{
		File << LeaderBoard[i].name << " " << LeaderBoard[i].score<<endl;
	}
	File.close();
}

void Add_Score(string newName, int newScore)
{
	if (newScore >= LeaderBoard[9].score)
	{
		LeaderBoard[9].name = newName;
		LeaderBoard[9].score = newScore;
	}
	Sort_Scores();
}