#include "Actor.h"
#include "StudentWorld.h"


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


//Earth class method implementations:


Earth::Earth(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_EARTH, startX, startY, right, 0.25, 3){
    
    setVisible(true); //Earths are set to be visible when they are created
}

char Earth::getGameID() const{
    return 'E'; //Earths have a character ID of 'E'
}


//Character class method implementations
Character::Character(StudentWorld* myWorld, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(myWorld, imageID, startX, startY, dir, size, depth){
    m_hitPoints = 10;
}

void Character::annoy(int howMuch){
    m_hitPoints -= howMuch;
    if(m_hitPoints < 0)
        setLiveStatus(false);
}



//TunnelMan class method implementations

TunnelMan::TunnelMan(StudentWorld* myWorld) : Character(myWorld, TID_PLAYER, 30, 60){
    m_numWater = 5;
    m_numSonarCharges = 1;
    m_numNuggets = 0;
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
    bool earthNeedsToBeRemoved = false;
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            if(getWorld()->isEarthAt(x+i, y+j)){
                getWorld()->removeEarth(x+i, y+j);
                getWorld()->playSound(SOUND_DIG);
                earthNeedsToBeRemoved = true;
            }
        }
    }
    if(earthNeedsToBeRemoved)
        return;
    
    int ch;
    if(getWorld()->getKey(ch) == true){
        if(ch == KEY_PRESS_ESCAPE){
            annoy(100);
        }
        else if(ch == KEY_PRESS_SPACE){
            //If they have suffient water in their squirt gun
            //the TunnelMan will fire a Squirt into the oil field
            //The TunnelMan will then reduce their water count by 1.
        }
        else if(ch == 'Z' || ch == 'z'){
            //If they have sufficient sonar charges
            //Decrement numSonarCharges
            //Illuminate the contents of the oil field within a radius of 12 of his location using setVisible()
        }
        else if(ch == KEY_PRESS_TAB){
            //If the player presses tab and they have 1+ units of gold
            //add a gold nugget object into the oil field at their current x, y w/lifetime of 100 ticks (will start out in a visible state)
            //decrement numGoldNuggets
        }
        //ch is one of the arrow keys
        else{
            //*the TunnelMan cannot occupy a square that is less than or equal to a radius of 3 away from the center of any Boulder.
            if(ch == KEY_PRESS_RIGHT){
                if(getDirection() == right){
                    if(canMove(right))
                       moveTo(getX() + 1, getY());
                }
                else
                    setDirection(right);
            }
            if(ch == KEY_PRESS_LEFT){
                if(getDirection() == left){
                    if(canMove(left))
                        moveTo(getX() - 1, getY());
                }
                else
                    setDirection(left);
            }
            if(ch == KEY_PRESS_UP){
                if(getDirection() == up){
                    if(canMove(up))
                        moveTo(getX(), getY() + 1);
                }
                else
                    setDirection(up);
            }
            if(ch == KEY_PRESS_DOWN){
                if(getDirection() == down){
                    if(canMove(down))
                        moveTo(getX(), getY() - 1);
                }
                else
                    setDirection(down);
            }
        }
    }
}


bool TunnelMan::canMove(Direction d) const{
    if(d == right){
        if(getX() >= VIEW_WIDTH - actorSize)
            return false;
        if(boulderAt(getX() + 1, getY()))
            return false;
        else
            return true;
    }
    if(d == left){
        if(getX() <= 0)
            return false;
        if(boulderAt(getX() - 1, getY()))
            return false;
        else
            return true;
    }
    if(d == up){
        if(getY() >= VIEW_HEIGHT - actorSize)
            return false;
        if(boulderAt(getX(), getY() + 1))
            return false;
        else
            return true;
    }
    if(d == down){
        if(getY() <= 0)
            return false;
        if(boulderAt(getX(), getY() - 1))
            return false;
        else
            return true;
    }
    
    return false; //This will never be hit
}

bool TunnelMan::boulderAt(int x, int y) const{
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

//Boulder class method implementations:
Boulder::Boulder(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_BOULDER, startX, startY, down, 1, 1.0){
    setVisible(true); //Boulders start out visible
    
    //When Boulders are constructed they replace/destroy the earth behind them
    for(int i = 0; i < actorSize; i++){
        for(int j = 0; j < actorSize; j++){
            getWorld()->removeEarth(startX + j, startY + i);
        }
    }
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
        if(ticksElapsed > ticksBeforeFalling){
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
            //smushCharacters();
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
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            Actor* doomed = getWorld()->findActor(x, y, 'T');
            if(doomed != nullptr && doomed->getLiveStatus()){
                doomed->annoy(100);
            }
        }
    }
}
bool Boulder::boulderCanFall() const{
    int x = getX();
    int y = getY();
    if(y == 0)
        return false; //Hits the bottom of the screen so boulder cannot fall any longer
    
    for(int i = 0; i < actorSize; i++){
        if(getWorld()->isEarthAt(x + i, y - 1) || getWorld()->findActor(x + i, y - 1, 'B')){
            return false;
        }
    }

    return true;
}


//Squirt class method implementations
//Squirt::Squirt(StudentWorld* myWorld, TunnelMan* owner) : Actor(myWorld, TID_WATER_SPURT, owner->getX(), owner->getY(), owner->getDirection(), 1, 1.0){
//    travelDistance = 4;
//}

//void Squirt::doSomething(){
//
//}
 


