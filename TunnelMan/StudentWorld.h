#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
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
    Actor* getTunnelMan() const;
    
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
    
    //returns whether a given coordinate is located within the game's playing field

};

#endif // STUDENTWORLD_H_

 



















































//#ifndef STUDENTWORLD_H_
//#define STUDENTWORLD_H_
//
//#include "GameWorld.h"
//#include "GameConstants.h"
//#include <string>
//#include <vector>
//#include <list>
//
//class Actor;
//class Earth;
//class TunnelMan;
//class Boulder;
//class Squirt;
//// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//
//class StudentWorld : public GameWorld
//{
//public:
//    StudentWorld(std::string assetDir);
//
//    virtual int init();
//
//    virtual int move();
//
//    virtual void cleanUp();
//
//    virtual ~StudentWorld();
//
//    //Additional Functions:
//
//    //returns whether an actor is located within a 4x4 area of the field
//    bool spotContains4(int x, int y, char c) const;
//    //returns whether an actor is at a location in the field
//    bool spotContains(int x, int y, char c, double size = 1.0) const;
//
//    //removes an actor (except for Earth and TunnelMan) from a location at the field
//    void removeActor(Actor* a);
//
//    //Adds an actor to the field
//    void addActor(Actor* a);
//
//    //removes earth object at the specified location
//    bool removeEarth(int x, int y);
//
//    //moves the actor one square in the field in the direction it is faced
//    void moveActor(Actor* a);
//
//    //Returns an actor (with the charID given) who is present at the given location in the field
//    Actor* findActor(int x, int y, char c, double size=1.0) const;
//
//    int getPlayerX() const;
//    int getPlayerY() const;
//
//
//
//
//private:
//    std::vector<char> m_gameRepresentation[VIEW_HEIGHT][VIEW_WIDTH];
//    Earth* m_earthTracker[VIEW_HEIGHT-4][VIEW_WIDTH];
//    std::list<Actor*> m_gameObjects;
//    TunnelMan* m_player;
//
//    //Return the distance between two objects in the field
//    double distanceBetween(Actor* a1, Actor* a2) const;
//
//    void addActorToGameObjects(Actor* a);
//    void removeActorFromGameRepresentation(int x, int y, char c);
//    void removeActorFromGameObjects(Actor* a);
//    void addActorToGameRepresentation(int x, int y, char c);
//
//    bool inField(int x, int y) const;
////    void setDisplayText() const;
//};
//
//#endif // STUDENTWORLD_H_
