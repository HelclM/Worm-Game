#include <cstdlib>
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include<cmath>
#include<iostream>
#include<SDL_ttf.h>
#include<sstream>
#include<vector>
#include<thread>
#include<mutex>
#include<deque>
#include<condition_variable>

using namespace std;

class Weapon;
class MySDL;
class Control;
class Worm;
class Timer;

struct coord{
    int x;
    int y;
};

struct explod{
    bool exp;
    int rad;
    int power;
    int counter=0;
    int pressure;
    int divide;
};

struct bullet{
    int x;
    int y;
    int w;
    int h;
    float vx;
    float vy;
    double angle;
    SDL_Texture* bulltex=NULL;
    SDL_RendererFlip flip;
    bool flying;
};

class Texture{
private:
    int** map;
    int sizex;
    int sizey;
public:
    Texture(const char* itxt,int isizex,int isizey);
    ~Texture();
    void writeWorm(Worm* w);
    void eraseWorm(Worm* w);
    int valueat(int x,int y);
    void writePoint(int x, int y, int value);
};

class Timer{
private:
    int starttime=0;
    int pausedtime;
    bool timepaused=false;
    SDL_Rect dest;
public:
    Timer();
    SDL_Rect getRect();
    int getTime();
    void pause();
    void unpause();
    bool paused();
    void reset();
};
 
class Weapon{
protected:
    double lx;
    double ly;
    int windCounter=0;
    int windProof;
public:
    SDL_Texture* explode=NULL;
    SDL_Texture* weaptex=NULL;
    SDL_Texture* weapChargesTex=NULL;
    SDL_Rect destexp;
    SDL_Rect destweap;
    bullet projectile;
    explod exploded;
    int maxspeed;
    int charges;
    int lastCharges=0;
    coord v0;
    SDL_RendererFlip flip=SDL_FLIP_NONE;
    double angle=0;
    virtual coord getStrikeCoord()=0;
    virtual void Shoot(Texture* tex, int wind)=0;
    virtual void strike(int x,int y)=0;
    virtual ~Weapon(){}
};

class Rocket: public Weapon{
protected:
    bool firstShoot=true;
public:
    Rocket();
    ~Rocket();
    coord getStrikeCoord();
    void Shoot(Texture* tex, int wind);   
    void strike(int x,int y);
};

class Granade:public Weapon{
protected:
    bool firstShoot=true;
    Timer timer;
public:
    Granade();
    ~Granade();
    coord getStrikeCoord();
    void Shoot(Texture* tex, int wind);   
    void strike(int x,int y);
};

class Dynamite:public Weapon{
protected:
    bool firstShoot=true;
    Timer timer;
public:
    Dynamite();
    ~Dynamite();
    coord getStrikeCoord();
    void Shoot(Texture* tex, int wind);   
    void strike(int x,int y);
};

class BSBat:public Weapon{
public:
    BSBat();
    ~BSBat();
    coord getStrikeCoord();
    void Shoot(Texture* tex, int wind);   
    void strike(int x,int y);
};

class Shotgun:public Weapon{
protected:
    bool firstShoot=true;
public:
    Shotgun();
    ~Shotgun();
    coord getStrikeCoord();
    void Shoot(Texture* tex, int wind);   
    void strike(int x,int y);
};

class Worm{
public:
    SDL_Rect dest;
    int team;
    int dir;//0 left 1 right
    int lasthealth;
    int health;
    bool firing=false;
    bool jumping=false;
    bool firstjump=true;
    int startJumping;
    bool playing=false;
    bool firstRound=false;
    bool flying=false;
    int fallspeed=0;
    int firstFly=1;
    coord flyspeed;
    vector<Weapon*> weapons;
    static int wormCounter; 
    int id;
    SDL_Texture* tex=NULL;
    SDL_Texture* texttex=NULL;
    Weapon* weap=NULL;
    Worm(int iteam, int teamsize);
    ~Worm();
    void moveWorm(int ix,int iy);
    void nextWeapon();
    void changeWeapon(int i);
};

class MySDL{
private:
    SDL_Texture* backtex=NULL;
    SDL_Texture* texarrow=NULL;
    SDL_Texture* textime=NULL;
    SDL_Window* window=NULL;
    TTF_Font* font=NULL;
    Control* c;
    Texture* tex;
    int counter=0;
    SDL_Rect srcdest;
    void resizeTextDest(SDL_Rect* dest, string text,int letterW,int letterH);
    void DrawTime();
    void drawAnnoucement(string text, SDL_Color color);
    void DrawBack();
    void DrawWind();
    void DrawWorm(Worm * w);
    void DrawProjectile(bullet proj );
    void DrawWeapons();
    void DrawLine(int x,int y,int wx, int wy);
    void DrawPic(SDL_Rect destpic, SDL_Texture* tex,double angle=0,
        SDL_RendererFlip flip=SDL_FLIP_NONE,SDL_Rect* srcdest=NULL);
    SDL_Texture* RenderText(string text, SDL_Color textcolor);
    void createBackTexture();
    void setCamera();
    void centerCameraAround(int x);
public:
    void DrawLoadingScreen();
    static int GAME_WINDOW_WIDTH;
    static int GAME_WINDOW_HEIGHT;
    static int GAME_MAP_WIDTH;
    MySDL();
    void setControl(Control* ic);
    void setTextureRef(Texture* itex);
    static SDL_Renderer* rend;//game window renderer
    void Init();
    void CreateWinRen(const char* name);
    void drawGame();
    void DestroyWin();
    SDL_Rect* getSrcdestRef();
};

class queue{
private:
    std::mutex mu;
    std::deque<char> q;
    std::condition_variable cond;
public:
    void eraseAll();
    bool empty();
    void pushFront(char a);
    char takeFromBack();
    void notifyAll();
}; 

class Input{
private:
    std::mutex mu;
    const Uint8* state;
    queue q;
    bool goRight=false;
    bool goLeft=false;
    bool firing=false;
    coord mouse;
    void eraseEvents();
public:
    char takeFromQueue();
    bool queueEmpty();
    bool moveRight();
    bool moveLeft();
    bool wormFiring();
    coord getMouse();
    void getEvents();
};

class Control{
private:
    static mutex mu;
    static int speed;
    static int up;
    static int down;
    static int checkh;
    static int jumpx;
    static int jumpy;
    static bool shotFlying;
    static bool wormFlying;
    static bool gamePaused;
    bool goToNextRound=false;
    bool goLeft=false;
    bool goRight=false;
    int lastworm1;
    int lastworm2;
    int lastteamplayed;
    int counterChangeWeap=0;
    int pauseCounter=0;
    SDL_Rect* srcdest;
    void wormFall(Worm* w);
    void Gravity();
    int handleTeamFlying(vector<Worm*> team);
    void handleWormsFlying();
    void handleExplosion();
    void eraseDeadWorms();
    void nextRound();
    void GoLeft();
    void GoRight();
    void Jump(Worm* w);
    void eraseWormWeap(Worm* w);
    void removeWormsEmptyWeapons();
    coord FireSpeed(int x, int y);
    void WormShoot(coord v0);
    bool CanFly(int first, SDL_Rect dest, int dir);
    void setWormFlySpeed(Worm* w);
    void handleWormsJumping();
    int fallDamage(double vx, double vy);
    int explosionDamage(double power,double rad,double distance);
    Input* input;
public:
    coord mouse;
    int wind=0;
    static bool exitGame;
    static bool delay;
    static bool explosion;
    bool backChanged=true;
    vector<Worm*> team1;
    vector<Worm*> team2;
    Texture* rectex;
    Worm* recworm;
    Timer timer;
    static bool animating();
    static bool pausedGame();
    double distance(coord a,coord b);
    void createTeams(int teamSize);
    void clearTeams();
    Control(Texture* tex);
    coord getMouseCoord();
    void handleEvents();
    void logic();
    void setSrcdestRef(SDL_Rect* dest);
    void setInputRef(Input* i);
};
