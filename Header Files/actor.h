#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class actor: public GraphObject
{
public:
	virtual ~actor(){}
	virtual void doSomething()=0;
	actor(int id, int x, int y);
	actor(int id);

	bool getLastTick()
	{
		return lastTick;
	}

	void setLastTick(bool tick)
	{
		lastTick=tick;
	}

	bool getStatus()
	{
		return m_status;
	}

	void setWorld(StudentWorld* world)
	{
		m_world=world;
	}
	void setStatus(bool status)
	{
		m_status=status;
	}
	StudentWorld* getWorld();

private:
	bool lastTick;
	bool m_status;
	StudentWorld* m_world;
};


class star: public actor
{
public:
	star();
	virtual ~star(){}
	virtual void doSomething()
	{
	    moveTo(getX(),getY()-1);
		if(getY()<0)
			this->setStatus(false);
	}
private:

};

class goodie: public actor
{
public:
	virtual ~goodie(){}
	goodie(int id, int x, int y,StudentWorld* world);
	int getTicks()
	{
		return ticks;
	}
	void setTicks(int t)
	{
		ticks=t;
	}
	int getMaxTicks()
	{
		return tickLife;
	}
private:
	int ticks;
	int tickLife;
};

class freeShipGoodie: public goodie
{
public:
	virtual ~freeShipGoodie(){}
	freeShipGoodie(int x, int y, StudentWorld* world);
	virtual void doSomething();
private:
};

class energyGoodie: public goodie
{
public:
	virtual ~energyGoodie(){}
	energyGoodie(int x, int y, StudentWorld* world);
	virtual void doSomething();
private:
};

class torpedoGoodie: public goodie
{
public:
	virtual ~torpedoGoodie(){}
	torpedoGoodie(int x, int y, StudentWorld* world);
	virtual void doSomething();
private:
};

class projectile: public actor
{
public:
	virtual ~projectile(){}
	projectile(int id,int x, int y, int actorID):actor(id, x, y), origin(actorID){}
	int getActor()
	{
		return origin;
	}
private:
	int origin;
};

class torpedo: public projectile
{
public:
	virtual ~torpedo(){}
	torpedo(int x, int y, int ID);
	virtual void doSomething();
private:
};

class bullet: public projectile
{
public:
	virtual ~bullet(){}
	bullet(int x, int y, int ID);
	virtual void doSomething();
private:
};

class target:public actor
{
public:
	virtual ~target(){}
	target(int id,int x,int y,int energy):actor(id,x,y), m_energy(energy){}
	target(int id, int energy):actor(id),m_energy(energy){}
	int getEnergy()
	{
		return m_energy;
	}
	void setEnergy(int num)
	{
		m_energy=num;
	}
	virtual void damage()=0;
private:
	int m_energy;
};

class alien: public target
{
public:
	alien::alien(int id, int factor, int round):target(id,int(factor*(0.9 + (0.1*round)))){}
	void damage();
	virtual ~alien(){}
private:
};

class nachling: public alien
{
public:
	virtual ~nachling(){}
	nachling(int round);
	nachling(int id, int factor,int round);
	virtual void doSomething();
	void setState(int num)
	{
		m_state=num;
	}
	int getState()
	{
		return m_state;
	}
    void setHorz(int num)
	{
		HMR=num;
	}
	int getHorz()
	{
		return HMR;
	}
	void setDir(bool direction)
	{
		moveRight=direction;
	}
	bool getDir()
	{
		return moveRight;
	}
	void setDist(int num)
	{
		HMD=num;
	}
	int getDist()
	{
		return HMD;
	}
private:
	int m_state;
	bool moveRight;
	int HMR;
	int HMD;
};

class wealthyNachling:public nachling
{
public:
	virtual ~wealthyNachling(){}
	wealthyNachling(int round);
	virtual void doSomething();
	bool getMal()
	{
		return isMal;
	}
	void setMal(bool yes)
	{
		isMal=yes;
	}
	int getCount()
	{
		return malCount;
	}
	void setCount(int num)
	{
		malCount=num;
	}
private:
	bool isMal;
	int malCount;
};

class smallBot:public alien
{
public:
	virtual ~smallBot(){}
	smallBot(int round);
	virtual void doSomething();
	void setAttacked(bool yes)
	{
		wasAttacked=yes;
	}
	bool getAttacked()
	{
		return wasAttacked;
	}
private:
	bool wasAttacked;
};


class player: public target
{
public:
	player();
	virtual ~player(){}
	virtual void doSomething();
	void setTorpedo(int num)
	{
		m_torpedo=num;
	}
	int getTorpedo()
	{
		return m_torpedo;
	}
	void setKills(int kills)
	{
		m_kills=kills;
	}
	int getKills()
	{
		return m_kills;
	}
	void damage();
private:
	int m_torpedo;
	int m_kills;
};


#endif // _ACTOR_H_