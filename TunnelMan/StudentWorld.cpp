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
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            m_earthTracker[i][j] = nullptr;
        }
    }
    Earth* temp;
    //fill rows 0 through 59 of the oil field with Earth Objects (with exception of vertical shafts)
    for(int col = 0; col < VIEW_WIDTH; col++){
        for(int row = 0; row < VIEW_HEIGHT-4; row++){
            //If the current location falls outside of the central tunnel
            if((col < 30 || col > 33) || row < 4) {
                temp = new Earth(this, col, row); //Create a new earth at the given location
                m_earthTracker[row][col] = temp; //Add the earth pointer to the earthTracker
            }
        }
    }
    
    Boulder* b = new Boulder(this, 20, 40);
    addActor(b);

    Barrel* o = new Barrel(this, 50, 50);
    addActor(o);

    //Create the tunnelman
    m_player = new TunnelMan(this); //Create a new TunnelMan
    
    return GWSTATUS_CONTINUE_GAME; //must return this to continue the game
}

int StudentWorld::move(){
    
    if(m_player->getLiveStatus())
        m_player->doSomething();
    
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getLiveStatus())
            (*it)->doSomething();
    }

    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if(!(*it)->getLiveStatus()){
            removeActor(*it);
        }
    }
    
    if(m_player->getLiveStatus())
        return GWSTATUS_CONTINUE_GAME;
    else{
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
}

void StudentWorld::cleanUp(){
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        delete *it;
        m_gameObjects.erase(it);
    }
    
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            delete m_earthTracker[i][j];
        }
    }
    
    delete m_player;
}

void StudentWorld::removeEarth(int x, int y){
    if(!inField(x, y))
        return;
    
    if(m_earthTracker[y][x] == nullptr)
        return;
    
    delete m_earthTracker[y][x];
    m_earthTracker[y][x] = nullptr;
}

bool StudentWorld::isEarthAt(int x, int y) const{
    if(!inField(x, y))
        return false;
    return m_earthTracker[y][x] != nullptr;
}

void StudentWorld::addActor(Actor* a){
    m_gameObjects.push_back(a);
}

void StudentWorld::removeActor(Actor* a){
    list<Actor*>::iterator it;
    it = m_gameObjects.begin();
    while(it != m_gameObjects.end()){
        if(*it == a){
            delete *it;
            it = m_gameObjects.erase(it);
        }
        else{
            it++;
        }
    }
}

Actor* StudentWorld::findActor(int x, int y, char c) const{
    
    list<Actor*>::const_iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        
        for(int i = 0; i < actorSize; i++){
            for(int j = 0; j < actorSize; j++){
                
                if(inField(x, y)){
                    if((*it)->getX() == x-j && (*it)->getY() == y-i && (*it)->getGameID() == c)
                        return *it;
                }
                
            }
        }
        
    }
    return nullptr;
}

Actor* StudentWorld::getTunnelMan() const{
    return m_player;
}

bool StudentWorld::isTunnelManAt(int x, int y) const{
    
    int xLoc = m_player->getX();
    int yLoc = m_player->getY();
    
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(x-j == xLoc && y-i == yLoc)
                return true;
        }
    }
    return false;
}

int StudentWorld::getPlayerX() const{
    return m_player->getX();
}
int StudentWorld::getPlayerY() const{
    return m_player->getY();
}

//PRIVATE STUDENTWORLD FUNCTIONS

bool StudentWorld::inField(int x, int y) const{
    if(x < 0 || x > VIEW_WIDTH)
        return false;
    if(y < 0 || y > VIEW_WIDTH)
        return false;
    return true;
}





GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}
 



















































//#include "StudentWorld.h"
//#include "Actor.h"
//#include <string>
//#include <cmath>
//using namespace std;
//
////PUBLIC STUDENTWORLD INTERFACE
//
//StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){}
//
//int StudentWorld::init()
//{
//
//    //Create a sample boulder
//
//    Boulder* tempB = new Boulder(this, 20, 40);
//    addActor(tempB);
//    tempB->setVisible(true);
//
//
//    //set earth tracker to have all nullptrs:
//    for(int i = 0; i < VIEW_HEIGHT-4; i++){
//        for(int j = 0; j < VIEW_WIDTH; j++){
//            m_earthTracker[i][j] = nullptr;
//        }
//    }
//    Earth* temp;
//    //fill rows 0 through 59 of the oil field with Earth Objects (with exception of vertical shafts)
//    for(int col = 0; col < VIEW_WIDTH; col++){
//        for(int row = 0; row < VIEW_HEIGHT-4; row++){
//
//            //If the current location falls outside of the central tunnel
//            if(((col < 30 || col > 33) || row < 4) && !spotContains(col, row, 'B')) {
//                temp = new Earth(this, col, row); //Create a new earth at the given location
//                temp->setVisible(true); //Make the earth visible
//                m_earthTracker[row][col] = temp; //Add the earth pointer to the earthTracker
//                m_gameRepresentation[row][col].push_back('E'); //The character 'E' represents an earth at the given location in the field
//            }
//        }
//    }
//
//
//    //Create the tunnelman
//    m_player = new TunnelMan(this); //Create a new TunnelMan
//    m_player->setVisible(true); //Make the TunnelMan visible
//    m_gameRepresentation[60][30].push_back('T'); //The character 'T' represents the TunnelMan at the given location in the field
//
//    assert(findActor(30, 60, 'T') == nullptr);
//
//    return GWSTATUS_CONTINUE_GAME; //must return this to continue the game
//}
//
//int StudentWorld::move(){
//
//    if(m_player->getLiveStatus())
//        m_player->doSomething();
//
//    list<Actor*>::iterator it;
//    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
//        if((*it)->getLiveStatus())
//            (*it)->doSomething();
//    }
//
//    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
//        if(!(*it)->getLiveStatus())
//            removeActor(*it);
//    }
//
//    if(m_player->getLiveStatus())
//        return GWSTATUS_CONTINUE_GAME;
//    else{
//        decLives();
//        return GWSTATUS_PLAYER_DIED;
//    }
//
//}
//
//void StudentWorld::cleanUp(){
//    delete m_player;
//    m_player = nullptr;
//    for(int i = 0; i < VIEW_HEIGHT-4; i++){
//        for(int j = 0; j < VIEW_WIDTH; j++){
//            delete m_earthTracker[i][j];
//            m_earthTracker[i][j] = nullptr;
//        }
//    }
//}
//
//StudentWorld::~StudentWorld(){
//    delete m_player;
//    m_player = nullptr;
//    for(int i = 0; i < VIEW_HEIGHT-4; i++){
//        for(int j = 0; j < VIEW_WIDTH; j++){
//            delete m_earthTracker[i][j];
//            m_earthTracker[i][j] = nullptr;
//        }
//    }
//}
//
//bool StudentWorld::removeEarth(int x, int y){
//    if(x < 0 || x > 63)
//        return false;
//    if(y < 0 || y > 59)
//        return false;
//
//    if(m_earthTracker[y][x] == nullptr)
//        return false;
//
//    if(!spotContains(x, y, 'E'))
//        return false;
//
//    delete m_earthTracker[y][x];
//    m_earthTracker[y][x] = nullptr;
//
//    vector<char>::iterator it;
//    for(it = m_gameRepresentation[y][x].begin(); it != m_gameRepresentation[y][x].end(); it++){
//        if(*it == 'E'){
//            m_gameRepresentation[y][x].erase(it);
//            break;
//        }
//    }
//
//    return true;
//}
//
//void StudentWorld::moveActor(Actor* a){
//    int x = a->getX();
//    int y = a->getY();
//
//    if(a->getDirection() == GraphObject::up){
//        if(y < 63){
//            a->moveTo(x, y+1);
//            removeActorFromGameRepresentation(x, y, a->getGameID());
//            addActorToGameRepresentation(x, y+1, a->getGameID());
//        }
//    }
//    if(a->getDirection() == GraphObject::down){
//        if(y > 0){
//            a->moveTo(x, y-1);
//            removeActorFromGameRepresentation(x, y, a->getGameID());
//            addActorToGameRepresentation(x, y-1, a->getGameID());
//        }
//    }
//    if(a->getDirection() == GraphObject::left){
//        if(x > 0){
//            a->moveTo(x-1, y);
//            removeActorFromGameRepresentation(x, y, a->getGameID());
//            addActorToGameRepresentation(x-1, y, a->getGameID());
//        }
//    }
//    if(a->getDirection() == GraphObject::right){
//        if(x < 63){
//            a->moveTo(x+1, y);
//            removeActorFromGameRepresentation(x, y, a->getGameID());
//            addActorToGameRepresentation(x+1, y, a->getGameID());
//        }
//    }
//
//}
//bool StudentWorld::spotContains4(int x, int y, char c) const{
//    for(int i = 0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            if(spotContains(x+j, y+i, c)){
//                return true;
//            }
//        }
//    }
//    return false;
//}
//bool StudentWorld::spotContains(int x, int y, char c, double size) const{
//    int nSquares = size*4;
//    for(int j = 0; j < nSquares; j++){
//        for(int k = 0; k < nSquares; k++){
//            if(inField(x-k, y-j)){
//                for(int i = 0; i < m_gameRepresentation[y-j][x-k].size(); i++){
//                    if(m_gameRepresentation[y-j][x-k][i] == c)
//                        return true;
//                }
//            }
//        }
//    }
//
//    return false;
//}
//
//
//
//
//void StudentWorld::removeActor(Actor* a){
//    removeActorFromGameRepresentation(a->getX(), a->getY(), a->getGameID());
//    removeActorFromGameObjects(a);
//}
//
//void StudentWorld::addActor(Actor* a){
//    addActorToGameRepresentation(a->getX(), a->getY(), a->getGameID());
//    addActorToGameObjects(a);
//}
//
//int StudentWorld::getPlayerX() const{
//    return m_player->getX();
//}
//int StudentWorld::getPlayerY() const{
//    return m_player->getY();
//}
//
//Actor* StudentWorld::findActor(int x, int y, char c, double size) const{
//    int nSquares = size*4;
//    for(int i = 0; i < nSquares; i++){
//        for(int j = 0; j < nSquares; j++){
//            if(inField(x-j, y-i)){
//                for(int k = 0; k < m_gameRepresentation[y-i][x-j].size(); k++){
//                    if(m_gameRepresentation[y-i][x-j][k] == c){
//                        list<Actor*>::const_iterator it;
//                        for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
//                            if((*it)->getX() == x && (*it)->getY() == y && (*it)->getGameID() == c)
//                                return *it;
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return nullptr;
//}
//
////PRIVATE STUDENTWORLD FUNCTIONS
//void StudentWorld::removeActorFromGameRepresentation(int x, int y, char c){
//    vector<char>::iterator it;
//    for(it = m_gameRepresentation[y][x].begin(); it != m_gameRepresentation[y][x].end(); it++){
//        if(*it == c){
//            m_gameRepresentation[y][x].erase(it);
//            break;
//        }
//    }
//}
//
//void StudentWorld::addActorToGameRepresentation(int x, int y, char c){
//    m_gameRepresentation[y][x].push_back(c);
//}
//
//void StudentWorld::removeActorFromGameObjects(Actor *a){
//    for(list<Actor*>::iterator it2 = m_gameObjects.begin(); it2 != m_gameObjects.end(); it2++){
//        if(*it2 == a){
//            delete a;
//            m_gameObjects.erase(it2);
//            break;
//        }
//    }
//}
//
//void StudentWorld::addActorToGameObjects(Actor* a){
//    m_gameObjects.push_back(a);
//}
//
//bool StudentWorld::inField(int x, int y) const{
//    if(x < 0 || x > VIEW_WIDTH)
//        return false;
//    if(y < 0 || y > VIEW_WIDTH)
//        return false;
//    return true;
//}
//
//
////double StudentWorld::distanceBetween(Actor* a1, Actor* a2) const{
////    double xDiff = a1->getX() - a2->getX();
////    double yDiff = a1->getY() - a2->getY();
////
////    return sqrt((xDiff * xDiff) + (yDiff * yDiff));
////}
////
//
//
//
//
//
//GameWorld* createStudentWorld(string assetDir)
//{
//    return new StudentWorld(assetDir);
//}
//
//
//
