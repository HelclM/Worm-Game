#include"Classes.h"

using namespace std;

void gameLoop(Control* control, MySDL* win1);

int main() {
    int teamSize=5;
    srand (time(NULL));
    Texture tex("./map_txt/map1_out.txt",MySDL::GAME_MAP_WIDTH,MySDL::GAME_WINDOW_HEIGHT);
    Input input;
    Control control(&tex);
    control.setInputRef(&input);
    MySDL win1;
    win1.setControl(&control);
    win1.Init();
    win1.setTextureRef(&tex);
    win1.CreateWinRen("worms");
    win1.DrawLoadingScreen();
    control.setSrcdestRef(win1.getSrcdestRef());
    control.createTeams(teamSize);
    thread mainGameLoop(gameLoop,&control,&win1);
    //inputs
    while(!Control::exitGame){
        input.getEvents();
        SDL_Delay(1000/480);
    }
//    cout<<"end"<<endl;
    mainGameLoop.join();
    control.clearTeams();
    win1.DestroyWin();
    return 0;
}

void gameLoop(Control* control, MySDL* win1){
    while(!Control::exitGame){
       // cout<<"animating: "<<Control::animating()<<endl;
        control->handleEvents();
        control->logic();
        win1->drawGame();
        SDL_Delay(2000/60);
        if (Control::delay){
            SDL_Delay(2000);
        }
    }
}
