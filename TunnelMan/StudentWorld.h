#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

//Constants:
const int actorSize = 4;
const int ticksBeforeFalling = 30;
const int squirtInitialTravelDistance = 4;

class Actor;
class Earth;
class TunnelMan;
class Boulder;
class Squirt;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    virtual ~StudentWorld(){}
    
    //Additional Functions:
    
    //removes earth object at the specified location. If there is no earth object at the given location, it does nothing
    void removeEarth(int x, int y);
    //returns whether there is an earth object at the given location. If the location is not valid, it returns false anyway
    bool isEarthAt(int x, int y) const;
    
    
    //adds an actor to the field (except for Earth) to the field
    void addActor(Actor* a);
    //removes an actor (except for Earth and TunnelMan) from a location at the field
    void removeActor(Actor* a);
    //returns a pointer to an actor (with a certain characterID) found at the given location. If none is found, returns nullptr
    Actor* findActor(int x, int y, char c) const;
    
    //Returns a pointer to the TunnelMan
    TunnelMan* getTunnelMan() const;
    
    //Returns whether any part of the TunnelMan is present at a given location
    bool isTunnelManAt(int x, int y) const;
    
    //returns the x-coordinate of the TunnelMan
    int getPlayerX() const;
    //returns the y-coordinate of the TunnelMan
    int getPlayerY() const;

    bool inField(int x, int y) const;
    
private:
    Earth* m_earthTracker[VIEW_HEIGHT][VIEW_WIDTH];
    std::list<Actor*> m_gameObjects;
    TunnelMan* m_player;
    
    void populateFieldWithEarth();
    void populateFieldWithBoulders();
    void populateFieldWithBarrels();
    void populateFieldWithNuggets();
    
    bool thereAreObjectsTooClose(int x, int y);
    bool playerCompletedLevel(); 
    //returns whether a given coordinate is located within the game's playing field
    
//    void updateDisplayText();
//    std::string formatStats();

};

#endif // STUDENTWORLD_H_
