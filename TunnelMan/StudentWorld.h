#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <list>

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
    
    virtual ~StudentWorld();
    
    //Additional Functions:
    
    //returns whether an actor is at a location in the field
    bool spotContains(int x, int y, char c) const;
    
    //removes an actor (except for Earth and TunnelMan) from a location at the field
    void removeActor(Actor* a);
    
    //Adds an actor to the field
    void addActor(Actor* a);

    //removes earth object at the specified location
    bool removeEarth(int x, int y);
    
    //moves the actor one square in the field in the direction it is faced
    void moveActor(Actor* a);
    
    Actor* findActor(int x, int y, char c) const;
    
    int getPlayerX() const;
    int getPlayerY() const;
    
    

    
    //returns whether or not a boulder can fall -- i.e. if there is earth (or the bottom of the screen) beneath it
//    bool boulderCanFall(Boulder* b) const;
    
    //returns whether a squirt can continue moving one square in its direction
//    bool squirtCanContinueMoving(Squirt* s) const;
    
    
    


private:
    //Actor* m_gameRepresentation[VIEW_HEIGHT][VIEW_WIDTH];
    //std::string m_gameRepresentation[VIEW_HEIGHT][VIEW_WIDTH];
    std::vector<char> m_gameRepresentation[VIEW_HEIGHT][VIEW_WIDTH];
    Earth* m_earthTracker[VIEW_HEIGHT-4][VIEW_WIDTH];
    std::list<Actor*> m_gameObjects;
    TunnelMan* m_player;
    
    //Return the distance between two objects in the field
    double distanceBetween(Actor* a1, Actor* a2) const;
    
    void addActorToGameObjects(Actor* a);
    void removeActorFromGameRepresentation(int x, int y, char c);
    void removeActorFromGameObjects(Actor* a);
    void addActorToGameRepresentation(int x, int y, char c);
    
//    void setDisplayText() const;
};

#endif // STUDENTWORLD_H_

 


