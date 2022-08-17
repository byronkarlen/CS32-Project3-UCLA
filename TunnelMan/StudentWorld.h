#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>


//Constants:
const int actorSize = 4;

class Actor;
class Earth;
class TunnelMan;
class Gold;
class WaterPool;
class SonarKit;
class Barrel;
class Boulder;
class Squirt;
class RegularProtestor;
class HardCoreProtestor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);

    virtual int init();

    virtual int move();

    virtual void cleanUp();
    
    virtual ~StudentWorld(){}
    
    //Additional Functions:
    
    //adds an actor to the field (except for Earth) to the field
    void addActor(Actor* a);
    //removes an actor (except for Earth and TunnelMan) from a location at the field
    void removeActor(Actor* a);
    
    //returns whether the tunnelman is within the given radius
    bool tunnelManWithinRadius(int x, int y, int radius);
    
    //Returns a pointer to the TunnelMan
    TunnelMan* getTunnelMan() const;
    
    //Returns a vector of all the actors with the given characterID within the given radius
    std::vector<Actor*> findActorsWithinRadius(int x, int y, int radius, char charID);
    
    //Returns a vector of all the protestors within the given radius
    std::vector<Actor*> findProtestorsWithinRadius(int x, int y, int radius);
    
    //removes earth objects within a 4x4 location specified by the bottom left corner. If there is no earth object at the given location, it does nothing
    void removeEarth(int x, int y);
    
    //returns whether there are earth objects within a given location 4x4 location specfied by the bottom left corner. If the location is not valid, it returns false anyways
    bool earthAt(int x, int y) const;
    
    //returns whether an actor located at the given location will overlap with a boulder
    bool actorWillOverlapBoulder(int x, int y) const;
    
    //returns whether there is a boulder object within a radius of 3 of the given location
    bool boulderWithinRadius3(int x, int y) const;
    
    //annoys all active protestors present within the given radius
    bool killProtestorsWithinRadius(int x, int y, int radius);
    
    //annoys a single active protestor present within the given radius
    bool squirtProtestorWithinRadius(int x, int y, int radius);
    
    //illuminates all actors in the oil field within the given radius
    void illuminateOilField(int x, int y, int radius);
    
    //bribes a single protestor present within the radius
    void bribeProtestor(int x, int y, int radius);
    
    GraphObject::Direction getDirectionToLocation(Actor* p, int x, int y);
    bool hardCoreProtestorCanMoveTowardTunnelMan(Actor* p); 
    
    bool willHitBoulderOrEdge(int x, int y, GraphObject::Direction d);
    bool willHitBoulderEdgeOrEarth(int x, int y, GraphObject::Direction d);

    

    //returns whether a given coordinate is located within the game's playing field
    bool inField(int x, int y) const;
    
    //returns whether an actor located at the given coordinate would be entirely within the game's playing field
    bool actorWouldBeWithinField(int x, int y) const;
    
    
    
private:
    Earth* m_earthTracker[VIEW_HEIGHT][VIEW_WIDTH];
    std::vector<Actor*> m_gameObjects;
    TunnelMan* m_player;
    int m_numBarrels;
    
    int m_minTicksBetweenProtestors;
    int m_ticksSinceLastProtestorAdded;
    int m_targetNumOfProtestors;
    int m_numProtestors;
    
    
    int m_maze[VIEW_HEIGHT][VIEW_WIDTH];
    struct mazeLocation{
        int x;
        int y;
        mazeLocation(int a, int b){
            x = a;
            y = b;
        }
    };
    
    void populateFieldWithEarth();
    void populateFieldWithBoulders();
    void populateFieldWithBarrels();
    void populateFieldWithNuggets();
    
    bool thereAreObjectsTooClose(int x, int y);
    bool playerCompletedLevel();
    
    double distanceApart(int x, int y, int x2, int y2) const;
    bool nearTunnel(int x, int y) const;
    void updateDisplayText();
    std::string formatStats(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);

    bool boulderAt(int x, int y) const;
    
};

#endif // STUDENTWORLD_H_

