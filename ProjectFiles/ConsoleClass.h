#pragma once

#include<windows.h>
#include<conio.h>
#include<vector>

using std::vector;

class Vector2{

    public:

    float x,y;

    Vector2() : x( 0 ), y( 0 ){

    }

    Vector2(float _x, float _y) : x( _x ), y( _y ){

    }

    ~Vector2(){};
};

class ConsoleController{

    private:
    //wyœwietlacz
    HWND display;
    RECT dRect;

    public:
    //konsola
    HWND & window;
    HDC dc;
    RECT cRect;
    COORD cSize;
    COORD cPos;

    ConsoleController(HWND & consoleWindow) : window( consoleWindow ),
                                              display( GetDesktopWindow() )
    {
        cPos.X = 0;
        cPos.Y = 0;
        dc = GetDC(window);
        GetWindowRect(window, &cRect);
        GetWindowRect(display, &dRect);
        cSize.X = cRect.right - cRect.left;
        cSize.Y = cRect.bottom - cRect.top;
        updateWindow();
    }

    ~ConsoleController(){};

    void setWindowPosition(Vector2 & _pos){
        cPos.X = _pos.x;
        cPos.Y = _pos.y;
        updateWindow();
    }

    void moveWindow(Vector2 & _diff){
        cPos.X += _diff.x;
        cPos.Y += _diff.y;
        updateWindow();
    }

    void centerWindow(){
        cPos.X = dRect.right / 2 - cSize.X / 2;
        cPos.Y = dRect.bottom / 2 - cSize.Y / 2;
        updateWindow();
    }

    void setWindowSize(Vector2 & _size){
        cSize.X = _size.x;
        cSize.Y = _size.y;
        updateWindow();
    }

    void updateWindow(){
        MoveWindow(window, cPos.X, cPos.Y, cSize.X, cSize.Y, true);
        GetWindowRect(window, &cRect);
    }

};

class Pixel{

    private:

    short pSize;

    public:

    COLORREF color;
    bool state;

    COORD pPos;
    COORD localPos;
    HWND window;
    HDC dc;

    Pixel(COORD _pos, COORD _lpos, short & _size, HWND & _window) : pSize( _size ),
                                                  pPos( _pos ),
                                                  localPos( _lpos ),
                                                  state( false ),
                                                  window( _window )
    {
        color = RGB(0,0,0);
        dc = GetWindowDC(window);
    }

    ~Pixel(){};

    void draw(){
        if(state){
            for(int y = pPos.Y; y < pPos.Y + pSize; y++){
                for(int x = pPos.X; x < pPos.X + pSize; x++){
                    SetPixel(dc, x, y, color);
                }
            }
        }
    }
};

class Canvas{
    public:
    vector<Pixel> pixels;
    vector<Pixel> savedPix;
    COORD & canSize;
    short & pSize;
    HWND window;
    Canvas(COORD & _canSize, short & _pSize, HWND & _window) : canSize( _canSize ),
                                                               pSize( _pSize ),
                                                               window( _window )
    {
        createCanvas();
    }

    ~Canvas(){
        clearCanvas();
    }

    void createCanvas(){
        COORD tempPos;
        COORD tempLocalPos;
        Pixel * tempPix;
        for(int y = 0; y < canSize.Y; y++){
            for(int x = 0; x < canSize.X; x++){
                tempPos.X = x * pSize + 40;
                tempPos.Y = y * pSize + 80;
                tempLocalPos.X = x;
                tempLocalPos.Y = y;
                tempPix = new Pixel(tempPos, tempLocalPos, pSize, window);
                pixels.push_back(*tempPix);
                delete tempPix;
            }
        }
    }

    void clearCanvas(){
        pixels.clear();
        savedPix.clear();
    }

    void update(){
        if(savedPix.size()==0){
            for(int i = 0; i < pixels.size(); i++){
                pixels[i].draw();
                savedPix.push_back(pixels[i]);
            }
        }
        else
        {
            for(int i = 0; i < pixels.size(); i++){
                if(pixels[i].color != savedPix[i].color){
                    pixels[i].draw();
                    savedPix[i]=pixels[i];
                }
            }
        }
    }
};

void gotoXY(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
