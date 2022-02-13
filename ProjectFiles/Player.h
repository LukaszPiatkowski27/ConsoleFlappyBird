#pragma once

#include "ConsoleClass.h"

class Player
{
    public:

    Vector2 pos;
    Vector2 oldPos;
    VectorRGB col;
    Vector2 velocity;
    float mass;
    float flapForce;

    Player(Vector2 _pos, float _mass, float _flapF, VectorRGB _col) : pos { _pos },
                                                                      col { _col },
                                                                      mass { _mass },
                                                                      flapForce { _flapF }{
        this->velocity.x = 0;
        this->velocity.y = 0;
    }
    ~Player() {}

    void draw(Canvas &canvas){
        this->oldPos = this->pos;
        canvas.pixels[(int)this->pos.x + (int)this->pos.y * canvas.canSize.X].color = RGB(this->col.R, this->col.G, this->col.B);
        canvas.update();
    }
/**
    void move(char x, char y){
        pos.x += x;
        pos.y += y;
        if(pos.x < 0){
            pos.x = 0;
        }else if(pos.x > 31){
            pos.x = 31;
        }
        if(pos.y < 0){
            pos.y = 0;
        }else if(pos.y > 31){
            pos.y = 31;
        }
    }
**/

    void flap(){
        this->velocity.y = this->flapForce * -1;
    }

    void move(float gravity){
        //this->pos.x += this->velocity.x;
        this->pos.y += this->velocity.y;
        this->velocity.y += this->mass * gravity;
        if(pos.x < 0){
            pos.x = 0;
        }else if(pos.x > 31){
            pos.x = 31;
        }
        if(pos.y < 0){
            pos.y = 0;
        }else if(pos.y > 31){
            pos.y = 31;
        }
    }

    void updatePosOnCanvas(Canvas &canvas, vector<VectorRGB> active_g){
        if((int)this->oldPos.y != (int)this->pos.y){
            short temp = (int)this->oldPos.x + (canvas.canSize.Y - (int)this->oldPos.y - 1) * canvas.canSize.X;
            canvas.pixels[(int)this->oldPos.x + (int)this->oldPos.y * canvas.canSize.X].color = RGB(active_g[temp].R, active_g[temp].G, active_g[temp].B);
            canvas.update();
            this->draw(canvas);
        }
    }

};
