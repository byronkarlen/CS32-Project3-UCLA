#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cmath>
using namespace std;

//PUBLIC STUDENTWORLD INTERFACE

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){}

int StudentWorld::init()
{
    //set earth tracker to have all nullptrs:
    for(int i = 0; i < VIEW_HEIGHT-4; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            m_earthTracker[i][j] = nullptr;
        }
    }
    Earth* temp;
    //fill rows 0 through 59 of the oil field with Earth Objects (with exception of vertical shafts)
    for(int col = 0; col < VIEW_WIDTH; col++){
        for(int row = 0; row < VIEW_HEIGHT-4; row++){
            
            //If the current location falls outside of the central tunnel
            if( (col < 30 || col > 33) || row < 4) {
                temp = new Earth(this, col, row); //Create a new earth at the given location
                temp->setVisible(true); //Make the earth visible
                m_earthTracker[row][col] = temp; //Add the earth pointer to the earthTracker
                m_gameRepresentation[row][col].push_back('E'); //The character 'E' represents an earth at the given location in the field
            }
        }
    }
    
    //Create the tunnelman
    m_player = new TunnelMan(this); //Create a new TunnelMan
    m_player->setVisible(true); //Make the TunnelMan visible
    m_gameRepresentation[60][30].push_back('T'); //The character 'T' represents the TunnelMan at the given location in the field

    //Create a sample boulder
    
    Boulder* tempB = new Boulder(this, 20, 20);
    addActor(tempB);
    tempB->setVisible(true);
    
    return GWSTATUS_CONTINUE_GAME; //must return this to continue the game
}

int StudentWorld::move(){
    
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getLiveStatus())
            (*it)->doSomething();
    }
    
    if(m_player->getLiveStatus()){
        m_player->doSomething();
        return GWSTATUS_CONTINUE_GAME;
    }
    else{
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

}

void StudentWorld::cleanUp(){
    delete m_player;
    m_player = nullptr;
    for(int i = 0; i < VIEW_HEIGHT-4; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            delete m_earthTracker[i][j];
            m_earthTracker[i][j] = nullptr;
        }
    }
}

StudentWorld::~StudentWorld(){
    delete m_player;
    m_player = nullptr;
    for(int i = 0; i < VIEW_HEIGHT-4; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            delete m_earthTracker[i][j];
            m_earthTracker[i][j] = nullptr;
        }
    }
}

bool StudentWorld::removeEarth(int x, int y){
    if(x < 0 || x > 63)
        return false;
    if(y < 0 || y > 59)
        return false;
    
    if(m_earthTracker[y][x] == nullptr)
        return false;
    
    if(!spotContains(x, y, 'E'))
        return false;
    
    delete m_earthTracker[y][x];
    m_earthTracker[y][x] = nullptr;
    
    vector<char>::iterator it;
    for(it = m_gameRepresentation[y][x].begin(); it != m_gameRepresentation[y][x].end(); it++){
        if(*it == 'E'){
            m_gameRepresentation[y][x].erase(it);
            break;
        }
    }
    
    return true;
}

void StudentWorld::moveActor(Actor* a){
    int x = a->getX();
    int y = a->getY();
    
    if(a->getDirection() == GraphObject::up){
        if(y < 63){
            a->moveTo(x, y+1);
            removeActorFromGameRepresentation(x, y, a->getID());
            addActorToGameRepresentation(x, y+1, a->getID());
        }
    }
    if(a->getDirection() == GraphObject::down){
        if(y > 0){
            a->moveTo(x, y-1);
            removeActorFromGameRepresentation(x, y, a->getID());
            addActorToGameRepresentation(x, y-1, a->getID());
        }
    }
    if(a->getDirection() == GraphObject::left){
        if(x > 0){
            a->moveTo(x-1, y);
            removeActorFromGameRepresentation(x, y, a->getID());
            addActorToGameRepresentation(x-1, y, a->getID());
        }
    }
    if(a->getDirection() == GraphObject::right){
        if(x < 63){
            a->moveTo(x+1, y);
            removeActorFromGameRepresentation(x, y, a->getID());
            addActorToGameRepresentation(x+1, y, a->getID());
        }
    }
        
}

bool StudentWorld::spotContains(int x, int y, char c) const{
    for(int i = 0; i < m_gameRepresentation[y][x].size(); i++){
        if(m_gameRepresentation[y][x][i] == c)
            return true;
    }
    return false;
}

void StudentWorld::removeActor(Actor* a){
    removeActorFromGameRepresentation(a->getX(), a->getY(), a->getID());
    removeActorFromGameObjects(a);
}

void StudentWorld::addActor(Actor* a){
    addActorToGameRepresentation(a->getX(), a->getY(), a->getID());
    addActorToGameObjects(a);
}

int StudentWorld::getPlayerX() const{
    return m_player->getX();
}
int StudentWorld::getPlayerY() const{
    return m_player->getY();
}

Actor* StudentWorld::findActor(int x, int y, char c) const{
    list<Actor*>::const_iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getX() == x && (*it)->getY() == y && (*it)->getID() == c)
            return *it;
    }
    return nullptr;
}

//PRIVATE STUDENTWORLD FUNCTIONS
void StudentWorld::removeActorFromGameRepresentation(int x, int y, char c){
    vector<char>::iterator it;
    for(it = m_gameRepresentation[y][x].begin(); it != m_gameRepresentation[y][x].end(); it++){
        if(*it == c){
            m_gameRepresentation[y][x].erase(it);
            break;
        }
    }
}

void StudentWorld::addActorToGameRepresentation(int x, int y, char c){
    m_gameRepresentation[y][x].push_back(c);
}

void StudentWorld::removeActorFromGameObjects(Actor *a){
    for(list<Actor*>::iterator it2 = m_gameObjects.begin(); it2 != m_gameObjects.end(); it2++){
        if(*it2 == a){
            m_gameObjects.erase(it2);
            break;
        }
    }
}

void StudentWorld::addActorToGameObjects(Actor* a){
    m_gameObjects.push_back(a);
}


//double StudentWorld::distanceBetween(Actor* a1, Actor* a2) const{
//    double xDiff = a1->getX() - a2->getX();
//    double yDiff = a1->getY() - a2->getY();
//
//    return sqrt((xDiff * xDiff) + (yDiff * yDiff));
//}
//




//void StudentWorld::setDisplayText() const{
//    int level = getCurrentGameLevel();
//    int lives = getNumLivesLeft();
//    int health = getCurrentHealth();
//    int squirts = getSquirtsLeftInSquirtGun();
//    int gold = getPlayerGoldCount();
//    int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
//    int sonar = getPlayerSonarChargeCount();
//    int score = getScore();
//
//    string s = ...
//
//}

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}
 
