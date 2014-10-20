#include "StudentWorld.h"

GameWorld* createStudentWorld()
{
    return new StudentWorld();
}


string StudentWorld::setText()
{
    int ten=10;
	int div=1;
	int round=getRound();
	int num=0;
	int hold=0;
	char stringRound[3]={'0'};
	for(int i=0; i<2; i++)
	{
		hold=round%ten-hold;
		num=hold/div;
		stringRound[2-i-1]='0'+num;
		ten*=10;
		div*=10;
	}

    string strz;
	double energyDouble=((m_player->getEnergy())/50.0)*100;
	energyDouble+=0.5;
	int energy=energyDouble;
	if(energy==100)
	{
		strz="100";
 	}
	else
	{
		char stringEnergy[3];
		std::fill_n(stringEnergy,3,' ');
		int j=0;
		do
		{
			if(j>=2)
				break;
			stringEnergy[2-j-1]=(energy%10)+'0';
			j++;
		}
		while((energy/=10) >0);

	
		for(int q=0; q<2; ++q)
		{
			strz+=stringEnergy[q];
		}
	}

	ten=10;
	hold=0;
	div=1;
	int torpedo=m_player->getTorpedo();
	char stringTorpedo[4]={'0'};
	for(int i=0; i<3; i++)
	{
		hold=torpedo%ten-hold;
		num=hold/div;
		stringTorpedo[3-i-1]='0'+num;
		ten*=10;
		div*=10;
	}


	ten=10;
	div=1;
	int ships=this->getLives();
	hold=0;
	char stringShip[3]={'0'};
	for(int i=0; i<2; i++)
	{
		hold=ships%ten-hold;
		num=hold/div;
		stringShip[2-i-1]='0'+num;
		ten*=10;
		div*=10;
	}

	ten=10;
	hold=0;
	div=1;
	int score=this->getScore();
	char stringScore[8]={'0'};
	for(int i=0; i<7; i++)
	{
		hold=score%ten-hold;
		num=hold/div;
		stringScore[7-i-1]='0'+num;
		ten*=10;
		div*=10;
	}

	string s="Score: ";
	s+=stringScore;
	s+="  Round: ";
	s+=stringRound;
	s+="  Energy: ";
	s+=strz;
	//s+=stringEnergy;
	s+="%";
	s+="  Torpedoes: ";
	s+=stringTorpedo;
	s+="  Ships: ";
	s+=stringShip;

	return s;
}



// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp