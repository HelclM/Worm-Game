#include"Classes.h"
#include<iostream>

using namespace std;
 
int Control::speed=3;
int Control::up=10;
int Control::down=25;
int Control::checkh=10;
int Control::jumpx=4;
int Control::jumpy=10;
bool Control::exitGame=false;
bool Control::delay=false;
bool Control::explosion=false;
bool Control::shotFlying=false;
bool Control::wormFlying=false;
bool Control::gamePaused=false;
mutex Control::mu;
 
Control::Control(Texture* tex){
    rectex=tex;
}

void Control::GoLeft(){
    int count=1;
    recworm->dir=0;
    //can go left control
    if(rectex->valueat(recworm->dest.x,recworm->dest.y)!=1 
            && rectex->valueat(recworm->dest.x,recworm->dest.y+(recworm->dest.h)/2)!=1 
            && rectex->valueat(recworm->dest.x+(recworm->dest.w/2),recworm->dest.y+recworm->dest.h)==1){
        //if go up
        if(rectex->valueat(recworm->dest.x-speed,recworm->dest.y+recworm->dest.h-checkh)==1){
            while(rectex->valueat(recworm->dest.x-speed,recworm->dest.y+recworm->dest.h-checkh-count)==1){
                if(count<100){
                    ++count;
                }
                else{
                    break;
                }
            }
            if(count<=up){
                recworm->moveWorm(recworm->dest.x-speed,recworm->dest.y-checkh-count+1);
  
          }           
        }
        //if down
        if(rectex->valueat(recworm->dest.x+(recworm->dest.w/2)-speed,recworm->dest.y+recworm->dest.h-checkh)!=1){
           while(rectex->valueat(recworm->dest.x+(recworm->dest.w/2)-speed,recworm->dest.y+recworm->dest.h-checkh+count)!=1){
                if(count<100){
                    ++count;
                }
                else{
                    break;
                }
            }
           if(count<=down){
                recworm->moveWorm(recworm->dest.x-speed,recworm->dest.y-checkh+count);
            }
           else{
               recworm->moveWorm(recworm->dest.x-speed,recworm->dest.y);
           }
        }
    }
}

void Control::GoRight(){
    int count=1;
    recworm->dir=1;
    //can go right control
    if(rectex->valueat(recworm->dest.x+recworm->dest.w,recworm->dest.y)!=1
        && rectex->valueat(recworm->dest.x+recworm->dest.w,recworm->dest.y+(recworm->dest.h)/2)!=1
        && rectex->valueat(recworm->dest.x+(recworm->dest.w/2),recworm->dest.y+recworm->dest.h)==1){
        //if go up
        if(rectex->valueat(recworm->dest.x+recworm->dest.w+speed,recworm->dest.y+recworm->dest.h-checkh)==1){
            while(rectex->valueat(recworm->dest.x+recworm->dest.w+speed,recworm->dest.y+recworm->dest.h-checkh-count)==1){
                if(count<100){
                    ++count;
                }
                else{
                    break;
                }
            }
            if(count<=up){;
                recworm->moveWorm(recworm->dest.x+speed,recworm->dest.y-checkh-count+1);
            }           
        }
        //if down
        if(rectex->valueat(recworm->dest.x+(recworm->dest.w/2)+speed,recworm->dest.y+recworm->dest.h-checkh)!=1){
           while(rectex->valueat(recworm->dest.x+(recworm->dest.w/2)+speed,recworm->dest.y+recworm->dest.h-checkh+count)!=1){
                if(count<100){
                    ++count;
                }
                else{
                    break;
                }
            }
           if(count<=down){
                recworm->moveWorm(recworm->dest.x+speed,recworm->dest.y-checkh+count);
            }
           else{
               recworm->moveWorm(recworm->dest.x+speed,recworm->dest.y);
           }
        }
    }
}

void Control::Gravity(){
    for (int i = 0; i <(int)team1.size(); i++) {
        wormFall(team1[i]);
    }
    for (int i = 0; i <(int)team2.size(); i++) {
        wormFall(team2[i]);
    }
}

int Control::fallDamage(double vx, double vy){
    double v,dmg;
    v=sqrt(pow(vy,2)+pow(1.3*vx,2));
//    cout<<"v= "<<v<<endl;
    if(false){
        dmg=pow(2,(v-18)/1.7);
    }
    else{
        dmg=1.3*(v-15)+1;
    }
//    cout<<"dmg= "<<dmg<<endl;
    if(dmg>0){
        return (int)dmg;
    }
    else{
        return 0;
    }
}

void Control::wormFall(Worm* w){
    int countfall=1;
    //if fall worm
    if(rectex->valueat(w->dest.x+(w->dest.w/2),w->dest.y+w->dest.h)==-1){
        w->health=-1;
        w->flying=false;
    }
    else if(rectex->valueat(w->dest.x+(w->dest.w/2),w->dest.y+w->dest.h)!=1){
        if(w!=recworm){
            rectex->eraseWorm(w);
        }
        while(rectex->valueat(w->dest.x+(w->dest.w/2),w->dest.y+w->dest.h+countfall)!=1 && countfall<MySDL::GAME_WINDOW_HEIGHT){
            ++countfall;
        }
        if(countfall<=w->fallspeed){
            w->moveWorm(w->dest.x,w->dest.y+countfall);
            if(w!=recworm){
               rectex->writeWorm(w);
            }
            if(!w->firstjump){
                if(w->flying){
                    w->health-=fallDamage((double)w->flyspeed.x+5,(double)(w->flyspeed.y-w->fallspeed+10));
                }
                else if(w->jumping){
                    w->health-=fallDamage((double)jumpx,(double)(jumpy-w->fallspeed));
                }
                else{
                    w->health-=fallDamage(0.,(double)w->fallspeed);
                }
            }
            w->fallspeed=0;
            w->firstjump=false;
        }
        else{
            w->moveWorm(w->dest.x,w->dest.y+w->fallspeed);
            ++w->fallspeed;
        }
    }
}

bool Control::CanFly(int first, SDL_Rect dest, int dir){
    if(first==1){
        if(dir==0){
        return (rectex->valueat(dest.x,dest.y)!=1 
                && rectex->valueat(dest.x,dest.y+(dest.h)/2)!=1);
        }
        else{
        return (rectex->valueat(dest.x+dest.w,dest.y)!=1
        && rectex->valueat(dest.x+dest.w,dest.y+(dest.h)/2)!=1);
        }
    }
    else{
        if(dir==0){
        return (rectex->valueat(dest.x,dest.y)!=1 
                && rectex->valueat(dest.x,dest.y+(dest.h)/2)!=1
                && rectex->valueat(dest.x+(dest.w/2),dest.y+dest.h)!=1);
        }
        else{
        return (rectex->valueat(dest.x+dest.w,dest.y)!=1
        && rectex->valueat(dest.x+dest.w,dest.y+(dest.h)/2)!=1
        && rectex->valueat(dest.x+(dest.w/2),dest.y+dest.h)!=1);
        }
    }
}

void Control::Jump(Worm* w){
    if(w->dir==0){
        w->moveWorm(w->dest.x-jumpx,w->dest.y-jumpy);
    }
    else{
        w->moveWorm(w->dest.x+jumpx,w->dest.y-jumpy);
    }
}

coord Control::FireSpeed(int x, int y){   
    double angle;
    coord v0;
    recworm->weap->destweap.x=recworm->dest.x+recworm->dest.w/2-recworm->weap->destweap.w/2;
    recworm->weap->destweap.y=recworm->dest.y+recworm->dest.h/1.5-recworm->weap->destweap.h/2;
    angle=atan2(y-(recworm->dest.y+recworm->dest.h/2),x-(recworm->dest.x+recworm->dest.w/2-srcdest->x));
    if(distance({x,y},{recworm->dest.x+recworm->dest.w/2-srcdest->x,
            recworm->dest.y+recworm->dest.h/2})<recworm->weap->maxspeed){
        v0.x=x-(recworm->dest.x+recworm->dest.w/2-srcdest->x);
        v0.y=y-(recworm->dest.y+recworm->dest.h/2);
    }
    else{
        v0.x=recworm->weap->maxspeed*cos(angle);
        v0.y=recworm->weap->maxspeed*sin(angle);
    }
    angle=(angle/M_PI)*180;
    if(angle>90 || angle<-90){
        recworm->weap->flip=SDL_FLIP_VERTICAL;
    }
    else{
        recworm->weap->flip=SDL_FLIP_NONE;
    }
    recworm->weap->angle=angle;
    return v0;
}
void Control::WormShoot(coord v0){
    recworm->weap->projectile.x=recworm->dest.x+recworm->dest.w/2;
    recworm->weap->projectile.y=recworm->dest.y+recworm->dest.h/2;
    recworm->weap->projectile.vx=v0.x/3;
    recworm->weap->projectile.vy=v0.y/3;
    recworm->weap->projectile.flying=true;
}

void Control::handleEvents(){
    if(input->wormFiring()){
        recworm->weap->v0=FireSpeed(input->getMouse().x,input->getMouse().y);
        recworm->firing=true;
    }
    else{
        recworm->firing=false;
    }
    while(!input->queueEmpty()){
        char a;
        a=input->takeFromQueue();
        switch(a){
            case 'f':
                WormShoot(recworm->weap->v0);
                shotFlying=true;
                break;
            case 'j':
                recworm->jumping=true;
                recworm->startJumping=1;
                break;
            case 'c':
                if(++counterChangeWeap>2){
                    recworm->nextWeapon();
                    counterChangeWeap=0;
                }
                break;
            case 'e':
                recworm->firstRound=true;
                break;
        }
        if(isdigit(a)){
            int i=a-'0';
            recworm->changeWeapon(i-1);
        }
    }
    if(input->moveLeft()){
        GoLeft();
    }
    if(input->moveRight()){
        GoRight();
    }
}

coord Control::getMouseCoord(){
    return input->getMouse();
}

void Control::setWormFlySpeed(Worm* w){
    coord wormCenter, exp;
    double v, angle;
    exp=recworm->weap->getStrikeCoord();
    wormCenter.x=w->dest.x+w->dest.w/2;
    wormCenter.y=w->dest.y+w->dest.h/2;
    v=(recworm->weap->exploded.pressure-(distance(exp,wormCenter)/recworm->weap->exploded.divide)
            *recworm->weap->exploded.pressure)/1.5;
    angle=atan2(exp.y-wormCenter.y,exp.x-wormCenter.x);
    w->flyspeed.x=v*cos(angle);
    w->flyspeed.y=v*sin(angle);
//    cout<<"vx= "<<w->flyspeed.x<<endl;
//    cout<<"vy= "<<w->flyspeed.y<<endl;
}

void Control::handleWormsJumping(){
    for (int i = 0; i < (int)team1.size(); i++) {
        if(CanFly(team1[i]->startJumping,team1[i]->dest,team1[i]->dir) && team1[i]->jumping){
        Jump(team1[i]);
        ++team1[i]->startJumping;
        }
        if(!CanFly(team1[i]->startJumping,team1[i]->dest,team1[i]->dir) && team1[i]->jumping){
            if(team1[i]->fallspeed!=0){
                if(team1[i]->fallspeed>=jumpy){
                    team1[i]->fallspeed-=jumpy;
                }
                else{
                    team1[i]->fallspeed=0;
                }
            }
            team1[i]->jumping=false;
        }
    }
    for (int i = 0; i < (int)team2.size(); i++) {
        if(CanFly(team2[i]->startJumping,team2[i]->dest,team2[i]->dir) && team2[i]->jumping){
        Jump(team2[i]);
        ++team2[i]->startJumping;
        }
        if(!CanFly(team2[i]->startJumping,team2[i]->dest,team2[i]->dir) && team2[i]->jumping){
            if(team2[i]->fallspeed>=jumpy){
                    team2[i]->fallspeed-=jumpy;
                }
                else{
                    team2[i]->fallspeed=0;
                }
            team2[i]->jumping=false;
        }
    }
}

void Control::logic(){
    //cout<<"recworm,fallspeed= "<<recworm->fallspeed<<endl;
    //bullet fly control
    if(recworm->weap->projectile.flying){
        recworm->weap->Shoot(rectex,wind);
    }
    else{
        shotFlying=false;
    }
    handleExplosion();
    handleWormsJumping();
    Gravity();
    //pause when animating
    if(pauseCounter-->0){
        gamePaused=true;
    }
    else{
        gamePaused=false;
    }
    if(animating()){
        timer.pause();
    }
    else if(pausedGame()){
        timer.pause();
    }
    else{
        timer.unpause();
    }
    handleWormsFlying();
    // go to next round control
    if(timer.getTime()>30){
        goToNextRound=true;
    }
    if(recworm->health!=recworm->lasthealth){
        goToNextRound=true;
    }
    if(goToNextRound && !animating()){
        nextRound();
        goToNextRound=false;
    }
    eraseDeadWorms();
    if(!Control::animating()){
        removeWormsEmptyWeapons();
    }
}

void Control::createTeams(int teamSize){
    for (int i = 0; i < teamSize; i++) {
        team1.push_back(new Worm (1,teamSize));
        team2.push_back(new Worm (2,teamSize));
    }
    recworm=team2[teamSize-1];
    recworm->playing=true;
    lastworm1=teamSize-1;
    lastworm2=teamSize-1;
    lastteamplayed=2;
}

void Control::clearTeams(){
    while(!team1.empty()){
        delete team1.back();
        team1.pop_back();
    }
    while(!team2.empty()){
        delete team2.back();
        team2.pop_back();
    }
}

double Control::distance(coord a, coord b){
    return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}

int Control::explosionDamage(double power, double rad, double dist){
    double dmg,c1,c2;
//    cout<<"rad= "<<rad<<endl;
//    cout<<"power= "<<power<<endl;
//    cout<<"dist= "<<dist<<endl;
    c1=-(3.0/4)*(power/rad);
    c2=power;
    dmg=c1*dist+c2;
//    cout<<"explosion dmg= "<<dmg<<endl;
    if (dmg>=0){
        return dmg;
    }
    else{
        cout<<"negative dmg!!"<<endl;
        return 0;
    }
}

void Control::handleExplosion(){
    coord exp=recworm->weap->getStrikeCoord();
    if(recworm->weap->exploded.exp==true){
        //handle worms health and worms speed
        for (int i = 0; i < (int)team1.size(); i++) {
            if(distance(exp,{team1[i]->dest.x+team1[i]->dest.w/2,team1[i]->dest.y+team1[i]->dest.h/2})
                    <recworm->weap->exploded.rad){
//                team1[i]->health-=recworm->weap->exploded.power-
//                        distance(exp,{team1[i]->dest.x,team1[i]->dest.y})/3;
                team1[i]->health-=explosionDamage((double)recworm->weap->exploded.power,
                        (double)recworm->weap->exploded.rad,
                        distance(exp,{team1[i]->dest.x+team1[i]->dest.w/2,team1[i]->dest.y+team1[i]->dest.h/2}));
                setWormFlySpeed(team1[i]);
            }
        }
        for (int i = 0; i < (int)team2.size(); i++) {
            if(distance(exp,{team2[i]->dest.x+team2[i]->dest.w/2,team2[i]->dest.y+team2[i]->dest.h/2})
                    <recworm->weap->exploded.rad){
//                team2[i]->health-=recworm->weap->exploded.power
//                    -distance(exp,{team2[i]->dest.x,team2[i]->dest.y})/3;
                team2[i]->health-=explosionDamage((double)recworm->weap->exploded.power,
                        (double)recworm->weap->exploded.rad,
                        distance(exp,{team2[i]->dest.x+team2[i]->dest.w/2,team2[i]->dest.y+team2[i]->dest.h/2}));
                setWormFlySpeed(team2[i]);
            }
        }
        //handle map changes
        for (int i = exp.x-recworm->weap->exploded.rad; i < exp.x+recworm->weap->exploded.rad; i++) {
            for (int j = exp.y-recworm->weap->exploded.rad; j < exp.y+recworm->weap->exploded.rad; j++) {
                if(distance({exp.x,exp.y},{i,j})<(double)recworm->weap->exploded.rad*2/3
                    && rectex->valueat(i,j)==1){
                        rectex->writePoint(i,j,0);
                    backChanged=true;
                }
            }
        }
        recworm->weap->exploded.exp=false;
        goToNextRound=true;
    }
}

void Control::eraseDeadWorms(){
    for (int i = 0; i < (int)team1.size(); i++) {
        if(team1[i]->health<=0){
            if(team1[i]!=recworm){
                rectex->eraseWorm(team1[i]);
                delete team1[i];
                team1.erase(team1.begin()+i);
            }
        }
    }
    for (int i = 0; i < (int)team2.size(); i++) {
        if(team2[i]->health<=0){
            if(team2[i]!=recworm){
                rectex->eraseWorm(team2[i]);
                delete team2[i];
                team2.erase(team2.begin()+i);
            }
        }
    }
    if(team1.size()==0){
        exitGame=true;
        //delay=true;
    }
    if(team2.size()==0){
        exitGame=true;
        //delay=true;
    }
}

void Control::nextRound(){
    if(!recworm->jumping){
        rectex->writeWorm(recworm);
    }
    recworm->playing=false;
    recworm->firing=false;
    if(lastteamplayed==1){
        if(lastworm2<(int)team2.size()-1){
            recworm=team2[++lastworm2];
            lastteamplayed=2;
        }
        else{
            recworm=team2[0];
            lastworm2=0;
            lastteamplayed=2;
        }
    }
    else if(lastteamplayed==2){
        if(lastworm1<(int)team1.size()-1){
            recworm=team1[++lastworm1];
            lastteamplayed=1;
        }
        else{
            recworm=team1[0];
            lastworm1=0;
            lastteamplayed=1;
        }
    }
    //change wind
    wind=rand() % 14 -7;
    timer.reset();
    rectex->eraseWorm(recworm);
    recworm->playing=true;
    recworm->firstRound=true;
    pauseCounter=50;
    //cout<<"changing worm"<<endl;
}

void Control::setSrcdestRef(SDL_Rect* dest){
    srcdest=dest;
}

void Control::setInputRef(Input* i){
    input=i;
}

void Control::handleWormsFlying(){
    int f=0,d=0;
    f+=handleTeamFlying(team1);
    f+=handleTeamFlying(team2);
    for (int i = 0; i < (int)team1.size(); i++) {
        if(team1[i]->flying==true){
            ++d;
        }
    }
    for (int i = 0; i < (int)team2.size(); i++) {
        if(team2[i]->flying==true){
            ++d;
        }
    }
    if(d>0){
        wormFlying=true;
    }
    else{
        wormFlying=false;
    }
}

int Control::handleTeamFlying(vector<Worm*> team){
    int counter=0,c=0;
    double x,y;
    SDL_Rect tmpdest;
    for (int i = 0; i < (int)team.size(); i++) {
        x=team[i]->dest.x;
        y=team[i]->dest.y;
        tmpdest=team[i]->dest;
        counter=0;
        if((team[i]->flyspeed.x>0 && team[i]->flyspeed.y>=0) && CanFly(team[i]->firstFly,tmpdest,0)){
            if(team[i]!=recworm){
                rectex->eraseWorm(team[i]);
            }
//            cout<<"left"<<endl;
            while(CanFly(team[i]->firstFly,tmpdest,0) && counter<10){
                counter++;
//                cout<<"1: "<<counter<<endl;
                x-=(double)team[i]->flyspeed.x/10;
                y-=((double)team[i]->flyspeed.y)/10;
                tmpdest.x=x;
                tmpdest.y=y;
            }
            //cout<<"firstfly= "<<team[i]->firstFly<<endl;
            if(rectex->valueat(team[i]->dest.x,team[i]->dest.y)!=-1){
                team[i]->flying=true;
            }
            ++team[i]->firstFly;
            ++c;
        }        
        else if((team[i]->flyspeed.x<=0 && team[i]->flyspeed.y>0) && CanFly(team[i]->firstFly,tmpdest,1)){
            if(team[i]!=recworm){
                rectex->eraseWorm(team[i]);
            }
//            cout<<"right"<<endl;
            while(CanFly(team[i]->firstFly,tmpdest,1) && counter<10){
                counter++;
//                cout<<"2: "<<counter<<endl;
                x-=(double)team[i]->flyspeed.x/10;
                y-=((double)team[i]->flyspeed.y)/10;
                tmpdest.x=x;
                tmpdest.y=y;
            }
            //cout<<"firstfly= "<<team[i]->firstFly<<endl;
            if(rectex->valueat(team[i]->dest.x,team[i]->dest.y)!=-1){
                team[i]->flying=true;
            }
            ++team[i]->firstFly;
            ++c;
        }
        else{
            team[i]->flyspeed.x=0;
            team[i]->flyspeed.y=0;
//            if(team[i]->flying){
//                cout<<"reset"<<endl;
//            }
            team[i]->flying=false;
            team[i]->firstFly=1;
        }
//        if(team[i]->flying){
//                cout<<"vx= "<<team[i]->flyspeed.x<<endl;
//                cout<<"vy= "<<team[i]->flyspeed.y<<endl;
//            }
        team[i]->moveWorm(x,y);
        if(team[i]!=recworm && !team[i]->jumping){
            rectex->writeWorm(team[i]);
        }
    }
    return c;
}

bool Control::animating(){
    std::lock_guard<mutex> guard(mu);
    return (shotFlying || wormFlying || explosion);
}

void Control::eraseWormWeap(Worm* w){
    for (int i = 0; i < (int)w->weapons.size(); i++) {
        if(w->weapons[i]->charges<=0){
            w->nextWeapon();
            delete w->weapons[i];
            w->weapons.erase(w->weapons.begin()+i);
        }
    }

}

void Control::removeWormsEmptyWeapons(){
    for (int i = 0; i <(int)team1.size(); i++) {
        eraseWormWeap(team1[i]);
    }
    for (int i = 0; i <(int)team2.size(); i++) {
        eraseWormWeap(team2[i]);
    }
}

bool Control::pausedGame(){
    std::lock_guard<mutex> guard(mu);
    return gamePaused;
}