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
}

char Earth::getID() const{
    return 'E';
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
}

char TunnelMan::getID() const{
    return 'T';
}

void TunnelMan::doSomething(){
    if(!getLiveStatus()){
        return;
    }
    int x = getX();
    int y = getY();
    bool earthNeedsToBeRemoved = false;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(getWorld()->spotContains(x+i, y+j, 'E', 0.25)){
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
                if(getDirection() == right && getX() < VIEW_WIDTH-4 && !getWorld()->spotContains4(x+1, y, 'B'))
                    getWorld()->moveActor(this);
                else
                    setDirection(right);
            }
            if(ch == KEY_PRESS_LEFT && getX() > 0 && !getWorld()->spotContains4(x-1, y, 'B')){
                if(getDirection() == left)
                    getWorld()->moveActor(this);
                else
                    setDirection(left);
            }
            if(ch == KEY_PRESS_UP && getY() < VIEW_HEIGHT-4 && !getWorld()->spotContains4(x, y+1, 'B')){
                if(getDirection() == up)
                    getWorld()->moveActor(this);
                else
                    setDirection(up);
            }
            if(ch == KEY_PRESS_DOWN && getY() > 0 && !getWorld()->spotContains4(x, y-1, 'B')){
                if(getDirection() == down)
                    getWorld()->moveActor(this);
                else
                    setDirection(down);
            }
        }
    }
}


//Boulder class method implementations:
Boulder::Boulder(StudentWorld* myWorld, int startX, int startY) : Actor(myWorld, TID_BOULDER, startX, startY, down, 1, 1.0){
    state = 0; //Boulders start out in a stable state (0 = stable, 1 = waiting, 2 = falling)
    ticksElapsed = -1; //To track how many ticks have elapsed after entering waiting state. ticksElapsed = -1 when the boulder is not in a waiting state
}


char Boulder::getID() const{
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
        if(ticksElapsed > 30){
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
            getWorld()->moveActor(this);
            getWorld()->playSound(SOUND_FALLING_ROCK);
            //TODO: If boulder comes within a radius of 3 of any characters, it must cause 100 points of annoyance to those actors
        }
        else{
            setLiveStatus(false); //Set the boulder to dead
        }
        

    }
}

bool Boulder::boulderCanFall() const{
    int x = getX();
    int y = getY();
    if(y == 0)
        return false;
    
    for(int i = 0; i < 4; i++){
        if(getWorld()->spotContains(x, y-1, 'E', 0.25) || getWorld()->spotContains(x, y-1, 'B')){
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
 


