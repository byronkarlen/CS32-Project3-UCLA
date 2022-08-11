#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//A base class for all the games Objects
class Actor : public GraphObject{
public:
    Actor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    
    void setLiveStatus(bool b);
    bool getLiveStatus() const;
    
    StudentWorld* getWorld() const;
    
    virtual char getGameID() const = 0;
    
    //Default annoy function on an actor does nothing
    virtual void annoy(int howMuch){}
    
    //Move the actor one square in it's current direction if it stays within bounds
    void move(); 
    
    virtual ~Actor(){}
    
    virtual void doSomething() = 0;
private:
    StudentWorld* m_world;
    bool m_isAlive;
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
    
    void incrementNumNuggets();
    int getNumNuggets() const;
    
    void incrementNumSonarCharges();
    int getNumSonarCharges() const;
    
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

//class Barrel : public Actor{
//public:
//    Barrel(StudentWorld* myWorld, int startX, int startY);
//    void doSomething();
//    char getGameID() const;
//private:
//};
//
//class RegularProtestor : public Actor{
//public:
//    RegularProtestor(StudentWorld* myWorld);
//    void doSomething();
//    char getGameID() const;
//    
//    
//private:
//    int m_numSquares;
//    bool m_leaveTheOilField;
//    int m_hitPoints;
//    int m_restingTicks;
//    int m_ticksElapsed;
//    int m_ticksSinceLastShout;
//    
//    bool canShoutAtTunnelMan();
//    bool canMoveTowardTunnelMan();
//};


//class Squirt : public Actor{
//public:
//    Squirt(StudentWorld* myWorld, TunnelMan* owner, int startX, int startY);
//    virtual char getGameID() const;
//    virtual void doSomething();
//private:
//    int m_travelDistance;
//    //Returns true if it successfully annoyed a nearby protestor
//    bool annoyNearbyProtestors();
//    bool canMove(Direction d) const;
//    bool boulderAt(int x, int y) const;
//};

//class Goodie : public Actor{
//public:
//    Goodie(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
//    virtual ~Goodie(){}
//protected:
//    bool tunnelManNearby(int radius) const; 
//private:
//    
//};



 

//class Gold : public Goodie{
//public:
//    Gold(StudentWorld* myWorld, int startX, int startY, bool tunnelManCanPickUp, int numTicks);
//
//    void doSomething();
//    char getGameID() const;
//private:
//    bool m_tunnelManCanPickUp;
//    int m_ticksRemaining;
////    Protestor* findNearbyProtestor();
//};

//class SonarKit : public Goodie{
//public:
//    SonarKit(StudentWorld* myWorld, int startX, int startY);
//    void doSomething();
//    char getGameID() const;
//private:
//    int m_ticksRemaining;
//
//};

//Waterpool Class:


//class Protestor : public Actor{
//public:
//    Protestor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
//    
//    int getHitPoints() const;
//    void setHitPoints(int num);
//    
//    virtual ~Protestor(){}
//private:
//    int m_hitPoints;
//    
//};



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
