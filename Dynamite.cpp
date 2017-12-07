#include"Classes.h"

Dynamite::Dynamite(){
    SDL_Surface* explodesur=IMG_Load("./ingame_images/explosion.png");
    SDL_Surface* weapsur=IMG_Load("./ingame_images/dynamite.png");
    SDL_Surface* projsur=IMG_Load("./ingame_images/dynamite_proj.png");
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
    maxspeed=30;
    destweap.w=25;
    destweap.h=29;
    projectile.flying=false;
    projectile.h=29;
    projectile.w=25;
    projectile.flip=SDL_FLIP_NONE;
    projectile.angle=0;
    windProof=5;
    exploded.exp=false;
    exploded.power=53;
    exploded.rad=65;
    destexp.w=2*exploded.rad+10;
    destexp.h=2*exploded.rad+10;
    exploded.pressure=40;
    exploded.divide=80;
    charges=5;
}

Dynamite::~Dynamite(){
    SDL_DestroyTexture(explode);
    SDL_DestroyTexture(weaptex);
    SDL_DestroyTexture(projectile.bulltex);
}

void Dynamite::Shoot(Texture* tex, int wind){
    int counter=0;
    if(firstShoot){
        projectile.vy=0;
        lx=projectile.x;
        ly=projectile.y;
        timer.reset();
    }
    while(tex->valueat(lx,ly)!=1 && counter<maxspeed){
        ++counter;
        lx=lx-(double)projectile.vx/maxspeed;
        ly=ly-(double)projectile.vy/maxspeed;
        firstShoot=false;
    }
    if(counter==maxspeed){
        projectile.x-=projectile.vx;
        projectile.vx=0;
        projectile.y-=projectile.vy;
    }
    --projectile.vy;
    if(timer.getTime()>2){
        strike((int)lx,(int)ly);
        projectile.flying=false;
    }
}

void Dynamite::strike(int x, int y){
    destexp.x=x-destexp.w/2;
    destexp.y=y-destexp.h/2;
    exploded.exp=true;
    exploded.counter=40;
    Control::explosion=true;
    firstShoot=true;
    --charges;
}

coord Dynamite::getStrikeCoord(){
    return {(int)lx,(int)ly};
}
