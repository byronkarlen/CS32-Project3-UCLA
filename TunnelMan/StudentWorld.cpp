#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cmath>
#include <cstdlib>

using namespace std;

//PUBLIC STUDENTWORLD INTERFACE

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){}

int StudentWorld::init()
{
    populateFieldWithEarth();

    populateFieldWithBoulders();

    populateFieldWithNuggets();
    
    populateFieldWithBarrels();

    //Create the tunnelman
    m_player = new TunnelMan(this); //Create a new TunnelMan
    
    return GWSTATUS_CONTINUE_GAME; //must return this to continue the game
}

int StudentWorld::move(){
//    updateDisplayText();
    
    if(m_player->getLiveStatus())
        m_player->doSomething();
    
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getLiveStatus()){
            (*it)->doSomething();
            if(!m_player->getLiveStatus()){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(playerCompletedLevel()){
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
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

TunnelMan* StudentWorld::getTunnelMan() const{
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

bool StudentWorld::inField(int x, int y) const{
    if(x < 0 || x > VIEW_WIDTH)
        return false;
    if(y < 0 || y > VIEW_WIDTH)
        return false;
    return true;
}

//Private StudentWorld Functions
void StudentWorld::populateFieldWithEarth(){
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
}

void StudentWorld::populateFieldWithBoulders(){
    int numBoulders;
    int alt = getLevel() / 2 + 2;
    if(alt < 9)
        numBoulders = alt;
    else
        numBoulders = 0;
    assert(numBoulders == 2);
    
    for(int i = 0; i < numBoulders; i++){
        int x, y;
        do{
            x = rand() % 54 + 1;
            y = rand() % 35 + 20;
        }while(thereAreObjectsTooClose(x, y));
        
        Boulder* b = new Boulder(this, x, y);
        addActor(b);
    }
}

void StudentWorld::populateFieldWithNuggets(){
    int numNuggets;
    int alt = 5 - getLevel() / 2;
    if(alt > 2)
        numNuggets = alt;
    else
        numNuggets = 2;
    
    for(int i = 0; i < numNuggets; i++){
        int x, y;
        do{
            x = rand() % 60;
            y = rand() % 56;
        }while(thereAreObjectsTooClose(x, y));
        
        Gold* g = new Gold(this, x, y, false, -1);
        addActor(g);
    }
}

void StudentWorld::populateFieldWithBarrels(){
    int numBarrels;
    int alt = 2 + getLevel();
    if(alt < 21)
        numBarrels = alt;
    else
        numBarrels = 21;
    
    for(int i = 0; i < numBarrels; i++){
        int x, y;
        do{
            x = rand() & 60;
            y = rand() % 56;
        }while(thereAreObjectsTooClose(x, y));

        
        Barrel* o = new Barrel(this, x, y);
        addActor(o);
    }
}

bool StudentWorld::thereAreObjectsTooClose(int x, int y){
    list<Actor*>::iterator it;
    
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
    {
        int x2 = (*it)->getX();
        int y2 = (*it)->getY();
        
        int total = (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y);
        if(sqrt(total) < 6)
            return true;
    }
    return false;
}

bool StudentWorld::playerCompletedLevel(){
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getGameID() == 'O')
            return false;
    }
    return true;
}

//void StudentWorld::updateDisplayText(){
//    int level = getLevel();
//    int lives = getLives();
//    int health = getCurrentHealth();
//    int squirts = getSquirtsLeftInSquirtGun();
//    int gold = getPlayerGoldCount();
//    int barrelsLeft = getNumberOfBarrelsRemainingToBePickedUp();
//    int sonar = getPlayerSonarChargeCount();
//    int score = getScore();
//    // Next, create a string from your statistics, of the form: // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
//              string s = someFunctionYouUseToFormatThingsNicely(level, lives, health,
//                                         squirts, gold, barrelsLeft, sonar, score);
//    // Finally, update the display text at the top of the screen with your // newly created stats
//    setGameStatText(s); // calls our provided GameWorld::setGameStatText
//}

//string StudentWorld::formatStats(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score){
//
//}


GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}
 
