#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

//Actor class method implementations:

Actor::Actor(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth){
    
    m_world = myWorld;
    setVisible(false); //All actors start out as not visible
    setLiveStatus(true); //All actors start out as alive
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


Earth::Earth(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_EARTH, startX, startY, right, 0.25, 3){
    
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
//            if(m_numWater > 0){
//                introduceSquirt();
//                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
//                m_numWater--;
//            }

        }
        else if(ch == 'Z' || ch == 'z'){
            //If they have sufficient sonar charges
            //Decrement numSonarCharges
            //Illuminate the contents of the oil field within a radius of 12 of his location using setVisible()
//            if(m_numSonarCharges > 0){
//                m_numSonarCharges--;
//                getWorld()->illuminateOilField(getX(), getY(), 12);
//            }
        }
        else if(ch == KEY_PRESS_TAB){
            //If the player presses tab and they have 1+ units of gold
            //add a gold nugget object into the oil field at their current x, y w/lifetime of 100 ticks (will start out in a visible state)
            //decrement numGoldNuggets
//            if(m_numNuggets > 0){
//                Gold* g = new Gold(getWorld(), getX(), getY(), false, 100);
//                g->setVisible(true);
//                getWorld()->addActor(g);
//                m_numNuggets--;
//            }

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
}

int TunnelMan::getNumNuggets() const{
    return m_numNuggets;
}

void TunnelMan::incrementNumSonarCharges(){
    m_numSonarCharges++;
}
int TunnelMan::getNumSonarCharges() const{
    return m_numSonarCharges;
}

void TunnelMan::incrementNumWater(){
    m_numWater++;
}
int TunnelMan::getNumWater() const{
    return m_numWater;
}

void TunnelMan::incrementBarrelsFound(){
    m_numBarrelsFound++;
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


//void TunnelMan::introduceSquirt(){
//    int startX = getX();
//    int startY = getY();
//    if(getDirection() == up){
//        startY += 4;
//    }
//    if(getDirection() == right){
//        startX += 4;
//    }
//    if(startX <= 60 && startY <= 60 && !getWorld()->isEarthAt4(startX, startY) && !getWorld()->actorWithinRadius(startX, startY, 3, 'B')){
//        Squirt* s = new Squirt(getWorld(), this, startX, startY);
//        getWorld()->addActor(s);
//    }
//}
//Boulder class method implementations:
Boulder::Boulder(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_BOULDER, startX, startY, down, 1, 1.0){
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

void Boulder:: smushCharacters(){
    int x = getX() - 1;
    int y = getY() - 1;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 6; j++){
            if(getWorld()->isTunnelManAt(x+j, y+i) && getWorld()->getTunnelMan()->getLiveStatus()){
                getWorld()->getTunnelMan()->annoy(100);
            }
            //TODO: Smush nearby protestors:
            //Actor* doomed = getWorld()->findActor(x, y, 'P');
//            if(doomed != nullptr && doomed->getLiveStatus()){
//                doomed->annoy(100);
//            }
            //Actor* doomed = getWorld()->findActor(x, y, 'p');
//            if(doomed != nullptr && doomed->getLiveStatus()){
//                doomed->annoy(100);
//            }
        }
    }
}
bool Boulder::boulderCanFall() const{
    int x = getX();
    int y = getY();
    if(y == 0)
        return false; //Hits the bottom of the screen so boulder cannot fall any longer
    
    
    if(getWorld()->earthAt(x, y - 1) || getWorld()->findActor(x, y - 1, 'B')){
            return false;
    }

    return true;
}


//Squirt class function implementations
//Squirt::Squirt(StudentWorld* myWorld, TunnelMan* owner, int startX, int startY) : Actor(myWorld, TID_WATER_SPURT, startX, startY, owner->getDirection(), 1, 1.0){
//    m_travelDistance = squirtInitialTravelDistance;
//    setVisible(true); //All squirt objects start off as visible
//}
//
//void Squirt::doSomething(){
//    if(annoyNearbyProtestors()){
//        setLiveStatus(false);
//        return;
//    }
//    if(m_travelDistance == 0){
//        setLiveStatus(false);
//        return;
//    }
//
//    if(!canMove(getDirection())){
//        setLiveStatus(false);
//        return;
//    }
//
//    Direction d = getDirection();
//    int x = getX();
//    int y = getY();
//    if(d == up)
//        moveTo(x, y+1);
//    if(d == down)
//        moveTo(x, y-1);
//    if(d == right)
//        moveTo(x+1, y);
//    if(d == left)
//        moveTo(x-1, y);
//}
//bool Squirt::canMove(Direction d) const{
//    int x = getX();
//    int y = getY();
//
//    if(d == right){
//        if(x >= VIEW_WIDTH - actorSize)
//            return false;
//        if(boulderAt(x + 1, getY()))
//            return false;
//        for(int i = 0; i < actorSize; i++){
//            if(getWorld()->isEarthAt(x+1, y+i))
//                return false;
//        }
//        return true;
//    }
//    if(d == left){
//        if(x <= 0)
//            return false;
//        if(boulderAt(x - 1, y))
//            return false;
//        for(int i = 0; i < actorSize; i++){
//            if(getWorld()->isEarthAt(x-1, y+i))
//                return false;
//        }
//        return true;
//    }
//    if(d == up){
//        if(y >= VIEW_HEIGHT - actorSize)
//            return false;
//        if(boulderAt(x, y + 1))
//            return false;
//        for(int i = 0; i < actorSize; i++){
//            if(getWorld()->isEarthAt(x+i, y+1))
//                return false;
//        }
//        return true;
//    }
//    if(d == down){
//        if(getY() <= 0)
//            return false;
//        if(boulderAt(getX(), getY() - 1))
//            return false;
//        for(int i = 0; i < actorSize; i++){
//            if(getWorld()->isEarthAt(x+i, y-1))
//                return false;
//        }
//        return true;
//    }
//
//    return false; //This will never be hit
//}
//
//bool Squirt::boulderAt(int x, int y) const{
//    for(int i = 0; i < actorSize; i++){
//        for(int j = 0; j < actorSize; j++){
//            if(getWorld()->inField(x + j, y + i)){
//                if(getWorld()->findActor(x + j, y + i, 'B') != nullptr){
//                    return true;
//                }
//            }
//        }
//    }
//
//    return false;
//}
//
//char Squirt::getGameID() const{
//    return 'S';
//}
//
////TODO: Ensure this function works correctly
//bool Squirt::annoyNearbyProtestors(){
//    bool output = false;
//
//    int x = getX() - 1;
//    int y = getY() - 1;
//
//    Actor* prevProtestor = nullptr;
//    for(int i = 0; i < actorSize + 2; i++){
//        for(int j = 0; j < actorSize + 2; j++){
//            if(getWorld()->inField(x+j, y+i)){
//                Actor* protestor = getWorld()->findActor(x+j, y+i, 'P');
//                if(protestor != nullptr && protestor != prevProtestor){
//                    protestor->annoy(2);
//                    prevProtestor = protestor;
//                    output = true;
//                }
//            }
//        }
//    }
//
//    return output;
//}
//
// //Goodie Class Function Implementations:
//Goodie::Goodie(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(myWorld, imageID, startX, startY, dir, size, depth){
//
//}
//
//bool Goodie::tunnelManNearby(int radius) const{
//    int decrement = radius - 2;
//    int startX = getX() - decrement;
//    int startY = getY() - decrement;
//
//    for(int i = 0; i < actorSize + (decrement*2); i++){
//        for(int j = 0; j < actorSize + 2; j++){
//            if(getWorld()->isTunnelManAt(startX+j, startY+i)){
//                return true;
//            }
//        }
//    }
//    return false;
//}
//Barrel Class Function Implementations:

//Barrel::Barrel(StudentWorld* myWorld, int startX, int startY) : Goodie(myWorld, TID_BARREL, startX, startY, right, 1.0, 2){
//    setVisible(false); //Barrels start out as invisible
//}
//
//char Barrel::getGameID() const{
//    return 'O'; //Barrels have a character/game ID of 'O' (Oil)
//}
//
//void Barrel::doSomething(){
//    if(!getLiveStatus())
//        return;
//    
//    if(tunnelManNearby(3)){
//        setLiveStatus(false);
//        getWorld()->playSound(SOUND_FOUND_OIL);
//        getWorld()->increaseScore(1000);
//        
//        //If necessary, inform StudentWorld object that it has been picked up
//    }
//    
//    if(!isVisible() && tunnelManNearby(4)){
//        setVisible(true);
//        return;
//    }
//    
//}

//Gold Class Function Implementations

//Gold::Gold(StudentWorld* myWorld, int startX, int startY, bool tunnelManCanPickUp, int numTicks) : Goodie(myWorld, TID_GOLD, startX, startY, right, 1.0, 2){
//
//    bool shouldBeVisible = true;
//    for(int i = 0; i < actorSize; i++){
//        for(int j = 0; j < actorSize; j++){
//            if(getWorld()->isEarthAt(startX+j, startY+i)){
//                setVisible(false);
//                shouldBeVisible = false;
//            }
//        }
//    }
//    if(shouldBeVisible)
//        setVisible(true);
//
//    m_tunnelManCanPickUp = tunnelManCanPickUp;
//
//    m_ticksRemaining = numTicks;
//}
//
//char Gold::getGameID() const{
//    return 'G';
//}
//
//void Gold::doSomething(){
//    if(!getLiveStatus())
//        return;
//
//    if(m_tunnelManCanPickUp && tunnelManNearby(3)){
//        setLiveStatus(false);
//        getWorld()->playSound(SOUND_GOT_GOODIE);
//        getWorld()->increaseScore(10);
//        getWorld()->getTunnelMan()->incrementNumNuggets();
//        return;
//    }
//
//    if(!isVisible() && tunnelManNearby(4)){
//        setVisible(true);
//        return;
//    }
//
////    if(!m_tunnelManCanPickUp){
////        Protestor* p = findNearbyProtestor();
////        if(p != nullptr){
////            setLiveStatus(false);
////            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
////            //TODO: tell protestor object it found a gold nugget
////            getWorld()->increaseScore(25);
////        }
////    }
//
//    if(m_ticksRemaining != -1){
//        if(m_ticksRemaining == 0)
//            setLiveStatus(false);
//    }
//}

//Protestor* Gold::findNearbyProtestor(){
//
//}


//SonarKit Class Function Implementations
//SonarKit::SonarKit(StudentWorld* myWorld, int startX, int startY) : Goodie(myWorld, TID_SONAR, startX, startY, right, 1.0, 2){
//
//    int level = getWorld()->getLevel();
//    int alternative = 300 - (10*level);
//    if(alternative > 100)
//        m_ticksRemaining = alternative;
//    else
//        m_ticksRemaining = 100;
//}
//
//void SonarKit::doSomething(){
//    return;
//}
//
//char SonarKit::getGameID() const{
//    return 'K';
//}

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

//RegularProtestor class function implementations:

//RegularProtestor::RegularProtestor(StudentWorld* myWorld) : Actor(myWorld, TID_PROTESTER, 60, 60, left, 1.0, 0){
//    
//    setVisible(true); //Regular protestors start out as visible
//    m_numSquares = rand() % 53 + 8;
//    m_leaveTheOilField = false;
//    m_restingTicks = fmax(0, 3 - getWorld()->getLevel() / 4);
//    m_ticksElapsed = 0;
//    m_ticksSinceLastShout = 15;
//}
//
//char RegularProtestor::getGameID() const{
//    return 'p';
//}
//
////THIS SHOULD NOT HAPPEN EVERY TICK
//void RegularProtestor::doSomething(){
//    if(!getLiveStatus())
//        return;
//    //Make sure the following only happens when it is allowed:
//    if(m_ticksElapsed < m_restingTicks){
//        m_ticksElapsed++;
//        m_ticksSinceLastShout++;
//        return;
//    }
//    m_ticksElapsed = 0;
//    
//    if(m_leaveTheOilField){
//        if(getX() < 60)
//            moveTo(getX() + 1, getY());
//        else if(getY() < 60)
//            moveTo(getX(), getY() + 1);
//        else
//            setLiveStatus(false);
//        m_ticksSinceLastShout++;
//        return;
//    }
//    
//    if(canShoutAtTunnelMan() && m_ticksSinceLastShout >= 15){
//        getWorld()->getTunnelMan()->annoy(2);
//        getWorld()->playSound(SOUND_PROTESTER_YELL);
//        m_ticksSinceLastShout = 0;
//        return;
//    }
//    if(canMoveTowardTunnelMan()){
//        faceTunnelMan();
//        m_numSquares = 0;
//        moveTowardTunnelMan();
//    }
//}
//
//bool RegularProtestor::canMoveTowardTunnelMan(){
//    int x = getX();
//    int y = getY();
//    int tX = getWorld()->getPlayerX();
//    int tY = getWorld()->getPlayerY();
//    if(x == tX){
//        int diff = tY - y;
//        for(int i = 0; i < diff; i++){
//            if(getWorld()->isEarthAt4(x, y+i))
//                return false;
//        }
//    }
//    if(y == tY){
//        int diff = tX - x;
//        for(int i = 0; i < diff; i++){
//            if(getWorld()->isEarthAt4(x+i, y))
//                return false;
//        }
//    }
//    return true;
//}
//
//bool RegularProtestor::canShoutAtTunnelMan(){
//    int x = getX();
//    int y = getY();
//    
//    int tX = getWorld()->getPlayerX();
//    int tY = getWorld()->getPlayerY();
//    
//    if(getDirection() == up)
//        if(tX == x && tY - y < 4)
//            return true;
//    if(getDirection() == down)
//        if(tX == x && y - ty < 4)
//            return true;
//    if(getDirection() == left)
//        if(tY == y && x - Tx < 4)
//            return true;
//    if(getDirection() == right)
//        if(tY == y && Tx - x < 4)
//            return true;
//    
//    return false;
//}
//void RegularProtestor::moveToExit(){
//    if()
//}
//HardcoreProtestor class function implementationss
