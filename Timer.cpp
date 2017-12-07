#include"Classes.h"

Timer::Timer(){
    dest.x=20;
    dest.y=20;
    dest.h=50;
    dest.w=25;
}

int Timer::getTime(){
    if(timepaused){
        return pausedtime/1000;
    }
    else{
        return (SDL_GetTicks()-starttime)/1000;
    }
}

void Timer::reset(){
    timepaused=false;
    starttime=SDL_GetTicks();
}
SDL_Rect Timer::getRect(){
    return dest;
}

bool Timer::paused(){
    return timepaused;
}

void Timer::pause(){
    if(!timepaused){
        pausedtime=SDL_GetTicks()-starttime;
        timepaused=true;
    }
}

void Timer::unpause(){
    if(timepaused){
        starttime=SDL_GetTicks()-pausedtime;
        timepaused=false;
    }
}