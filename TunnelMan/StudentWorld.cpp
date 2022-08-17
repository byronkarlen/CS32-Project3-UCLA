#include "StudentWorld.h"
#include "Actor.h"
#include <cmath>
#include <queue>
//#include <cstdlib>

using namespace std;

//PUBLIC STUDENTWORLD INTERFACE

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir){}

int StudentWorld::init()
{
    //Populate field with objects that start out:
    populateFieldWithEarth();
    populateFieldWithBoulders();
    populateFieldWithBarrels();
//    populateFieldWithNuggets();
    


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
    
    for(int i = 0; i != m_gameObjects.size(); i++){
    
        if(m_gameObjects[i]->getLiveStatus()){
            m_gameObjects[i]->doSomething();
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

    for(int i = 0; i != m_gameObjects.size(); i++){
        if(!m_gameObjects[i]->getLiveStatus()){
            removeActor(m_gameObjects[i]);
            i--; 
        }
    }
    
    
    //Add protestors if needed
    if(m_ticksSinceLastProtestorAdded > m_minTicksBetweenProtestors && m_numProtestors < m_targetNumOfProtestors){
        
        int p = fmin(90, getLevel()*10 + 30);
        int i = rand() % 100 + 1;
        if(i <= p){
            addActor(new HardcoreProtestor(this));
        }
        else{
            addActor(new RegularProtestor(this));
        }
        m_ticksSinceLastProtestorAdded = -1;
        m_numProtestors++;
    }
    //UPDATE TICK COUNTS
    m_ticksSinceLastProtestorAdded++;
//    
//    
//    //Add sonar/water if needed
//    int g = getLevel() * 25 + 300;
//    int i = rand() % g;
//    if(i == 0){
//        int j = rand() % 5;
//        if(j == 0){//Add a Sonarkit
//            Actor* a = new SonarKit(this, 0, 60);
//            addActor(a);
//        }
//        else{//Add a waterpool
//            
//            int x;
//            int y;
//            do{
//                x = rand() % 61;
//                y = rand() % 61;
//            }while(earthAt(x, y));
//            
//            Actor* a = new Waterpool(this, x, y);
//            addActor(a);
//        }
//    }
    
    if(m_player->getLiveStatus())
        return GWSTATUS_CONTINUE_GAME;
    else{
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
}

void StudentWorld::cleanUp(){
    vector<Actor*>::iterator it;
    
    it = m_gameObjects.begin();
    while(it != m_gameObjects.end()){
        delete *it;
        it = m_gameObjects.erase(it);
    }
    
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            delete m_earthTracker[i][j];
        }
    }
    delete m_player;
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

bool StudentWorld::boulderAt(int x, int y) const{
    
    for(int k = 0; k < m_gameObjects.size(); k++){
        if(m_gameObjects[k]->getGameID() == 'B'){
            for(int i = 0; i < actorSize; i++){
                for(int j = 0; j < actorSize; j++){
                    if(inField(x-j, y-i)){
                        if(m_gameObjects[k]->getX() == x-j && m_gameObjects[k]->getY() == y-i){
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;

}

void StudentWorld::illuminateOilField(int x, int y, int radius){
    vector<Actor*>::iterator it;
    for(it = m_gameObjects.begin(); it != m_gameObjects.end(); it++){
        if(distanceApart(x, y, (*it)->getX(), (*it)->getY()) < radius)
            (*it)->setVisible(true);
    }
}


vector<Actor*> StudentWorld::findActorsWithinRadius(int x, int y, int radius, char c){
    vector<Actor*> output;
    for(int i = 0; i != m_gameObjects.size(); i++){
        Actor* a = m_gameObjects[i];
        if(a->getGameID() == c && a->getLiveStatus()){
            if(distanceApart(x, y, a->getX(), a->getY()) <= radius)
                output.push_back(a);
        }
    }
    return output;
}

vector<Actor*> StudentWorld::findProtestorsWithinRadius(int x, int y, int radius){
    vector<Actor*> output;
    for(int i = 0; i != m_gameObjects.size(); i++){
        Actor* a = m_gameObjects[i];
        if((a->getGameID() == 'p' || a->getGameID() == 'P') && a->getLiveStatus()){
            if(distanceApart(x, y, a->getX(), a->getY()) <= radius)
                output.push_back(a);
        }
    }
    return output;
}

bool StudentWorld::tunnelManWithinRadius(int x, int y, int radius  ){
    if(distanceApart(x, y, m_player->getX(), m_player->getY()) <= radius)
        return true;
    else
        return false;
}

bool StudentWorld::killProtestorsWithinRadius(int x, int y, int radius){
    bool output = false;
    vector<Actor*> doomed = findProtestorsWithinRadius(x, y, radius);
    if(doomed.size() != 0){
        output = true;
        for(int i = 0; i < doomed.size(); i++){
            doomed[i]->annoy(100); //AllocatePoints in the protestor::annoy function
        }
    }
    return output;
}

bool StudentWorld::squirtProtestorWithinRadius(int x, int y, int radius){
    for(int i = 0; i != m_gameObjects.size(); i++){
        Actor* a = m_gameObjects[i];
        vector<Actor*> doomed = findProtestorsWithinRadius(x, y, radius);
        if(doomed.size() != 0){
//            doomed[0]->annoy(squirtDamage); //Allocate points in the protestor::annoy function
            doomed[0]->annoy(100); 
            return true;
        }
    }
    return false;
}

bool StudentWorld::willHitBoulderOrEdge(int x, int y, GraphObject::Direction d){
    
    if(d == GraphObject::up){
        if(!actorWouldBeWithinField(x, y+1))
            return true;
        
        if(boulderAt(x, y+1))
            return true;
    }
    else if(d == GraphObject::down){
        if(!actorWouldBeWithinField(x, y-1))
            return true;
        
        if(boulderAt(x, y-1))
            return true;
    }
    else if(d == GraphObject::left){
        if(!actorWouldBeWithinField(x-1, y))
            return true;
        
        if(boulderAt(x-1, y))
            return true;
    }
    else if(d == GraphObject::right){
        if(!actorWouldBeWithinField(x+1, y))
            return true;
        
        if(boulderAt(x+1, y))
            return true;
    }

    return false;
}

bool StudentWorld::willHitBoulderEdgeOrEarth(int x, int y, GraphObject::Direction d){
    if(willHitBoulderOrEdge(x, y, d))
        return true;
    
    if(d == GraphObject::up){
        if(earthAt(x, y+1))
            return true;
    }
    else if(d == GraphObject::down){
        if(earthAt(x, y-1))
            return true;
    }
    else if(d == GraphObject::left){
        if(earthAt(x-1, y))
            return true;
    }
    else if(d == GraphObject::right){
        if(earthAt(x+1, y))
            return true;
    }
    
    return false;
}

GraphObject::Direction StudentWorld::getDirectionForProtestorExit(Actor* p){
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            m_maze[i][j] = 0;
        }
    }
    bool visited[VIEW_HEIGHT][VIEW_WIDTH];
    for(int i = 0; i < VIEW_HEIGHT; i++){
        for(int j = 0; j < VIEW_WIDTH; j++){
            visited[i][j] = false;
        }
    }
    queue<mazeLocation> q;
    q.push(mazeLocation(60, 60));
    
    while(!q.empty()){
        mazeLocation current = q.front();
        q.pop();
        int x = current.x;
        int y = current.y;
        
        if(!willHitBoulderEdgeOrEarth(x, y, GraphObject::up) && !visited[y+1][x]){
            visited[y+1][x] = true;
            q.push(mazeLocation(x, y+1));
            m_maze[y+1][x] = m_maze[y][x] + 1;
        }
        if(!willHitBoulderEdgeOrEarth(x, y, GraphObject::down) && !visited[y-1][x]){
            visited[y-1][x] = true;
            q.push(mazeLocation(x, y-1));
            m_maze[y-1][x] = m_maze[y][x] + 1;
        }
        if(!willHitBoulderEdgeOrEarth(x, y, GraphObject::right) && !visited[y][x+1]){
            visited[y][x+1] = true;
            q.push(mazeLocation(x+1, y));
            m_maze[y][x+1] = m_maze[y][x] + 1;
        }
        if(!willHitBoulderEdgeOrEarth(x, y, GraphObject::left) && !visited[y][x-1]){
            visited[y][x-1] = true;
            q.push(mazeLocation(x-1, y));
            m_maze[y][x-1] = m_maze[y][x] + 1;
        }
    }
    
    int protestorX = p->getX();
    int protestorY = p->getY();
    
    const int notValid = 100;
    int up = notValid;
    int down = notValid;
    int left = notValid;
    int right = notValid;
    
    if(inField(protestorX, protestorY+1) && visited[protestorY+1][protestorX]){
        up = m_maze[protestorY+1][protestorX];
    }
    if(inField(protestorX, protestorY-1) && visited[protestorY-1][protestorX]){
        down = m_maze[protestorY-1][protestorX];
    }
    if(inField(protestorX+1, protestorY) && visited[protestorY][protestorX+1]){
        right = m_maze[protestorY][protestorX+1];
    }
    if(inField(protestorX-1, protestorY) && visited[protestorY][protestorX-1]){
        left = m_maze[protestorY][protestorX-1];
    }
    
    int lowestDir = up;
    if(down < lowestDir)
        lowestDir = down;
    if(left < lowestDir)
        lowestDir = left;
    if(right < lowestDir)
        lowestDir = right;
    
    assert(lowestDir != notValid);
    
    if(lowestDir == up)
        return GraphObject::up;
    else if(lowestDir == down)
        return GraphObject::down;
    else if(lowestDir == right)
        return GraphObject::right;
    else
        return GraphObject::left;
    
}

//Private Function Implementations
               
double StudentWorld::distanceApart(int x, int y, int x2, int y2){
    int diffX = x2 - x;
    int diffY = y2 - y;
    int total = diffX * diffX + diffY * diffY;
    return sqrt(total);
}

void StudentWorld::addActor(Actor* a){
    m_gameObjects.push_back(a);
}




void StudentWorld::removeActor(Actor* a){
    vector<Actor*>::iterator it;
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


TunnelMan* StudentWorld::getTunnelMan() const{
    return m_player;
}



bool StudentWorld::inField(int x, int y) const{
    if(x < 0 || x >= VIEW_WIDTH)
        return false;
    if(y < 0 || y >= VIEW_HEIGHT)
        return false;
    return true;
}

bool StudentWorld::actorWouldBeWithinField(int x, int y)const{
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

//void StudentWorld::populateFieldWithNuggets(){
//    int numNuggets;
//    int alt = 5 - getLevel() / 2;
//    if(alt > 2)
//        numNuggets = alt;
//    else
//        numNuggets = 2;
//
//    for(int i = 0; i < numNuggets; i++){
//        int x, y;
//        do{
//            x = rand() % 60;
//            y = rand() % 56;
//        }while(thereAreObjectsTooClose(x, y));
//
//        Gold* g = new Gold(this, x, y, true, true, -1);
//        addActor(g);
//    }
//}
//
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
    vector<Actor*>::iterator it;
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
    int squirts = m_player->getNumSquirts();
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
 

