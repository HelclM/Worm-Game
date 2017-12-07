#include"Classes.h"

bool Input::moveLeft(){
    std::lock_guard<mutex> guard(mu);
    return (goLeft && !goRight);
}

bool Input::moveRight(){
    std::lock_guard<mutex> guard(mu);
    return (goRight && !goLeft);
}

bool Input::wormFiring(){
    std::lock_guard<mutex> guard(mu);
    return firing;
}

bool Input::queueEmpty(){
    return q.empty();
}

coord Input::getMouse(){
    std::lock_guard<mutex> guard(mu);
    return mouse;
}

char Input::takeFromQueue(){
    return q.takeFromBack();
}

void Input::eraseEvents(){
    SDL_PumpEvents();
    SDL_FlushEvent(SDL_KEYDOWN);
    SDL_FlushEvent(SDL_KEYUP);
    SDL_FlushEvent(SDL_MOUSEBUTTONUP);
    q.eraseAll();
    std::lock_guard<mutex> guard(mu);
    goRight=false;
    goLeft=false;
    firing=false;
    mouse.x=MySDL::GAME_WINDOW_WIDTH/2;
}

void Input::getEvents(){
    std::unique_lock<mutex> locker(mu);
    locker.unlock();
    state=SDL_GetKeyboardState(NULL);
    SDL_Event event;
    if(Control::pausedGame()){
        eraseEvents();
    }
    else{
        while(SDL_PollEvent(&event)==1){
            switch(event.type){
                case SDL_QUIT:
                    Control::exitGame=true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    locker.lock();
                    firing=false;
                    locker.unlock();
                    if(event.button.button==SDL_BUTTON_LEFT && !state[SDL_SCANCODE_ESCAPE] 
                            && !Control::animating()){
                        q.pushFront('f'); 
                    }
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym==SDLK_UP || event.key.keysym.sym==SDLK_w){
                        q.pushFront('j');
                    }
                    else if(event.key.keysym.sym==SDLK_q && !Control::animating()){
                        q.pushFront('c');
                    }
                    else if(event.key.keysym.sym==SDLK_SPACE){
                        q.pushFront('e');
                    }
                    else if(event.key.keysym.sym==SDLK_1 && !Control::animating()){
                        q.pushFront('1');
                    }
                    else if(event.key.keysym.sym==SDLK_2 && !Control::animating()){
                        q.pushFront('2');
                    }
                    else if(event.key.keysym.sym==SDLK_3 && !Control::animating()){
                        q.pushFront('3');
                    }
                    else if(event.key.keysym.sym==SDLK_4 && !Control::animating()){
                        q.pushFront('4');
                    }
                    else if(event.key.keysym.sym==SDLK_5 && !Control::animating()){
                        q.pushFront('5');
                    }
                    break;
            }
        }
        locker.lock();
        if(SDL_GetMouseState(&mouse.x,&mouse.y) & SDL_BUTTON(SDL_BUTTON_LEFT)){
            firing=true;
        }
        locker.unlock();
        if(state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]){
            locker.lock();
            goLeft=true;
            locker.unlock();
        }
        else{
            locker.lock();
            goLeft=false;
            locker.unlock();
        }
        if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){
            locker.lock();
            goRight=true;
            locker.unlock();
        }
        else{
            locker.lock();
            goRight=false;
            locker.unlock();
        }
    }
}