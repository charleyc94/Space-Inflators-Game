#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "actor.h"
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld()
	{
		gameRound=1;
		m_player= new player;
		
		m_player->setWorld(this);
	}
	virtual void init()
	{ 
		if(m_player==NULL)
		{
			m_player=new player;
			m_player->setWorld(this);
			m_player->setEnergy(50);
			m_player->setTorpedo(0);
			m_player->setKills(0);
			m_player->setLastTick(false);
		}
		
		m_player->moveTo(VIEW_WIDTH/2,1);

    }

	virtual int move()
    {
		if(this->testParamsProvided())
		{
			if(m_actor.size()==0)
			{
				int j=this->getTestParam(TEST_PARAM_ACTOR_INDEX);
				switch (j)
				{
				case TEST_PARAM_NACHLING:
					{
						nachling* nachPtr=new nachling(getRound());
						nachPtr->setWorld(this);
						m_actor.push_back(nachPtr);
						break;
					}
				case TEST_PARAM_WEALTHY_NACHLING:
					{
						wealthyNachling* wealthPtr=new wealthyNachling(getRound());
						wealthPtr->setWorld(this);
						m_actor.push_back(wealthPtr);
						break;
					}
				case TEST_PARAM_SMALLBOT:
					{
						smallBot* botPtr=new smallBot(getRound());
						botPtr->setWorld(this);
						m_actor.push_back(botPtr);
						break;
					}
				case TEST_PARAM_GOODIE_ENERGY:
					{
						energyGoodie* energyPtr=new energyGoodie(15,39,this);
						m_actor.push_back(energyPtr);
						break;
					}
				case TEST_PARAM_GOODIE_TORPEDO:
					{
						torpedoGoodie* torpPtr=new torpedoGoodie(15,39,this);
						m_actor.push_back(torpPtr);
						break;
					}
				case TEST_PARAM_GOODIE_FREE_SHIP:
					{
						freeShipGoodie* shipPtr=new freeShipGoodie(15,39,this);
						m_actor.push_back(shipPtr);
						break;
					}
				}
			}
		}
		else
		{
			if(rand()%3==1)
			{
				star* temp= new star;
				temp->setWorld(this);
     			m_actor.push_back(temp);
			}
			int numAliensAlive=0;
			list<actor*>::iterator it;
			for(it=m_actor.begin();it!=m_actor.end();it++)
			{
				alien* isAlien=dynamic_cast<alien*> (*it);
				if(isAlien!=NULL)
				{
					numAliensAlive++;
				}
			}

			if(numAliensAlive<int(2+0.5*getRound()))
	        {   
				if(numAliensAlive<(2*(getRound()*(getRound()+1)))-m_player->getKills())
				{ 
		    
					if(rand()%100<70)
					{
			    
						if(rand()%100>=20)
						{
							nachling* nachy=new nachling(getRound());
							nachy->setWorld(this);
							m_actor.push_back(nachy);
						}	   
						else
						{
							wealthyNachling* nach=new wealthyNachling(getRound());
							nach->setWorld(this);
							m_actor.push_back(nach);
						}
					}
					else
					{
						smallBot* botty=new smallBot(getRound());
						botty->setWorld(this);
						m_actor.push_back(botty);
					}
				}
			}
		 
		}
		this->setGameStatText(setText());
		// This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
		
		list<actor*>::iterator i;
		for(i=m_actor.begin(); i!=m_actor.end(); i++)
		{
			if((*i)->getStatus())
			{
				(*i)->doSomething();
			}
		}
        m_player->doSomething();
		actor* toDelete=NULL;
		list<actor*>::iterator iter;
		for(iter=m_actor.begin();iter!=m_actor.end();)
		{
			if(!(*iter)->getStatus())
			{	
				toDelete=(*iter);
			    iter=m_actor.erase(iter);
				delete toDelete;
			}
			else 
				iter++;
			
		}

		if(m_player->getEnergy()<=0)
		{
			playSound(SOUND_PLAYER_DIE);
			this->decLives();
			return GWSTATUS_PLAYER_DIED;
		}
			
		if(m_player->getWorld()->getRound()==11)
		{
			return GWSTATUS_GAMEWON;
		}

		return GWSTATUS_CONTINUE_GAME;

    }

	virtual void cleanUp()
    {     
		delete m_player;
		m_player=NULL;
		actor* toDelete=NULL;
		list<actor*>::iterator i;
		for(i=m_actor.begin();i!=m_actor.end();)
		{
				toDelete=(*i);
			    i=m_actor.erase(i);
				delete toDelete;
		}
    }

	string setText();

	void addActor(int ID, int x, int y, int actorType)
	{
		switch (ID)
		{
		case IID_BULLET:
		{
			bullet* add_actor=new bullet(x,y,actorType);
		    m_actor.push_back(add_actor);
		    add_actor->setWorld(this);
			break;
		}
		case IID_TORPEDO:
		{
			torpedo* torp=new torpedo(x,y,actorType);
		    m_actor.push_back(torp);
		    torp->setWorld(this);
			break;
		}
		case IID_FREE_SHIP_GOODIE:
		{
			freeShipGoodie* freeGoodie=new freeShipGoodie(x,y,this);
		    m_actor.push_back(freeGoodie);
		    freeGoodie->setWorld(this);
			break;
		}
		case IID_TORPEDO_GOODIE:
		{	torpedoGoodie* torpGoodie=new torpedoGoodie(x,y,this);
		    m_actor.push_back(torpGoodie);
		    torpGoodie->setWorld(this);
			break;
		}
		case IID_ENERGY_GOODIE:
		{
			energyGoodie* eGoodie=new energyGoodie(x,y,this);
		    m_actor.push_back(eGoodie);
		    eGoodie->setWorld(this);
			break;
		}
		}

	}

	player* getPlayer()
	{
		return m_player;
	}

	int getRound()
	{
		return gameRound;
	}

	void setRound(int round)
	{
		gameRound=round;
	}

void deleteActor(int x, int y, projectile* bullet)
{
	
	if(bullet->getActor()==IID_PLAYER_SHIP)
	{
		list<actor*>::iterator it;
		for(it=m_actor.begin();it!=m_actor.end(); it++)
		{
			alien* a=dynamic_cast<alien*> (*it);
			if(a!=NULL && (*it)->getX()==x && (*it)->getY()==y)
			{	
				a->damage();
				bullet->setStatus(false);
			}
		}
	}
	if(bullet->getActor()>=1 && bullet->getActor()<=3)
	{
		if(x==m_player->getX() && y==m_player->getY())
		{
			m_player->damage();
			bullet->setStatus(false);
		}
	}
}

bool checkCollision()
{
	list<actor*>::iterator it;
	for(it=m_actor.begin();it!=m_actor.end();it++)
	{
	  alien* a=dynamic_cast<alien*> (*it);
			if(a!=NULL)
			{
				if(a->getX()==m_player->getX() && a->getY()==m_player->getY())
				{
					playSound(SOUND_ENEMY_PLAYER_COLLISION);
					a->setStatus(false);
					m_player->setEnergy(m_player->getEnergy()-15);
					return true;
				}
			}
	}
	return false;
}

int getNumOfEnemyBullets()
{
	int enemyProjectCount=0;
	list<actor*>::iterator it;
	for(it=m_actor.begin();it!=m_actor.end();it++)
	{
		projectile* p=dynamic_cast<projectile*> (*it);
		if(p!=NULL)
		{
			if(p->getActor()>=1 && p->getActor()<=3)
				enemyProjectCount++;
		}
	}
	return enemyProjectCount;
}

virtual ~StudentWorld()
{
	delete m_player;
	m_player=NULL;
	actor* toDelete=NULL;
	list<actor*>::iterator i;
	for(i=m_actor.begin();i!=m_actor.end();)
	{
		toDelete=(*i);
		i=m_actor.erase(i);
		delete toDelete;
	}

}

private:
	list<actor*> m_actor;
	player* m_player;
	int gameRound;
};

#endif // _GAMEWORLD_H_
