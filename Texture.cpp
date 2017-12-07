#include"Classes.h"

using namespace std;

Texture::Texture(const char* itxt, int isizex, int isizey) {
    sizex = isizex;
    sizey = isizey;
    map = new int*[sizex];
    for (int i = 0; i < sizex; i++) {
        map[i] = new int[sizey];
    }
    FILE *file;
    file = fopen(itxt, "r");
    for (int k = 0; k < sizey; k++) {
        for (int l = 0; l < sizex; l++) {
            fscanf(file, "%1d", &map[l][k]);
        }
    }
    fclose(file);
    //create water
    for (int i = sizey-100; i < sizey; i++) {
        for (int j = 0; j < sizex; j++){
            map[j][i]=-1;
        }
    }

}

Texture::~Texture() {
    for (int i = 0; i < sizex; i++) {
        delete [] map[i];
    }
    delete[] map;
}

int Texture::valueat(int x, int y) {
    if (x > 0 && x < sizex && y > 0 && y < sizey) {
        return map[x][y];
    }
    else if (x > 0 && x < sizex && y<=0){
        return 0;
    }
    else if(x > 0 && x < sizex && y>=MySDL::GAME_WINDOW_HEIGHT){
        return -1;
    }
    else{
        return -1;
    }
}

void Texture::writeWorm(Worm* w){
    for (int i = w->dest.x; i < w->dest.x+w->dest.w; i++) {
        for (int j = w->dest.y; j < w->dest.y+w->dest.h; j++) {
            if(i>0 && i<sizex && j>0 && j<sizey && map[i][j]==0){
                map[i][j]=2;
            }
        }
    }
}

void Texture::eraseWorm(Worm* w){
    for (int i = w->dest.x; i < w->dest.x+w->dest.w; i++) {
        for (int j = w->dest.y; j < w->dest.y+w->dest.h; j++) {
            if(i>0 && i<sizex && j>0 && j<sizey && abs(map[i][j])!=1){
                map[i][j]=0;
            }
        }
    }
}

void Texture::writePoint(int x, int y, int value){
    if(x>0 && x<sizex && y>0 && y<sizey){
        map[x][y]=value;
    }
}