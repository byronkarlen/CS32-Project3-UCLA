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
    
    //removes earth objects within a 4x4 location specified by the bottom left corner. If there is no earth object at the given location, it does nothing
    void removeEarth(int x, int y);
    
    //returns whether there are earth objects within a given location 4x4 location specfied by the bottom left corner. If the location is not valid, it returns false anyways
    bool earthAt(int x, int y) const; 
    
    //returns whether there is an active actor with the given character ID within the specified radius
    bool actorWithinRadius(int x, int y, int radius, char ID);
    
    //annoys all active protestors within the radius an amount 'howMuch'
    void annoyProtestorsWithinRadius(int x, int y, int radius, int howMuch);
    
    //returns whether the tunnelman is within the given radius
    bool tunnelManWithinRadius(int x, int y, int radius);
    
//    Actor* findActorWithinRadius(int x, int y, int radius, char ID);
    
    //adds an actor to the field (except for Earth) to the field
    void addActor(Actor* a);
    //removes an actor (except for Earth and TunnelMan) from a location at the field
    void removeActor(Actor* a);
    
    //returns a pointer to an actor (with a certain characterID) found at the given location. If none is found, returns nullptr
    Actor* findActor(int x, int y, char c) const;
    
    //returns whether an actor (with a certain characterID) is present within a 4x4 location starting with (x,y) at the bottom left
    bool actorAt(int x, int y, char c) const;
    
    //Returns a pointer to the TunnelMan
    TunnelMan* getTunnelMan() const;
    
    //Returns whether any part of the TunnelMan is present at a given location
    bool isTunnelManAt(int x, int y) const;
    
    //returns the x-coordinate of the TunnelMan
    int getPlayerX() const;
    //returns the y-coordinate of the TunnelMan
    int getPlayerY() const;

    //returns whether a given coordinate is located within the game's playing field
    bool inField(int x, int y) const;
    
    void illuminateOilField(int x, int y, int radius);
    

    
private:
    Earth* m_earthTracker[VIEW_HEIGHT][VIEW_WIDTH];
    std::list<Actor*> m_gameObjects;
    TunnelMan* m_player;
    int m_numBarrels;
    
    void populateFieldWithEarth();
    void populateFieldWithBoulders();
    void populateFieldWithBarrels();
//    void populateFieldWithNuggets();
    
    bool thereAreObjectsTooClose(int x, int y);
    bool playerCompletedLevel(); 
    
    double distanceApart(int x, int y, int x2, int y2); 
    bool inTunnel(int x, int y) const; 
//    void updateDisplayText();
//    std::string formatStats();

};

#endif // STUDENTWORLD_H_
