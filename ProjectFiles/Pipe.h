#pragma once

#include "ConsoleClass.h"
#include <time.h>
#include <stdlib.h>

class Pipe {
    public:

    Vector2 pos;
    Vector2 oldPos;
    VectorRGB col;
    int gape;
    bool passed = false;

    Pipe(Vector2 _pos, int _gape, VectorRGB _col) : pos { _pos },
                                                    gape { _gape },
                                                    col { _col }{
    }

    ~Pipe(){}

    void draw(Canvas &canvas){
        this->oldPos = this->pos;
        for(int i = (int)this->pos.y; i >= 0; i--){
            canvas.pixels[(int)this->pos.x + i * canvas.canSize.X].color = RGB(this->col.R, this->col.G, this->col.B);
        }
        for(int i = 31; i >= (int)this->pos.y + this->gape; i--){
            canvas.pixels[(int)this->pos.x + i * canvas.canSize.X].color = RGB(this->col.R, this->col.G, this->col.B);
        }
        canvas.update();
    }

    void move(float speed){
        this->pos.x -= speed;
        if(this->pos.x < 0){
            this->pos.x = 31.9;
            this->randomize();
        }
    }

    void updatePosOnCanvas(Canvas &canvas, vector<VectorRGB> active_g){
        if((int)this->oldPos.x != (int)this->pos.x){
            short temp;
            for(int i = (int)this->oldPos.y; i >= 0; i--){
                temp = (int)this->oldPos.x + (canvas.canSize.Y - i - 1) * canvas.canSize.X;
                canvas.pixels[(int)this->oldPos.x + i * canvas.canSize.X].color = RGB(active_g[temp].R, active_g[temp].G, active_g[temp].B);
            }
            for(int i = 31; i >= (int)this->oldPos.y + this->gape; i--){
                temp = (int)this->oldPos.x + (canvas.canSize.Y - i - 1) * canvas.canSize.X;
                canvas.pixels[(int)this->oldPos.x + i * canvas.canSize.X].color = RGB(active_g[temp].R, active_g[temp].G, active_g[temp].B);
            }
            canvas.update();
            this->draw(canvas);
        }
        if(this->pos.x > 31){
            for(int i = 0; i < 32; i++){
                short temp = (canvas.canSize.Y - i - 1) * canvas.canSize.X;
                canvas.pixels[i * canvas.canSize.X].color = RGB(active_g[temp].R, active_g[temp].G, active_g[temp].B);
            }
        }
    }

    void randomize(){
        srand(time(NULL));
        this->gape = rand() % 5 + 10;
        this->pos.y = rand() % (31-this->gape) + 1;
        this->passed = false;
    }

    bool checkPassed(Player &player){
        if(!this->passed){
            if(this->pos.x < player.pos.x){
                this->passed = true;
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
};
