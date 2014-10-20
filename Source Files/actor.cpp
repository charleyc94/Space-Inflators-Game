#include "actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"
#include <cstdlib>

// Students:  Add code to this file (if you wish), actor.h, StudentWorld.h, and StudentWorld.cpp

actor::actor(int id, int x, int y):GraphObject(id,x,y), lastTick(false), m_status(true), m_world(NULL)
{
	this->setVisible(true);
}

actor::actor(int id):GraphObject(id,rand()%VIEW_WIDTH,VIEW_HEIGHT-1), lastTick(false),m_status(true), m_world(NULL)
{
	this->setVisible(true);
}

player::player():target(IID_PLAYER_SHIP,VIEW_WIDTH/2,1,50),m_torpedo(0), m_kills(0){}

star::star():actor(IID_STAR){}

nachling::nachling(int round):alien(IID_NACHLING,5, round), m_state(0),moveRight(true),HMR(0),HMD(0){}

nachling::nachling(int id, int factor, int round):alien(id,factor,round), m_state(0),moveRight(true),HMR(0),HMD(0){}

wealthyNachling::wealthyNachling(int round):nachling(IID_WEALTHY_NACHLING, 8, round), isMal(false), malCount(0){}

smallBot::smallBot(int round):alien(IID_SMALLBOT,12,round),wasAttacked(false){}

torpedo::torpedo(int x, int y, int ID):projectile(IID_TORPEDO, x, y,ID){}

bullet::bullet(int x, int y, int ID):projectile(IID_BULLET, x, y,ID){}

goodie::goodie(int id, int x, int y,StudentWorld* world): actor(id,x,y),ticks(0)
{
	setWorld(world);
    tickLife=(100/(getWorld()->getRound()))+30;
}

freeShipGoodie::freeShipGoodie(int x, int y, StudentWorld* world):goodie(IID_FREE_SHIP_GOODIE,x,y, world){}

energyGoodie::energyGoodie(int x, int y, StudentWorld* world):goodie(IID_ENERGY_GOODIE,x,y,world){}

torpedoGoodie::torpedoGoodie(int x, int y,StudentWorld* world):goodie(IID_TORPEDO_GOODIE,x,y,world){}

StudentWorld* actor::getWorld()
{
  return m_world;
}

void player::doSomething()
{
	if(getWorld()->checkCollision())
	{
		return;
	}
	if(getEnergy()<=0)
	{	
		return;
	}
	int num;
	getWorld()->getKey(num);
	
	switch (num)
	{
	case KEY_PRESS_LEFT :
	{
		if(getX()>0)
		this->moveTo(getX()-1,getY());
		break;
	}
	case KEY_PRESS_RIGHT:
	{
		if(getX()<29)
		this->moveTo(getX()+1,getY());
		break;
	}
	case KEY_PRESS_UP:
	{
		if(getY()<39)
		this->moveTo(getX(),getY()+1);
		break;
	}
	case KEY_PRESS_DOWN:
	{
		if(getY()>0)
		this->moveTo(getX(),getY()-1);
		break;
	}
	case KEY_PRESS_SPACE:
	{
		if(!getLastTick()&& getY()<39)
		{	
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			getWorld()->addActor(IID_BULLET,getX(), getY()+1,IID_PLAYER_SHIP);
			setLastTick(true);
			return;
	    }
		break;
	}
	case KEY_PRESS_TAB:
	{
		if(!getLastTick())
		{
			if(getTorpedo()>0)
		    {
				getWorld()->playSound(SOUND_PLAYER_TORPEDO);
		        getWorld()->addActor(IID_TORPEDO,getX(), getY()+1,IID_PLAYER_SHIP);
			    setTorpedo(getTorpedo()-1);
		    }
			setLastTick(true);
			return;
		}
		break;
	}	
	}
	setLastTick(false);
	getWorld()->checkCollision();

}

void player::damage()
{
	getWorld()->playSound(SOUND_PLAYER_HIT);
	setEnergy(getEnergy()-2);
}

void alien::damage()
{
	getWorld()->playSound(SOUND_ENEMY_HIT);
	setEnergy(getEnergy()-2);
	smallBot* bot=dynamic_cast<smallBot*>(this);
	if(bot!=NULL)
	{
		bot->setAttacked(true);
	}
	if(getEnergy()<=0)
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		

		getWorld()->getPlayer()->setKills(getWorld()->getPlayer()->getKills()+1);
		if(getWorld()->getPlayer()->getKills()==2*(getWorld()->getRound()*(getWorld()->getRound()+1)))
     	{
			getWorld()->setRound(getWorld()->getRound()+1);
					
		}

          		nachling* b=dynamic_cast<nachling*> (this);
	    if(b!=NULL)
			getWorld()->increaseScore(1000);
		wealthyNachling* c=dynamic_cast<wealthyNachling*> (this);
		if(c!=NULL)		
		{
			getWorld()->increaseScore(200);
		    if(rand()%3==1)
			{
				
				if(rand()%2==0)
				{
					getWorld()->addActor(IID_ENERGY_GOODIE,getX(),getY(),IID_WEALTHY_NACHLING);
				}
				else
				{
					getWorld()->addActor(IID_TORPEDO_GOODIE,getX(),getY(),IID_WEALTHY_NACHLING);
				}
			}
		}
		smallBot* d=dynamic_cast<smallBot*> (this);
		if(d!=NULL)
		{
			getWorld()->increaseScore(1500);
			
		    if(rand()%3==1)
			{
				getWorld()->addActor(IID_FREE_SHIP_GOODIE,getX(),getY(),IID_SMALLBOT);
			}
		}
		setStatus(false);
	}
}


void bullet::doSomething()
{		
	getWorld()->deleteActor(getX(),getY(), this);
	if(!getStatus())
		return;
	if(this->getActor()==IID_PLAYER_SHIP)
	{
		moveTo(getX(),getY()+1);
	}
	else
		moveTo(getX(),getY()-1);

	if(getY()>39 || getY()<0)
		setStatus(false);

    getWorld()->deleteActor(getX(),getY(), this);
}

void torpedo::doSomething()
{		
	getWorld()->deleteActor(getX(),getY(), this);
	if(!getStatus())
		return;
	if(this->getActor()==IID_PLAYER_SHIP)
	{
		moveTo(getX(),getY()+1);
	}
	else
		moveTo(getX(),getY()-1);
	
	if(getY()>39 || getY()<0)
		setStatus(false);

   getWorld()->deleteActor(getX(),getY(), this);
	
}


void nachling::doSomething()
{
	if(!getLastTick())
	{
		setLastTick(true);
		if(getState()==0)
		{
			if(getWorld()->getPlayer()->getX()==getX()&& getX()!=0 && getX()!=29)
			{
				setState(1);
				int MDB=0;
				MDB=((VIEW_WIDTH-1-getX())<getX())?(VIEW_WIDTH-1-getX()):getX();
				if(MDB>3)
				{
					setDist((rand()%3)+1);
				}
				else 
					setDist(MDB);
				setHorz(getDist());
				if(rand()%2==0)
					setDir(true);
				else
					setDir(false);
			}
			else
			{
				
				if(rand()%3==1)
				{
					if(getWorld()->getPlayer()->getX()>getX())
					{
							moveTo(getX()+1,getY()-1);
						return;
					}
					if(getWorld()->getPlayer()->getX()<getX())
					{
					
							moveTo(getX()-1,getY()-1);

						return;
					}
				}
			}
			moveTo(getX(),getY()-1);
			if(getY()<0)
			{
				setStatus(false);
				return;
			}
			return;
		}

		if(getState()==1)
		{
			if(getWorld()->getPlayer()->getY()>getY())
			{
				setState(2);
				return;
			}
			if(getHorz()==0)
			{
				setDir(!getDir());
				setHorz(2*getDist());
			}
			else
				setHorz(getHorz()-1);
			int n=(getDir())?1:-1;
				moveTo(getX()+n,getY());
			int chanceOfFiring=(10/getWorld()->getRound())+1;
			if(rand()%chanceOfFiring==0)
			{
				if(getWorld()->getNumOfEnemyBullets()<2*getWorld()->getRound())
				{
					getWorld()->playSound(SOUND_ENEMY_FIRE);
					wealthyNachling* nachlingPtr=dynamic_cast<wealthyNachling*> (this);
					if(nachlingPtr!=NULL)
					{
						getWorld()->addActor(IID_BULLET, getX(),getY()-1,IID_WEALTHY_NACHLING);
					}
					else
						getWorld()->addActor(IID_BULLET, getX(),getY()-1,IID_NACHLING);
				}
			}
			if(rand()%20==3)
			{
				setState(2);
				return;
			}
			return;
		}

		if(getState()==2)
		{
			if(getY()==39)
			{
				setState(0);
				return;
			}
			if(getX()==0)
				setDir(true);
			if(getX()==29)
				setDir(false);
			if(getX()<29 && getX()>0)
			{
				if(rand()%2==0)
					setDir(true);
				else
					setDir(false);
			}
			int x=(getDir())?1:-1;
			moveTo(getX()+x,getY()+1);
			
			return;
		}
		
	}
	else
	{
		setLastTick(false);
		return;
	}
	if(getY()<0)
		setStatus(false);
}

void wealthyNachling::doSomething()
{
	if(getMal())
    {   
		setCount(getCount()+1);
		if(getCount()==30)
			setMal(false);
		return;
	}
	else
	{
		if(rand()%200==6)
		{
			setCount(0);
			setMal(true);
			return;
		}
		nachling::doSomething();
	}
	
}

void smallBot::doSomething()
{
	if(!getLastTick())
	{
		if(getAttacked())// && rand()%3==1)
		{
			setAttacked(false);
			if(getX()==0)
				moveTo(1,getY());
			if(getX()==29)
				moveTo(28,getY());
			else
			{
				if(rand()%2==1)
				{
					if(getX()>0)
					{
						moveTo(getX()-1,getY()-1);
					}
				}
				else
				{
					if(getX()<29)
					{
						moveTo(getX()+1,getY()-1);
					}
				}
			}
		}
		else
		{
			moveTo(getX(),getY()-1);
		}
		if(getWorld()->getPlayer()->getX()==getX() && getY()>getWorld()->getPlayer()->getY())
		{
			if(getWorld()->getNumOfEnemyBullets()<2*getWorld()->getRound())
			{
				int q=100/getWorld()->getRound();
			    if(rand()%q==4)
				{
					getWorld()->playSound(SOUND_ENEMY_FIRE);
					getWorld()->addActor(IID_TORPEDO,getX(),getY()-1,IID_SMALLBOT);
				}
				else
				{
					getWorld()->playSound(SOUND_ENEMY_FIRE);
					getWorld()->addActor(IID_BULLET,getX(),getY()-1,IID_SMALLBOT);
				}
			}

		}
		if(getY()<0)
			setStatus(false);

		setLastTick(true);
	}
	else
		setLastTick(false);
}

void freeShipGoodie::doSomething() 
{
	if(getX()!=getWorld()->getPlayer()->getX()|| getY()!=getWorld()->getPlayer()->getY())
	{
		if(getTicks()==getMaxTicks())
			setStatus(false);
		double brightness=((getMaxTicks()-getTicks())/double(getMaxTicks()));
		this->setBrightness(0.2+brightness);
		if(getTicks()%3==0)
		{
			moveTo(getX(),getY()-1);
		}
		setTicks(getTicks()+1);
		if(getX()==getWorld()->getPlayer()->getX()&& getY()==getWorld()->getPlayer()->getY())
		{
			getWorld()->increaseScore(getWorld()->getScore()+5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setStatus(false);
			getWorld()->incLives();
		}
		
	}	
	else
	{
		getWorld()->increaseScore(getWorld()->getScore()+5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		setStatus(false);
		getWorld()->incLives();
	}
}

void energyGoodie::doSomething()
{
	if(getX()!=getWorld()->getPlayer()->getX() || getY()!=getWorld()->getPlayer()->getY())
	{
		if(getTicks()==getMaxTicks())
			setStatus(false);
		double brighty=(getMaxTicks()-getTicks())/double (getMaxTicks());
 		this->setBrightness(0.2+brighty);
		if(getTicks()%3==0)
		{
			moveTo(getX(),getY()-1);
		}
		setTicks(getTicks()+1);
		if(getX()==getWorld()->getPlayer()->getX()&& getY()==getWorld()->getPlayer()->getY())
		{
			getWorld()->increaseScore(getWorld()->getScore()+5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setStatus(false);
			getWorld()->getPlayer()->setEnergy(50);
		}
	}
	else
	{
		getWorld()->increaseScore(getWorld()->getScore()+5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		setStatus(false);
		getWorld()->getPlayer()->setEnergy(50);
	}
}

void torpedoGoodie::doSomething()
{
	if(getX()!=getWorld()->getPlayer()->getX()|| getY()!=getWorld()->getPlayer()->getY())
	{
		if(getTicks()==getMaxTicks())
			setStatus(false);
		double bright=(getMaxTicks()-getTicks())/double(getMaxTicks());
		this->setBrightness(0.2+ bright);
		if(getTicks()%3==0)
		{
			moveTo(getX(),getY()-1);
		}
		setTicks(getTicks()+1);
		if(getX()==getWorld()->getPlayer()->getX()&& getY()==getWorld()->getPlayer()->getY())
		{
			getWorld()->increaseScore(getWorld()->getScore()+5000);
			getWorld()->playSound(SOUND_GOT_GOODIE);
			setStatus(false);
			getWorld()->getPlayer()->setTorpedo(getWorld()->getPlayer()->getTorpedo()+5);
		}
	}
	else
	{
		getWorld()->increaseScore(getWorld()->getScore()+5000);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		setStatus(false);
		getWorld()->getPlayer()->setTorpedo(getWorld()->getPlayer()->getTorpedo()+5);
	}
}