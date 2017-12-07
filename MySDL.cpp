#include"Classes.h"

using namespace std;

SDL_Renderer* MySDL::rend = NULL;
int MySDL::GAME_WINDOW_HEIGHT=700;
int MySDL::GAME_WINDOW_WIDTH=1200;
int MySDL::GAME_MAP_WIDTH=2000;

MySDL::MySDL(){
    srcdest.x=0;
    srcdest.y=0;
    srcdest.h=MySDL::GAME_WINDOW_HEIGHT;
    srcdest.w=MySDL::GAME_WINDOW_WIDTH;
}

void MySDL::Init(){
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0){
        cout<<"error init SDL "<<SDL_GetError()<<endl;
    }
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
}

void MySDL::CreateWinRen(const char* name){
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            GAME_WINDOW_WIDTH,GAME_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            | SDL_RENDERER_TARGETTEXTURE);
    if (!rend){
    cout<<"errorsurface:"<<SDL_GetError()<<endl;
    }
    backtex=SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, MySDL::GAME_MAP_WIDTH, MySDL::GAME_WINDOW_HEIGHT);
    SDL_Surface* arrow=IMG_Load("./ingame_images/arrow.png" );
    if(arrow==NULL){
        cout<<"error surface="<<IMG_GetError()<<endl;
    }
    texarrow=SDL_CreateTextureFromSurface(rend,arrow);
    if (texarrow==NULL){
        cout<<"arrow"<<endl;
        cout<<"errorbacktex:"<<SDL_GetError()<<endl;
    }
    SDL_FreeSurface(arrow);
    font=TTF_OpenFont("./Fonts/OpenSans-Light.ttf",100);
    if(font==NULL){
        cout<<"font error"<<TTF_GetError()<<endl;
    }    
    string ss="30";
    textime=RenderText(ss,{0,0,0,0});
}
void MySDL::DrawBack(){
    //cout<<"DrawBack"<<endl;
    int i=0;
    ++counter;
    if(counter==2 || c->backChanged){
        createBackTexture();
        c->backChanged=false;
    }
    i=SDL_RenderCopy(rend,backtex,&srcdest,NULL); 
    if(i!=0){
        cout<<"error RenderCopy back "<<SDL_GetError()<<endl;
    }
}

void MySDL::createBackTexture(){
    int i=0,time=SDL_GetTicks();
    i=SDL_SetRenderTarget(rend, backtex);
    if(i!=0){
        cout<<"SetRendererTarget error: "<<SDL_GetError()<<endl;
    }
    i=SDL_SetRenderDrawColor(rend,255,255,255,255);
    if(i!=0){
        cout<<"SetRendererDrawColor error: "<<SDL_GetError()<<endl;
    }
    i=SDL_RenderClear(rend);
    if(i!=0){
        cout<<"RenderClear error: "<<SDL_GetError()<<endl;
    }
    i=SDL_SetRenderDrawColor(rend,0,0,0,0);
    if(i!=0){
        cout<<"SetRendererDrawColor2 error: "<<SDL_GetError()<<endl;
    }
    for (int k = 0; k < MySDL::GAME_WINDOW_HEIGHT; k++) {
        for (int l = 0; l < MySDL::GAME_MAP_WIDTH; l++) {
            if(tex->valueat(l,k)==1){
                i=SDL_SetRenderDrawColor(rend,0,0,0,0);
                if(i!=0){
                 cout<<"SetRendererDrawColor2 error: "<<SDL_GetError()<<endl;
                }
                i=SDL_RenderDrawPoint(rend,l,k);
                if(i!=0){
                    cout<<"RenderDrawPoint error: "<<SDL_GetError()<<endl;
                }
            }
            else if(tex->valueat(l,k)==-1){
                i=SDL_SetRenderDrawColor(rend,50,100,200,0);
                if(i!=0){
                 cout<<"SetRendererDrawColor2 error: "<<SDL_GetError()<<endl;
                }
                i=SDL_RenderDrawPoint(rend,l,k);
                if(i!=0){
                    cout<<"RenderDrawPoint error: "<<SDL_GetError()<<endl;
                }
            }
//            else if(tex->valueat(l,k)==2){
//                i=SDL_SetRenderDrawColor(rend,50,100,0,0);
//                if(i!=0){
//                 cout<<"SetRendererDrawColor2 error: "<<SDL_GetError()<<endl;
//                }
//                i=SDL_RenderDrawPoint(rend,l,k);
//                if(i!=0){
//                    cout<<"RenderDrawPoint error: "<<SDL_GetError()<<endl;
//                }
//            }
        }
    }
    i=SDL_SetRenderTarget(rend, NULL);
    if(i!=0){
        cout<<"SetRendererTarget2 error: "<<SDL_GetError()<<endl;
    }
    cout<<"time creating back texture: "<<SDL_GetTicks()-time<<endl;
}

void MySDL::DrawWorm(Worm* w){
    SDL_RendererFlip flip;
    SDL_Rect dest, destarrow;
    SDL_Rect desttext;
    SDL_Color textColor;
    stringstream ss;
    if(w->team==1){
        textColor.r=0;
        textColor.g=0;
        textColor.b=255;
    }
    else if(w->team==2){
        textColor.r=255;
        textColor.g=0;
        textColor.b=0;
    }
    if(w->dir==1){
        flip=SDL_FLIP_HORIZONTAL;
    }
    else{
        flip=SDL_FLIP_NONE;
    }
    dest.x =w->dest.x;
    dest.y =w->dest.y;
    dest.w =w->dest.w;
    dest.h =w->dest.h;
    desttext.x=w->dest.x;
    desttext.y=w->dest.y-w->dest.h/2;
    desttext.w=30;
    destarrow.x=w->dest.x+w->dest.w/4;
    destarrow.y=w->dest.y-w->dest.h;
    destarrow.w=w->dest.w/2;
    destarrow.h=w->dest.h/2;
    if(w->lasthealth!=w->health){
        ss<<w->health;
        if(w->texttex!=NULL){
            SDL_DestroyTexture(w->texttex);
        }
        w->texttex=RenderText(ss.str(),textColor);
    }
    desttext.h=w->dest.h/2;
    desttext.w=20;
    w->lasthealth=w->health;
    if(w->health==100){
        desttext.w=30;
    }
    else if(w->health>=10 && w->health<100){
        desttext.w=20;
    }
    else{
        desttext.w=10;
    }
    if(w->playing){
        DrawPic(destarrow,texarrow);
    }
    DrawPic(desttext,w->texttex);
    DrawPic(dest,w->tex,0.0,flip);
    if(w->firing){
        double angle=(w->weap->angle/180)*M_PI;
        DrawPic(w->weap->destweap,w->weap->weaptex,w->weap->angle,w->weap->flip);
        if(c->distance({c->getMouseCoord().x,c->getMouseCoord().y},
        {w->dest.x+w->dest.w/2-srcdest.x,w->dest.y+w->dest.h/2})
        <w->weap->maxspeed){
            DrawLine(c->getMouseCoord().x,c->getMouseCoord().y,
                    w->dest.x+w->dest.w/2-srcdest.x,w->dest.y+w->dest.h/2);
        }
        else{
            DrawLine(w->dest.x+w->dest.w/2-srcdest.x+w->weap->maxspeed*cos(angle)
                    ,w->dest.y+w->dest.h/2+w->weap->maxspeed*sin(angle),
                    w->dest.x+w->dest.w/2-srcdest.x,w->dest.y+w->dest.h/2);
        }
    }
}

void MySDL::DrawLine(int x, int y, int wx, int wy){
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderDrawLine(rend,x,y,wx,wy);
}

void MySDL::DestroyWin(){
    SDL_DestroyTexture(backtex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
}
void MySDL::DrawProjectile(bullet proj){
    SDL_Rect dest;   
    dest.x =proj.x-proj.w/2;
    dest.y =proj.y-proj.h/2;
    dest.w =proj.w;
    dest.h =proj.h;
    DrawPic(dest,proj.bulltex,proj.angle,proj.flip);  
}
void MySDL::DrawPic(SDL_Rect destpic, SDL_Texture* tex, double angle, SDL_RendererFlip flip,SDL_Rect* srcpic){
    SDL_Rect dest;
    if(SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h)!=0){
        cout<<"query error: "<<SDL_GetError()<<endl;
    }   
    dest.x=destpic.x-srcdest.x;
    dest.y=destpic.y;
    dest.w=destpic.w;
    dest.h=destpic.h;
    SDL_RenderCopyEx(rend, tex, srcpic,&dest,angle,NULL,flip);
}
SDL_Texture* MySDL::RenderText(string text, SDL_Color textcolor){
    SDL_Texture* tex;
    SDL_Surface* textSurface=TTF_RenderText_Solid(font,text.c_str(),textcolor);
    if(textSurface==NULL){
        cout<<"text Surface error "<<TTF_GetError()<<endl;
    }
    tex=SDL_CreateTextureFromSurface(rend,textSurface);
    if(tex==NULL){
        cout<<"text tex error "<<TTF_GetError()<<endl;
    }
    SDL_FreeSurface(textSurface);
    return tex;
}

void MySDL::drawGame(){
    //cout<<"draw begin"<<endl;
    setCamera();
    DrawBack();
    for (int i = 0; i <(int) c->team1.size(); i++) {
        DrawWorm(c->team1[i]);
    }
    for (int i = 0; i <(int) c->team2.size(); i++) {
        DrawWorm(c->team2[i]);
    }
    if(Control::explosion){
        if(c->recworm->weap->exploded.counter>0){
            DrawPic(c->recworm->weap->destexp,c->recworm->weap->explode);
            --c->recworm->weap->exploded.counter;
        }
        else{
            Control::explosion=false;
        }
    }
    if(c->recworm->weap->projectile.flying==true){
        DrawProjectile(c->recworm->weap->projectile);
    }
    DrawWind();
    DrawTime();
    DrawWeapons();
    if(c->team1.empty()){
        drawAnnoucement("red teams wins",{255,0,0,0});
        Control::delay=true;
    }
    else if(c->team2.empty()){
        drawAnnoucement("blue teams wins",{0,0,255,0});
        Control::delay=true;
    }
    SDL_RenderPresent(rend);
    //cout<<"draw end"<<endl;
}

void MySDL::resizeTextDest(SDL_Rect* dest, string text,int letterW,int letterH){
    dest->h=letterH;
    dest->w=letterW*text.length();
}

void MySDL::DrawTime(){
    SDL_Rect dest=c->timer.getRect();
    dest.x=srcdest.x+c->timer.getRect().x;
//    if(!c->timer.paused()){
        stringstream ss;
        ss<<30-c->timer.getTime();
        SDL_DestroyTexture(textime);
        textime=RenderText(ss.str(),{0,0,0,0});
//    }
    DrawPic(dest,textime);
}

void MySDL::drawAnnoucement(string text, SDL_Color color){
    SDL_Texture* winnertex;
    SDL_Rect dest;
    winnertex=RenderText(text,color);
    dest.w=GAME_WINDOW_WIDTH/2;
    dest.h=GAME_WINDOW_HEIGHT/5;
    dest.y=GAME_WINDOW_HEIGHT/2-dest.h/2;
    dest.x=GAME_WINDOW_WIDTH/2-dest.w/2+srcdest.x;
    DrawPic(dest,winnertex);
}

void MySDL::setControl(Control* ic){
    c=ic;
}

void MySDL::setTextureRef(Texture* itex){
    tex=itex;
}

void MySDL::setCamera(){
    int cameraMove=15;
    if(c->recworm->firstRound){
        centerCameraAround(c->recworm->dest.x);
        c->recworm->firstRound=false;
    }
    else if(c->recworm->weap->projectile.flying &&
            abs(c->recworm->weap->projectile.x-c->recworm->dest.x)>MySDL::GAME_WINDOW_WIDTH/2){
        centerCameraAround(c->recworm->weap->projectile.x);
    }
    else{
        if(c->getMouseCoord().x>MySDL::GAME_WINDOW_WIDTH*19/20 
            && srcdest.x+MySDL::GAME_WINDOW_WIDTH+cameraMove<MySDL::GAME_MAP_WIDTH){
            srcdest.x+=cameraMove;
        }
        else if(c->getMouseCoord().x<MySDL::GAME_WINDOW_WIDTH*1/20 
            && srcdest.x-cameraMove>0){
            srcdest.x-=cameraMove;
        }
    }
}

void MySDL::centerCameraAround(int x){
    if(x-MySDL::GAME_WINDOW_WIDTH/2>0 &&
                x+MySDL::GAME_WINDOW_WIDTH/2<MySDL::GAME_MAP_WIDTH){
            srcdest.x=x-MySDL::GAME_WINDOW_WIDTH/2;
        }
        else if(x-MySDL::GAME_WINDOW_WIDTH/2>0 &&
                x+MySDL::GAME_WINDOW_WIDTH/2>MySDL::GAME_MAP_WIDTH){
            srcdest.x=MySDL::GAME_MAP_WIDTH-MySDL::GAME_WINDOW_WIDTH;
        }
        else{
            srcdest.x=0;
        }
}

SDL_Rect* MySDL::getSrcdestRef(){
    return &srcdest;
}

void MySDL::DrawWind(){
    if(c->wind!=0){
        SDL_Rect dest;
        SDL_RendererFlip flip;
        dest.w=30;
        dest.h=20*abs(c->wind);
        dest.x=srcdest.x+MySDL::GAME_WINDOW_WIDTH/2-dest.w/2;
        dest.y=60;
        if(c->wind<0){
            flip=SDL_FLIP_NONE;
        }
        else{
            flip=SDL_FLIP_VERTICAL;
        }
        DrawPic(dest, texarrow,90,flip);
    }
}

void MySDL::DrawWeapons(){
    SDL_Rect dest,chargedest;
    dest.x=srcdest.x+MySDL::GAME_WINDOW_WIDTH*2/3;
    dest.y=MySDL::GAME_WINDOW_HEIGHT-65;
    chargedest.h=40;
    chargedest.w=15;
    chargedest.y=dest.y+30;
    for (int i = 0; i < (int)c->recworm->weapons.size(); i++) {
        dest.x+=40;
        chargedest.x=dest.x;
        dest.h=c->recworm->weapons[i]->destweap.h;
        dest.w=c->recworm->weapons[i]->destweap.w;
        DrawPic(dest,c->recworm->weapons[i]->weaptex);
        if(c->recworm->weapons[i]->charges!=c->recworm->weapons[i]->lastCharges){
            c->recworm->weapons[i]->lastCharges=c->recworm->weapons[i]->charges;
            stringstream ss;
            ss<<c->recworm->weapons[i]->charges;
            if(c->recworm->weapons[i]->weapChargesTex!=NULL){
                SDL_DestroyTexture(c->recworm->weapons[i]->weapChargesTex);
            }
            c->recworm->weapons[i]->weapChargesTex=RenderText(ss.str(),{0,0,0,0});
        }
        DrawPic(chargedest,c->recworm->weapons[i]->weapChargesTex);
        if(c->recworm->weap==c->recworm->weapons[i]){
            SDL_Rect destar;
            destar.x=dest.x+dest.w/4;
            destar.y=dest.y-dest.h;
            destar.h=dest.h;
            destar.w=dest.w/2;
            DrawPic(destar,texarrow);
        }
    }

}

void MySDL::DrawLoadingScreen(){
    for (int i = 0; i < 2; i++) {
        DrawBack();
    }
    SDL_Texture* tex;
    SDL_Rect dest;
    dest.w=MySDL::GAME_WINDOW_WIDTH/3;
    dest.h=MySDL::GAME_WINDOW_HEIGHT/4;
    dest.y=MySDL::GAME_WINDOW_HEIGHT/2-dest.h/2;
    dest.x=MySDL::GAME_WINDOW_WIDTH/2-dest.w/2;
    tex=RenderText("Loading...",{200,100,100,0});
    DrawPic(dest,tex);
    SDL_RenderPresent(rend);
}