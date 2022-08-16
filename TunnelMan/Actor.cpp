#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <queue>

//Actor class method implementations:

Actor::Actor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, bool annoyable) : GraphObject(imageID, startX, startY, dir, size, depth){
    
    m_world = myWorld;
    setVisible(false); //All actors start out as not visible
    setLiveStatus(true); //All actors start out as alive
    m_annoyable = annoyable;
}

void Actor::changeAnnoyableStatus(){
    if(m_annoyable)
        m_annoyable = false;
    else
        m_annoyable = true;
}
bool Actor::isAnnoyable(){
    return m_annoyable;
}

void Actor::setLiveStatus(bool b){
    m_isAlive = b;
}
bool Actor::getLiveStatus() const{
    return m_isAlive;
}

StudentWorld* Actor::getWorld() const{
    return m_world;
}

void Actor::move(){
    if(getDirection() == up && getY() < VIEW_HEIGHT-actorSize)
        moveTo(getX(), getY()+1);
    else if(getDirection() == down && getY() > 0)
        moveTo(getX(), getY() - 1);
    else if(getDirection() == right && getX() < VIEW_WIDTH-actorSize)
        moveTo(getX()+1, getY());
    else if(getDirection() == left && getX() > 0)
        moveTo(getX() - 1, getY());
}

//Earth class method implementations:


Earth::Earth(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_EARTH, startX, startY, right, 0.25, 3, false){
    
    setVisible(true); //Earths are set to be visible when they are created
}

char Earth::getGameID() const{
    return 'E'; //Earths have a character ID of 'E'
}



//TunnelMan class method implementations

TunnelMan::TunnelMan(StudentWorld* myWorld) : Actor(myWorld, TID_PLAYER, 30, 60){
    m_numWater = 5;
    m_numSonarCharges = 1;
    m_numNuggets = 0;
    m_numBarrelsFound = 0;
    m_hitPoints = 10;
    
    setVisible(true); //TunnelMan starts out as visible
}

char TunnelMan::getGameID() const{
    return 'T';
}

void TunnelMan::doSomething(){
    if(!getLiveStatus()){
        return;
    }
    int x = getX();
    int y = getY();
    StudentWorld* myWorld = getWorld();
    
    bool earthNeedsToBeRemoved = false;
    if(myWorld->earthAt(x, y)){
        myWorld->removeEarth(x, y);
        myWorld->playSound(SOUND_DIG);
        earthNeedsToBeRemoved = true;
    }
    if(earthNeedsToBeRemoved)
        return;
    
    
    int ch;
    if(myWorld->getKey(ch) == true){
        if(ch == KEY_PRESS_ESCAPE){
            annoy(100);
        }
        else if(ch == KEY_PRESS_SPACE){
            //If they have suffient water in their squirt gun
            //the TunnelMan will fire a Squirt into the oil field
            //The TunnelMan will then reduce their water count by 1.
            if(m_numWater > 0){
                introduceSquirt();
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                m_numWater--;
            }

        }
        else if(ch == 'Z' || ch == 'z'){
            if(m_numSonarCharges > 0){
                m_numSonarCharges--;
                getWorld()->illuminateOilField(getX(), getY(), 12);
                getWorld()->playSound(SOUND_SONAR);
            }
        }
        else if(ch == KEY_PRESS_TAB){
            if(m_numNuggets > 0){
                Gold* g= new Gold(getWorld(), getX(), getY(), false, false, 100);
                getWorld()->addActor(g);
                m_numNuggets--;
            }

        }
        //ch is one of the arrow keys
        else{
            //*the TunnelMan cannot occupy a square that is less than or equal to a radius of 3 away from the center of any Boulder.
            if(ch == KEY_PRESS_RIGHT){
                if(getDirection() == right){
                    if(!myWorld->actorWithinRadius(x+1, y, 3, 'B'))
                       move();
                }
                else
                    setDirection(right);
            }
            if(ch == KEY_PRESS_LEFT){
                if(getDirection() == left){
                    if(!myWorld->actorWithinRadius(x-1, y, 3, 'B'))
                        move();
                }
                else
                    setDirection(left);
            }
            if(ch == KEY_PRESS_UP){
                if(getDirection() == up){
                    if(!myWorld->actorWithinRadius(x, y+1, 3, 'B'))
                        move();
                }
                else
                    setDirection(up);
            }
            if(ch == KEY_PRESS_DOWN){
                if(getDirection() == down){
                    if(!myWorld->actorWithinRadius(x, y-1, 3, 'B'))
                        move();
                }
                else
                    setDirection(down);
            }
        }
    }
}

void TunnelMan::incrementNumNuggets(){
    m_numNuggets++;
    getWorld()->increaseScore(10);
}

int TunnelMan::getNumNuggets() const{
    return m_numNuggets;
}

void TunnelMan::incrementNumSonarCharges(){
    m_numSonarCharges ++;
    getWorld()->increaseScore(75);
}
int TunnelMan::getNumSonarCharges() const{
    return m_numSonarCharges;
}

void TunnelMan::incrementNumWater(){
    m_numWater += 5;
    getWorld()->increaseScore(100);
}
int TunnelMan::getNumWater() const{
    return m_numWater;
}

void TunnelMan::incrementBarrelsFound(){
    m_numBarrelsFound++;
    getWorld()->increaseScore(1000);
}
int TunnelMan::getNumBarrelsFound(){
    return m_numBarrelsFound;
}
void TunnelMan::annoy(int howMuch){
    m_hitPoints -= howMuch;
    if(m_hitPoints < 0){
        setLiveStatus(false);
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
}
void TunnelMan::setHitPoints(int points){
    m_hitPoints = points;
}

int TunnelMan::getHitPoints() const{
    return m_hitPoints;
}


void TunnelMan::introduceSquirt(){
    int startX = getX();
    int startY = getY();
    if(getDirection() == up){
        startY += 4;
    }
    if(getDirection() == right){
        startX += 4;
    }
    if(startX <= 60 && startY <= 60 && !getWorld()->earthAt(startX, startY) && !getWorld()->actorWithinRadius(startX, startY, 3, 'B')){
        Squirt* s = new Squirt(getWorld(), this, startX, startY);
        getWorld()->addActor(s);
    }
}

//Boulder class method implementations:
Boulder::Boulder(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_BOULDER, startX, startY, down, 1, 1.0, false){
    setVisible(true); //Boulders start out visible
    
    getWorld()->removeEarth(startX, startY);
    state = 0; //Boulders start out in a stable state (0 = stable, 1 = waiting, 2 = falling)
    ticksElapsed = -1; //To track how many ticks have elapsed after entering waiting state. ticksElapsed = -1 when the boulder is not in a waiting state
}


char Boulder::getGameID() const{
    return 'B';
}

void Boulder::doSomething(){
    if(!getLiveStatus())
        return;

    if(state == 0){ //Stable state
        if(boulderCanFall()){
            state = 1; //Waiting state
            ticksElapsed++;
        }
    }

    if(state == 1){ //Waiting state
        if(ticksElapsed >= ticksBeforeFalling){ //check if this should be > or >=
            state = 2; //Falling state
            getWorld()->playSound(SOUND_FALLING_ROCK);
            ticksElapsed = -1;
        }
        else{
            ticksElapsed++;
        }
    }
    if(state == 2){//Falling state
        if(boulderCanFall()){
            getWorld()->playSound(SOUND_FALLING_ROCK);
            smushCharacters();
            moveTo(getX(), getY() - 1);
        }
        else{
            setLiveStatus(false); //Set the boulder to dead
        }


    }
}

void Boulder::smushCharacters(){
    if(getWorld()->annoyProtestorsWithinRadius(getX(), getY(), 4, 100)){
        getWorld()->increaseScore(500);
    }
        
    if(getWorld()->tunnelManWithinRadius(getX(), getY(), 4)){
        getWorld()->getTunnelMan()->annoy(100);
    }
}
bool Boulder::boulderCanFall() const{
    int x = getX();
    int y = getY();
    
    if(y == 0)
        return false; //Hits the bottom of the screen so boulder cannot fall any longer
    if(getWorld()->earthAt(x, y - 1) || getWorld()->findActor(x, y - 1, 'B'))
            return false; //Hits eath or another bolder so cannot fall any longer
    return true;
}

//Barrel Class Function Implementations:

Barrel::Barrel(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_BARREL, startX, startY, right, 1.0, 2, false){
    
    setVisible(false); //Barrels start out as invisible
}

char Barrel::getGameID() const{
    return 'O'; //Barrels have a character/game ID of 'O' (Oil)
}

void Barrel::doSomething(){
    if(!getLiveStatus())
        return;
    
    if(!isVisible() && getWorld()->tunnelManWithinRadius(getX(), getY(), 4)){
        setVisible(true);
        return;
    }
    
    if(getWorld()->tunnelManWithinRadius(getX(), getY(), 3)){
        setLiveStatus(false);
        getWorld()->playSound(SOUND_FOUND_OIL);
        
        //Tell the TunnelMan it has picked up a barrel; the tunnelman class will increase the score
        getWorld()->getTunnelMan()->incrementBarrelsFound();
    }
}

//Protestor Class function implementations:

Protestor::Protestor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(myWorld, imageID, startX, startY, dir, size, depth){}


//RegularProtestor class function implementations:

RegularProtestor::RegularProtestor(StudentWorld* myWorld) : Protestor(myWorld, TID_PROTESTER, 60, 60, left, 1.0, 0){
    
    setVisible(true); //Regular protestors start out as visible
    
    m_numSquaresToMoveInCurrentDirection = generateNumSquaresToMove();
    m_hitPoints = 5;
    m_leaveTheOilField = false;
    
    m_tickCount = 0; //Protestors start out ready to do something on their first tick
    m_NonRestingTicksSinceShout = 0;
    m_NonRestingTicksSinceTurn = 0;
}

char RegularProtestor::getGameID() const{
    return 'p';
}
int RegularProtestor::generateNumSquaresToMove(){
    return rand() % 53 + 8;
}

bool RegularProtestor::wantsToLeave(){
    return m_leaveTheOilField;
}

void RegularProtestor::annoy(int howMuch){
    if(m_leaveTheOilField)
        return;
    
    m_hitPoints -= howMuch;
    if(m_hitPoints > 0){
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        int stunnedTicks = fmax(50, 100 - getWorld()->getLevel() * 10);
        m_tickCount = stunnedTicks; //make the protestor stunned
    }
    else{
        //If he died because of a squirt
        if(howMuch == 2)
            getWorld()->increaseScore(100);
        //Points allocated for dying by means of a boulder occur in the boulder class
        
        changeAnnoyableStatus();
        m_leaveTheOilField = true;
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        m_tickCount = 0;
    } //Make sure points are given in the other classes
}

void RegularProtestor::doSomething(){
    if(!getLiveStatus())
        return;
    
    if(m_tickCount > 0){
        m_tickCount--;
        return;
    }
    
    //int ticksToWait = fmax(0, 3 - getWorld()->getLevel() / 4);
    int ticksToWait = 8;
    m_tickCount = ticksToWait;
    //At this point the protestor passed these checks and is allowed to do something:
    
    if(m_leaveTheOilField){
//        if(getX() == 60 || getY() == 60)
//            setLiveStatus(false);
//        else{
//            changeDirectionInOrderToExit();
//            move();
//            m_NonRestingTicksSinceShout = -1;
//            m_NonRestingTicksSinceTurn = -1;
//        }
        setLiveStatus(false);
        return;
    }
    
    if(withinShoutingDistance()){
        if(m_NonRestingTicksSinceShout >= 15){
            getWorld()->getTunnelMan()->annoy(2);
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            m_NonRestingTicksSinceShout = 0;
        }
        else{
            m_NonRestingTicksSinceShout++;
        }
        m_NonRestingTicksSinceTurn++;
        return;
    }
    
    if(canMoveTowardTunnelMan()){
        Direction d = getDirection();
        
        faceTunnelMan();
        
        if(justTurned90(d, getDirection()))
            m_NonRestingTicksSinceTurn = -1;
        
        
        
        m_numSquaresToMoveInCurrentDirection = 0;
        move();
        
        m_NonRestingTicksSinceShout++;
        m_NonRestingTicksSinceTurn++;
        return;
    }
    
    //At this point the protestor is not near and does not see the TunnelMan
    
    m_numSquaresToMoveInCurrentDirection--;
    if(m_numSquaresToMoveInCurrentDirection <= 0){
        Direction d;
        Direction d2 = getDirection();
        do{
            d = generateRandomDirection();
            setDirection(d);
        }while(!isViableDirection(d));
        
        if(justTurned90(d2, d))
            m_NonRestingTicksSinceTurn = -1;
        
        m_numSquaresToMoveInCurrentDirection = generateNumSquaresToMove();
    }
        
    
    if(m_NonRestingTicksSinceTurn >= 200 && atIntersection()){
        m_numSquaresToMoveInCurrentDirection = generateNumSquaresToMove();
        
        m_NonRestingTicksSinceTurn = -1;
    }
    
    if(isViableDirection(getDirection()))
        move();
    else
        m_numSquaresToMoveInCurrentDirection = 0;
    
    m_NonRestingTicksSinceShout++;
    m_NonRestingTicksSinceTurn++;
    return;
}

bool RegularProtestor::justTurned90(Direction d1, Direction d2){
    if(d1 == up || d1 == down){
        if(d2 == right || d2 == left)
            return true;
        else
            return false;
    }
    else{
        if(d2 == up || d2 == down)
            return true;
        else
            return false;
    }
}


bool RegularProtestor::atIntersection(){
    Direction d = getDirection();
    if(d == up || d == down){
        if(!isViableDirection(left) && !isViableDirection(right))
            return false;
        else{
            if(isViableDirection(left) && isViableDirection(right)){
                int i = rand() % 2;
                if(i == 0)
                    setDirection(left);
                else
                    setDirection(right);
            }
            else if(isViableDirection(left)){
                setDirection(left);
            }
            else if(isViableDirection(right))
                setDirection(right);
            return true;
        }
    }
    else{
        if(!isViableDirection(up) && !isViableDirection(down)){
            return false;
        }
        else{
            if(isViableDirection(up) && isViableDirection(down)){
                int i = rand() % 2;
                if(i == 0)
                    setDirection(up);
                else
                    setDirection(down);
            }
            else if(isViableDirection(up))
                setDirection(up);
            else if(isViableDirection(down))
                setDirection(down);
            return true;
        }
    }
}



bool RegularProtestor::isViableDirection(Direction d){
    int x = getX();
    int y = getY();
    if(d == up){
        if(!getWorld()->actorInField(x, y + 1))
            return false;
        if(getWorld()->earthAt(x, y+1))
            return false;
        if(getWorld()->actorAt(x, y+1, 'B'))
            return false;
    }
    else if(d == down){
        if(!getWorld()->actorInField(x, y - 1))
            return false;
        if(getWorld()->earthAt(x, y-1))
            return false;
        if(getWorld()->actorAt(x, y-1, 'B'))
            return false;
    }
    else if(d == right){
        if(!getWorld()->actorInField(x+1, y))
            return false;
        if(getWorld()->earthAt(x+1, y))
            return false;
        if(getWorld()->actorAt(x+1, y, 'B'))
            return false;
    }
    else if(d == left){
        if(!getWorld()->actorInField(x-1, y))
            return false;
        if(getWorld()->earthAt(x-1, y))
            return false;
        if(getWorld()->actorAt(x-1, y, 'B'))
            return false;
    }
    return true;
}

GraphObject::Direction RegularProtestor::generateRandomDirection(){
    int i = rand() % 4;
    if(i == 0)
        return up;
    if(i == 1)
        return down;
    if(i == 2)
        return left;
    else
        return right;
        
}
void RegularProtestor::faceTunnelMan(){
    int x = getX();
    int y = getY();
    
    int tX = getWorld()->getPlayerX();
    int tY = getWorld()->getPlayerY();
    
    assert(tX == x || tY == y);
    
    if(tX > x){
        setDirection(right);
    }
    if(tX < x){
        setDirection(left);
    }
    if(tY < y){
        setDirection(down);
    }
    if(tY > y){
        setDirection(up);
    }
}

//void RegularProtestor::changeDirectionInOrderToExit(){
//    char maze[64][64];
//    getWorld()->getMazeForExit(maze);
//
//    if(l)
//
//
//}

bool RegularProtestor::withinShoutingDistance(){
    
    int x = getX();
    int y = getY();

    if(!getWorld()->tunnelManWithinRadius(x, y, 4))
        return false;
    
    int tX = getWorld()->getPlayerX();
    int tY = getWorld()->getPlayerY();
    
    if(getDirection() == up && tY > y)
        return true;
    if(getDirection() == down && tY < y)
        return true;
    if(getDirection() == left && tX < x)
        return true;
    if(getDirection() == right && tX > x)
        return true;
    
    return false;
}

bool RegularProtestor::canMoveTowardTunnelMan(){
    int x = getX();
    int y = getY();
    int tX = getWorld()->getPlayerX();
    int tY = getWorld()->getPlayerY();
    
    if(x != tX && y != tY)
        return false;
    
    if(x == tX){
        if(y < tY){
            for(int i = 0; i < tY - y; i++){
                if(getWorld()->earthAt(x, y+i) || getWorld()->actorAt(x, y+i, 'B'))
                    return false;
            }
        }
        else{
            for(int i = 0; i < y - tY - 1; i++){ //To account for if the tunnelman is digging
                if(getWorld()->earthAt(x, y-i) || getWorld()->actorAt(x, y-i, 'B'))
                    return false;
            }
        }
    }
    if(y == tY){
        if(x < tX){
            for(int i = 0; i < tX - x; i++){
                if(getWorld()->earthAt(x+i, y) || getWorld()->actorAt(x+i, y, 'B'))
                    return false;
            }
        }
        else{
            for(int i = 0; i < x - tX - 1; i++){ //to account for if the tunnelman is digging
                if(getWorld()->earthAt(x-i, y) || getWorld()->actorAt(x-i, y, 'B'))
                    return false;
            }
        }
    }
    return true;
}


//Squirt class function implementations
Squirt::Squirt(StudentWorld* myWorld, TunnelMan* owner, int startX, int startY) : Actor(myWorld, TID_WATER_SPURT, startX, startY, owner->getDirection(), 1, 1.0){
    m_travelDistance = squirtInitialTravelDistance;
    setVisible(true); //All squirt objects start off as visible
}

void Squirt::doSomething(){
    if(getWorld()->annoyProtestorsWithinRadius(getX(), getY(), 3, 2)){
        setLiveStatus(false);
        return;
    }
    if(m_travelDistance == 0){
        setLiveStatus(false);
        return;
    }

    if(!canMove(getDirection())){
        setLiveStatus(false);
        return;
    }

    Direction d = getDirection();
    int x = getX();
    int y = getY();
    if(d == up)
        moveTo(x, y+1);
    if(d == down)
        moveTo(x, y-1);
    if(d == right)
        moveTo(x+1, y);
    if(d == left)
        moveTo(x-1, y);
    m_travelDistance--;
}
bool Squirt::canMove(Direction d) const{
    int x = getX();
    int y = getY();

    if(d == right){
        if(x >= VIEW_WIDTH - actorSize)
            return false;
        if(boulderAt(x + 1, getY()))
            return false;
        
        if(getWorld()->earthAt(x+1, y))
            return false;
        
        return true;
    }
    if(d == left){
        if(x <= 0)
            return false;
        if(boulderAt(x - 1, y))
            return false;
        
        if(getWorld()->earthAt(x-1, y))
            return false;
        
        return true;
    }
    if(d == up){
        if(y >= VIEW_HEIGHT - actorSize)
            return false;
        if(boulderAt(x, y + 1))
            return false;
        
        if(getWorld()->earthAt(x, y+1))
            return false;
        
        return true;
    }
    if(d == down){
        if(getY() <= 0)
            return false;
        if(boulderAt(getX(), getY() - 1))
            return false;

        if(getWorld()->earthAt(x, y-1))
            return false;
        
        return true;
    }

    return false; //This will never be hit
}

bool Squirt::boulderAt(int x, int y) const{
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(getWorld()->inField(x + j, y + i)){
                if(getWorld()->findActor(x + j, y + i, 'B') != nullptr){
                    return true;
                }
            }
        }
    }

    return false;
}

char Squirt::getGameID() const{
    return 'S';
}

//Waterpool Class
Waterpool::Waterpool(StudentWorld* myWorld, int startX, int startY) : Goodie(myWorld, TID_WATER_POOL, startX, startY){
    
    
    m_tickCount = fmax(100, 300 - 10*getWorld()->getLevel());
    setVisible(true);
    
}

char Waterpool::getGameID() const{
    return 'W';
}

void Waterpool::doSomething(){
    if(!getLiveStatus())
        return;
    
    if(m_tickCount <= 0){
        setLiveStatus(false);
        return;
    }
    
    if(getWorld()->tunnelManWithinRadius(getX(), getY(), 3)){
        setLiveStatus(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getTunnelMan()->incrementNumWater();
    }
    
    m_tickCount--;
}

//SonarKit Class Function Implementations
SonarKit::SonarKit(StudentWorld* myWorld, int startX, int startY) : Goodie(myWorld, TID_SONAR, startX, startY){

    m_tickCount = fmax(100, 300 - getWorld()->getLevel()*10);
    setVisible(true);
}

void SonarKit::doSomething(){
    
    if(!getLiveStatus())
        return;
    
    if(m_tickCount <= 0){
        setLiveStatus(false);
        return;
    }
    
    if(getWorld()->tunnelManWithinRadius(getX(), getY(), 3)){
        setLiveStatus(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getTunnelMan()->incrementNumSonarCharges();
    }
    
    m_tickCount--;
}

char SonarKit::getGameID() const{
    return 'K';
}

 //Goodie Class Function Implementations:

Goodie::Goodie(StudentWorld* myWorld, int imageID, int startX, int startY) : Actor(myWorld, imageID, startX, startY, right, 1.0, 2){

}


//Gold Class Function Implementations

Gold::Gold(StudentWorld* myWorld, int startX, int startY, bool tunnelManCanPickUp, bool permanent, int ticksToWait) : Goodie(myWorld, TID_GOLD, startX, startY){
    
    m_tunnelManCanPickUp = tunnelManCanPickUp;
    m_permanent = permanent;
    m_tickCount = ticksToWait;
    
    if(!m_tunnelManCanPickUp)
        setVisible(true);
    else
        setVisible(false);
}

char Gold::getGameID() const{
    return 'G';
}

void Gold::doSomething(){
    if(!getLiveStatus())
        return;
    
    if(!m_permanent){
        if(m_tickCount <= 0){
            setLiveStatus(false);
            return;
        }
        else
            m_tickCount--;
    }

    if(m_tunnelManCanPickUp){
        if(!isVisible() && getWorld()->tunnelManWithinRadius(getX(), getY(), 4)){
            setVisible(true);
            return;
        }
        
        if(getWorld()->tunnelManWithinRadius(getX(), getY(), 3)){
            setLiveStatus(false);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->getTunnelMan()->incrementNumNuggets(); //This increases the score by ten
            return;
        }
        
        
    }
    else{
        Actor* p = getWorld()->findActorWithinRadius(getX(), getY(), 3, 'P');
        p = getWorld()->findActorWithinRadius(getX(), getY(), 3, 'p');
        if(p != nullptr){
            //TODO: bribe the protestor
            getWorld()->increaseScore(25);
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            p->setLiveStatus(false);
            

        }
    }
}




//Waterpool class function implementations:

//Protestor class function implementations:
//Protestor::Protestor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(myWorld, imageID, startX, startY, dir, size, depth){
//    m_hitPoints = 5;
//}
//
//void Protestor::setHitPoints(int num){
//    m_hitPoints = num;
//}
//
//int Protestor::getHitPoints() const{
//    return m_hitPoints;
//}


//HardcoreProtestor class function implementationss

