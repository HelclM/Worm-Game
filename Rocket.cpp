#include"Classes.h"

Rocket::Rocket(){
    SDL_Surface* explodesur=IMG_Load("./ingame_images/explosion.png");
    SDL_Surface* weapsur=IMG_Load("./ingame_images/rocket.png");
    SDL_Surface* projsur=IMG_Load("./ingame_images/rocket_proj.png");
    explode=SDL_CreateTextureFromSurface(MySDL::rend,explodesur);
    if (explode==NULL){
        cout<<"error explode:"<<SDL_GetError()<<endl;
    }
    weaptex=SDL_CreateTextureFromSurface(MySDL::rend,weapsur);
    if (weaptex==NULL){
        cout<<"error weaptex:"<<SDL_GetError()<<endl;
    }
    projectile.bulltex=SDL_CreateTextureFromSurface(MySDL::rend,projsur);
    if (projectile.bulltex==NULL){
        cout<<"error bulltex:"<<SDL_GetError()<<endl;
    }
    SDL_FreeSurface(weapsur);
    SDL_FreeSurface(explodesur);
    SDL_FreeSurface(projsur);
    maxspeed=110;
    destweap.w=29;
    destweap.h=25;
    projectile.flying=false;
    projectile.h=20;
    projectile.w=25;
    projectile.flip=SDL_FLIP_NONE;
    projectile.angle=0;
    windProof=5;
    exploded.exp=false;
    exploded.power=40;
    exploded.rad=55;
    destexp.w=2*exploded.rad+10;
    destexp.h=2*exploded.rad+10;
    exploded.pressure=35;
    exploded.divide=70;
    charges=9;
}
Rocket::~Rocket(){
    SDL_DestroyTexture(explode);
    SDL_DestroyTexture(weaptex);
    SDL_DestroyTexture(projectile.bulltex);
}

void Rocket::Shoot(Texture* tex, int wind){
    int counter=0;
    if(firstShoot){
        lx=projectile.x;
        ly=projectile.y;
    }
    while(tex->valueat(lx,ly)==0 && counter<maxspeed){
        ++counter;
        lx=lx-(double)projectile.vx/maxspeed;
        ly=ly-(double)projectile.vy/maxspeed;
        firstShoot=false;
    }
    if(counter==maxspeed){
        projectile.x-=projectile.vx;
        projectile.y-=projectile.vy;
    }
    else if(counter<1000){
        projectile.flying=false;
        strike(lx,ly);
    }
    --projectile.vy;
    projectile.angle=(atan2(projectile.vy,projectile.vx)/M_PI)*180;
    //wind 
    if(++windCounter%windProof==0){
        projectile.vx-=wind;
    }
}


void Rocket::strike(int x, int y){
    destexp.x=x-destexp.w/2;
    destexp.y=y-destexp.h/2;
    exploded.exp=true;
    exploded.counter=40;
    Control::explosion=true;
    firstShoot=true;
    --charges;
}

coord Rocket::getStrikeCoord(){
    return {(int)lx,(int)ly};
}