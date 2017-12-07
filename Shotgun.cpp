#include"Classes.h"

Shotgun::Shotgun(){
    SDL_Surface* explodesur=IMG_Load("./ingame_images/explosion.png");
    SDL_Surface* weapsur=IMG_Load("./ingame_images/shotgun.png");
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
    maxspeed=90;
    destweap.w=29;
    destweap.h=25;
    projectile.flying=false;
    projectile.h=1;
    projectile.w=1;
    projectile.flip=SDL_FLIP_NONE;
    projectile.angle=0;
    windProof=5;
    exploded.exp=false;
    exploded.power=43;
    exploded.rad=25;
    destexp.w=2*exploded.rad+10;
    destexp.h=2*exploded.rad+10;
    exploded.pressure=15;
    exploded.divide=50;
    charges=5;
}

Shotgun::~Shotgun(){
    SDL_DestroyTexture(explode);
    SDL_DestroyTexture(weaptex);
    SDL_DestroyTexture(projectile.bulltex);
}

void Shotgun::Shoot(Texture* tex, int wind){
    int counter=0;
    lx=projectile.x;
    ly=projectile.y;
    while(tex->valueat(lx,ly)==0 && counter<50000){
        ++counter;
        lx=lx-(double)projectile.vx*2000/50000;
        ly=ly-(double)projectile.vy*2000/50000;
    }
//    if(!firstShoot){
//        projectile.flying=false;
//    }
    firstShoot=false;
    projectile.flying=false;
    strike(lx,ly);
}

void Shotgun::strike(int x, int y){
    destexp.x=x-destexp.w/2;
    destexp.y=y-destexp.h/2;
    exploded.exp=true;
    exploded.counter=10;
    Control::explosion=true;
    --charges;
}

coord Shotgun::getStrikeCoord(){
    return{(int)lx,(int)ly};
}