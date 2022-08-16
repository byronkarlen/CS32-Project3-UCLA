#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//A base class for all the games Objects
class Actor : public GraphObject{
public:
    Actor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool annoyable = true);
    
    void setLiveStatus(bool b);
    bool getLiveStatus() const;
    
    StudentWorld* getWorld() const;
    
    virtual char getGameID() const = 0;
    
    //Default annoy function on an actor does nothing
    virtual void annoy(int howMuch){}
    virtual void changeAnnoyableStatus();
    virtual bool isAnnoyable();
    
    //Move the actor one square in it's current direction if it stays within bounds
    void move();
    
    virtual ~Actor(){}
    
    virtual void doSomething() = 0;
private:
    StudentWorld* m_world;
    bool m_isAlive;
    bool m_annoyable;
};

class Earth : public Actor{
public:
    Earth(StudentWorld* myWorld, int startX, int startY);
    
    //Earth doesn't do anything
    virtual void doSomething(){}
    //Earth has a gameID of 'E'
    virtual char getGameID() const;
};



class TunnelMan : public Actor{
public:
    TunnelMan(StudentWorld* myWorld);
    
    virtual void doSomething();
    virtual char getGameID() const;
    virtual void annoy(int howMuch);
    
    //Getters and Setters
    void incrementNumNuggets();
    int getNumNuggets() const;
    
    void incrementNumSonarCharges();
    int getNumSonarCharges() const;
    
    //This function adds 5 units of squirts to the Tunnelman's inventory and increases the points by 100
    void incrementNumWater();
    int getNumWater() const;
    
    void incrementBarrelsFound();
    int getNumBarrelsFound();
    
    void setHitPoints(int points);
    int getHitPoints() const;
    
private:
    int m_numWater;
    int m_numSonarCharges;
    int m_numNuggets;
    int m_numBarrelsFound;
    int m_hitPoints;

    void introduceSquirt();
};


class Boulder : public Actor{
public:
    Boulder(StudentWorld* myWorld, int startX, int startY);
    virtual char getGameID() const;
    virtual void doSomething();
private:
    int state; //0 for stable, 1 for waiting, 2 for falling
    int ticksElapsed;
    void smushCharacters();
    bool boulderCanFall() const;
};

class Barrel : public Actor{
public:
    Barrel(StudentWorld* myWorld, int startX, int startY);
    virtual void doSomething();
    virtual char getGameID() const;
};


class Protestor : public Actor{
public:
    Protestor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);

    virtual ~Protestor(){}
    
private:

};

class RegularProtestor : public Protestor{
public:
    RegularProtestor(StudentWorld* myWorld);
    virtual void doSomething();
    virtual char getGameID() const;
    virtual void annoy(int howMuch);
    bool wantsToLeave();
    
private:
    int m_numSquaresToMoveInCurrentDirection;
    bool m_leaveTheOilField;
    int m_hitPoints;
    
    int m_tickCount;
    int m_NonRestingTicksSinceShout;
    int m_NonRestingTicksSinceTurn;
    
    bool justTurned90(Direction d1, Direction d2);
    
    Direction generateRandomDirection();
    void changeDirectionInOrderToExit();
    bool withinShoutingDistance();
    void faceTunnelMan();
    bool canMoveTowardTunnelMan();
    int generateNumSquaresToMove();
    bool isViableDirection(Direction d);
    bool atIntersection(); //If the protestor is at the intersection, it will return true and rotate it 90 degrees
};

class Squirt : public Actor{
public:
    Squirt(StudentWorld* myWorld, TunnelMan* owner, int startX, int startY);
    virtual char getGameID() const;
    virtual void doSomething();
private:
    int m_travelDistance;
    bool canMove(Direction d) const;
    bool boulderAt(int x, int y) const;
};

class Goodie : public Actor{
public:
    Goodie(StudentWorld* myWorld, int imageID, int startX, int startY);
    
    virtual ~Goodie(){}
    
    virtual bool isAnnoyable(){return false;}
    
    
private:

};



//Waterpool Class:
class Waterpool : public Goodie{
public:
    Waterpool(StudentWorld* myWorld, int startX, int startY);
    virtual char getGameID() const;
    virtual void doSomething();
private:
    int m_tickCount;
};

class SonarKit : public Goodie{
public:
    SonarKit(StudentWorld* myWorld, int startX, int startY);
    void doSomething();
    char getGameID() const;
private:
    int m_tickCount;

};


class Gold : public Goodie{
public:
    Gold(StudentWorld* myWorld, int startX, int startY, bool tunnelManCanPickUp, bool m_permanent, int ticksToWait);
    void doSomething();
    char getGameID() const;
    
private:
    int m_tickCount;
    bool m_tunnelManCanPickUp;
    bool m_permanent;
};










//class HardcoreProtestor : public Protestor{
//public:
//    HardcoreProtestor(StudentWorld* myWorld);
//    void doSomething();
//    char getGameID() const;
//
//private:
//
//};


#endif // ACTOR_H_

