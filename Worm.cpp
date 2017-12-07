#include"Classes.h"

int Worm::wormCounter=0;

Worm::Worm(int iteam, int teamsize){
    ++wormCounter;
    id=wormCounter;
    team=iteam;
    dest.x=rand() % (MySDL::GAME_MAP_WIDTH-200) +100;
    dest.y=0;
    dest.w=20;
    dest.h=40;
    flyspeed.x=0;
    flyspeed.y=0;
    startJumping=2;
    //cout<<"worm ready: "<<id<<", "<<x<<", "<<team<<endl;
    team=iteam;
    dir=0;
    health=100;
    lasthealth=-1000;
    SDL_Surface* sur=IMG_Load("./ingame_images/worm.png");
    if(sur==NULL){
        cout<<"error sur worm: "<<SDL_GetError()<<endl;
    }
    tex=SDL_CreateTextureFromSurface(MySDL::rend,sur);
    if (tex==NULL){
        cout<<"errortex:"<<SDL_GetError()<<endl;
    }
    SDL_FreeSurface(sur);
    weapons.push_back(new Rocket());
    weapons.push_back(new Granade());
    weapons.push_back(new Dynamite());
    weapons.push_back(new Shotgun());
    weapons.push_back(new BSBat());
    weap=weapons[0];
}
Worm::~Worm(){
    for (int i = 0; i < (int)weapons.size(); i++) {
        delete weapons[i];
    }
    SDL_DestroyTexture(tex);
}
void Worm::moveWorm(int ix, int iy){
    dest.x=ix;
    dest.y=iy;
}

void Worm::nextWeapon(){
    int i=0;
    for ( i = 0; i < (int)weapons.size(); i++) {
        if(weap==weapons[i]) break;
    }
    if(i<(int)weapons.size()-1){
        weap=weapons[++i];
    }
    else{
        weap=weapons[0];
    }
}

void Worm::changeWeapon(int i){
    if(i<(int)weapons.size()){
        weap=weapons[i];
    }
}