#include"Classes.h"

BSBat::BSBat(){
    SDL_Surface* explodesur=IMG_Load("./ingame_images/explosion.png");
    SDL_Surface* weapsur=IMG_Load("./ingame_images/Baseballbat.png");
    SDL_Surface* projsur=IMG_Load("./ingame_images/Projectile.png");
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
    maxspeed=40;
    destexp.w=10;
    destexp.h=10;
    destweap.w=25;
    destweap.h=30;
    projectile.flying=false;
    projectile.h=6;
    projectile.w=6;
    projectile.flip=SDL_FLIP_NONE;
    projectile.angle=0;
    windProof=100;
    exploded.exp=false;
    exploded.power=100;
    exploded.rad=12;
    exploded.pressure=20;
    exploded.divide=70;
    charges=1;
}

BSBat::~BSBat(){
    SDL_DestroyTexture(explode);
    SDL_DestroyTexture(weaptex);
    SDL_DestroyTexture(projectile.bulltex);
}

void BSBat::Shoot(Texture* tex, int wind){
    projectile.flying=false;
    strike(projectile.x-projectile.vx,projectile.y-projectile.vy);
}

void BSBat::strike(int x, int y){
    destexp.x=x;
    destexp.y=y;
    exploded.exp=true;
    exploded.counter=0;
    Control::explosion=true;
    --charges;
}

coord BSBat::getStrikeCoord(){
    return {destexp.x,destexp.y};
}