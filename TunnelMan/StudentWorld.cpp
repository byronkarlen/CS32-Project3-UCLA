#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <queue>
#include <cmath>
#include <cstdlib>

using namespace std;

//PUBLIC STUDENTWORLD INTERFACE

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){}

int StudentWorld::init()
{
    //Populate field with objects that start out:
    populateFieldWithEarth();
    populateFieldWithBoulders();
    populateFieldWithBarrels();
    populateFieldWithNuggets();
    


    //Set appropriate tick counts
    m_numProtestors = 0;
    m_ticksSinceLastProtestorAdded = 1000; //So that a protestor will be added on the very first tick
    m_minTicksBetweenProtestors = fmax(25, 200 - getLevel());
    m_targetNumOfProtestors = fmin(15, 2 + getLevel() * 1.5);
    
    
    //Create the tunnelman
    m_player = new TunnelMan(this); //Create a new TunnelMan
    
    
    return GWSTATUS_CONTINUE_GAME; //must return this to continue the game
}

int StudentWorld::move(){
    updateDisplayText();
    
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
    
    
    //Add protestors if needed
    if(m_ticksSinceLastProtestorAdded > m_minTicksBetweenProtestors && m_numProtestors < m_targetNumOfProtestors){
        
        int p = fmin(90, getLevel()*10 + 30);
        int i = rand() % 100 + 1;
        if(i <= p){ 
            addActor(new HardCoreProtestor(this));
        }
        else{
            addActor(new RegularProtestor(this));
        }
        
        m_ticksSinceLastProtestorAdded = -1;
        m_numProtestors++;
    }
    //UPDATE TICK COUNTS
    m_ticksSinceLastProtestorAdded++;
    
    
    //Add sonar/water if needed
    int g = getLevel() * 25 + 300;
    int i = rand() % g;
    if(i == 0){
        int j = rand() % 5;
        if(j == 0){//Add a Sonarkit
            Actor* a = new SonarKit(this, 0, 60);
            addActor(a);
        }
        else{//Add a waterpool
            
            int x;
            int y;
            do{
                x = rand() % 61;
                y = rand() % 61;
            }while(earthAt(x, y));
            
            Actor* a = new Waterpool(this, x, y);
            addActor(a);
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

bool StudentWorld::actorAt(int x, int y, char c) const{
    
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(findActor(x+j, y+i, c) != nullptr)
                return true;
        }
    }
    return false;
    
}


void StudentWorld::removeEarth(int x, int y){
    
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(inField(x+j, y+i) && m_earthTracker[y+i][x+j] != nullptr){
                delete m_earthTracker[y+i][x+j];
                m_earthTracker[y+i][x+j] = nullptr;
            }
        }
    }
}

bool StudentWorld::earthAt(int x, int y) const{
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(inField(x+j, y+i) && m_earthTracker[y+i][x+j] != nullptr){
                return true;
            }
        }
    }
    return false;
}

void StudentWorld::illuminateOilField(int x, int y, int radius){
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) < radius)
            (*it)->setVisible(true);
    }
}

bool StudentWorld::actorWithinRadius(int x, int y, int radius, char GameID){
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getGameID() == GameID && (*it)->getLiveStatus()){
            if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) <= radius)
                return true;
        }
    }
    return false;
}

Actor* StudentWorld::findActorWithinRadius(int x, int y, int radius, char c){
    list<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getGameID() == c && (*it)->getLiveStatus()){
            if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) <= radius)
                return (*it);
        }
    }
    return nullptr;
}

bool StudentWorld::tunnelManWithinRadius(int x, int y, int radius  ){
    if(distanceApart(x, y, getPlayerX(), getPlayerY()) <= radius)
        return true;
    else
        return false;
}

bool StudentWorld::annoyProtestorsWithinRadius(int x, int y, int radius, int howMuch){
    list<Actor*>::iterator it;
    bool output = false;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if((*it)->getLiveStatus() && (*it)->isAnnoyable() && ((*it)->getGameID() == 'p' || (*it)->getGameID() == 'P')){
            if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) <= radius){
                (*it)->annoy(howMuch);
                    output = true;
            }
        }
    }
    return output;
}

//Actor* StudentWorld::findActorWithinRadius(int x, int y, int radius, char GameID){
//    list<Actor*>::iterator it;
//    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
//        if((*it)->getGameID() == GameID && (*it)->getLiveStatus()){
//            if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) <= radius)
//                return (*it);
//        }
//    }
//    return nullptr;
//}
               
double StudentWorld::distanceApart(int x, int y, int x2, int y2){
    int diffX = x2 - x;
    int diffY = y2 - y;
    int total = diffX * diffX + diffY * diffY;
    return sqrt(total);
}

void StudentWorld::addActor(Actor* a){
    m_gameObjects.push_back(a);
}

void StudentWorld::getMazeForExit(char maze[64][64]){
    
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            if(m_earthTracker[i][j] != nullptr){
                maze[i][j] = 'E';
            }
            else if(actorAt(j, i, 'B')){
                maze[i][j] = 'B';
            }
            else
                maze[i][j] = '.';
        }
    }
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
    if(x < 0 || x >= VIEW_WIDTH)
        return false;
    if(y < 0 || y >= VIEW_HEIGHT)
        return false;
    return true;
}

bool StudentWorld::actorInField(int x, int y)const{
    if(x < 0 || (x+actorSize) > VIEW_WIDTH)
        return false;
    if (y < 0 || (y+actorSize) > VIEW_HEIGHT)
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
    assert(!earthAt(64,63));
}

void StudentWorld::populateFieldWithBoulders(){
    int numBoulders;
    int alt = getLevel() / 2 + 2;
    if(alt < 9)
        numBoulders = alt;
    else
        numBoulders = 0;
    
    
    for(int i = 0; i < numBoulders; i++){
        int x, y;
        do{
            x = rand() % 54 + 1;
            y = rand() % 35 + 20;
        }while(thereAreObjectsTooClose(x, y) || nearTunnel(x, y));
        
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
        
        Gold* g = new Gold(this, x, y, true, true, -1);
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
    
    m_numBarrels = numBarrels;
    
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
        if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) <= 6)
            return true;
    }
    return false;
}

bool StudentWorld::playerCompletedLevel(){
    return m_player->getNumBarrelsFound() == m_numBarrels;
}

bool StudentWorld::nearTunnel(int x, int y) const{
    if((x >= 26 && x <= 34) && (y >= 4))
        return true;
    else
        return false;
}

void StudentWorld::updateDisplayText(){
    int level = getLevel();
    int lives = getLives();
    int health = m_player->getHitPoints() * 10;
    int squirts = m_player->getNumWater();
    int gold = m_player->getNumNuggets();
    int barrelsLeft = m_numBarrels - m_player->getNumBarrelsFound();
    int sonar = m_player->getNumSonarCharges();
    int score = getScore();
    // Next, create a string from your statistics, of the form: // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
    string s = formatStats(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
    // Finally, update the display text at the top of the screen with your // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatStats(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score){
    return "Lvl: " + to_string(level) + " Lives: " + to_string(lives) + " Hlth: " + to_string(health) + "% Wtr: " + to_string(squirts) + " Gld: " + to_string(gold) + " Oil Left: " + to_string(barrelsLeft) + " Sonar: " + to_string(sonar) + " Scr: " + to_string(score);
}


GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}
 

