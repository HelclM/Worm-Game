#include"Classes.h"

Granade::Granade(){
    SDL_Surface* explodesur=IMG_Load("./ingame_images/explosion.png");
    SDL_Surface* weapsur=IMG_Load("./ingame_images/granade.png");
    SDL_Surface* projsur=IMG_Load("./ingame_images/granade.png");
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
    projectile.h=25;
    projectile.w=29;
    projectile.flip=SDL_FLIP_NONE;
    projectile.angle=0;
    windProof=30;
    exploded.exp=false;
    exploded.power=45;
    exploded.rad=70;
    destexp.w=2*exploded.rad+10;
    destexp.h=2*exploded.rad+10;
    exploded.pressure=40;
    exploded.divide=90;
    charges=5;
}

Granade::~Granade(){
    SDL_DestroyTexture(explode);
    SDL_DestroyTexture(weaptex);
    SDL_DestroyTexture(projectile.bulltex);
}

void Granade::Shoot(Texture* tex, int wind){
    int counter=0, changed=0;
    if(firstShoot){
        lx=projectile.x;
        ly=projectile.y;
        timer.reset();
        firstShoot=false;
    }
    while(tex->valueat(lx,ly)!=1 && counter<maxspeed){
        ++counter;
        lx=lx-(double)projectile.vx/maxspeed;
        ly=ly-(double)projectile.vy/maxspeed;
    }
    if(counter==maxspeed){
        projectile.x-=projectile.vx;
        projectile.y-=projectile.vy;
    }
    else{
        if(tex->valueat(lx,projectile.y)==1){
            projectile.vx=-projectile.vx/3;
            projectile.vy=projectile.vy/2;
            ++changed;
        }
        if(tex->valueat(projectile.x,ly)==1){
            projectile.vx=projectile.vx/2;
            projectile.vy=-projectile.vy/3;
            ++changed;
        }
        if(abs(projectile.vx)<2){
            projectile.vx=0;
        }
        if(abs(projectile.vy)<2){
            projectile.vy=0;
        }
        if(changed==0){
            projectile.vx=-projectile.vx/2;
            projectile.vy=-projectile.vy/2;
        }
        projectile.x-=projectile.vx;
        projectile.y-=projectile.vy;
        lx=projectile.x;
        ly=projectile.y;
    }
    if(timer.getTime()>3){
        strike(lx,ly);
        projectile.flying=false;
    }
    --projectile.vy;
    if(projectile.vx<0){
        projectile.angle=(atan2(projectile.vy,projectile.vx)/M_PI)*180+180;
        
    }
    else{
        projectile.angle=(atan2(projectile.vy,projectile.vx)/M_PI)*180;
    }
    //wind 
//    if(++windCounter%windProof==0){
//        projectile.vx-=wind;
//    }
}

void Granade::strike(int x, int y){
    destexp.x=x-destexp.w/2;
    destexp.y=y-destexp.h/2;
    exploded.exp=true;
    exploded.counter=40;
    Control::explosion=true;
    firstShoot=true;
    --charges;
}

coord Granade::getStrikeCoord(){
    return {(int)lx,(int)ly};
}