#define _WIN32_WINNT 0x0500

#include"Bitmap.h"
#include"player.h"
#include"Pipe.h"
#include<io.h>
#include<mmsystem.h>
#include<w32api.h>
#include<string>

ConsoleController * cc;
Canvas * canvas;
COORD canSize;

LPSTR sMenu_m = "sounds/test.wav";

Player * player;
Pipe * pipe1;
Pipe * pipe2;

vector<VectorRGB> gameOver_g;
vector<VectorRGB> cyberPunkMenu_g;
vector<VectorRGB> mainMenu1_g;
vector<VectorRGB> mainMenu2_g;
vector<VectorRGB> mainMenu3_g;
vector<VectorRGB> mainMenuPL1_g;
vector<VectorRGB> mainMenuPL2_g;
vector<VectorRGB> mainMenuPL3_g;
vector<VectorRGB> options1_g;
vector<VectorRGB> options2_g;
vector<VectorRGB> optionsPL1_g;
vector<VectorRGB> optionsPL2_g;
vector<VectorRGB> lvl1_g;

vector<VectorRGB> active_g;

void loadGraphics(vector<VectorRGB> vec, bool _update);
void sMenu();
void mMenu();
void loadHeader();
void clearHeader();
void prepareGraphics();
void startGame();
void options();
void credit();
void setVolume(float _volumeLevel);
void gameOver();
void loadVolume();

bool collision(Player &player, Pipe &pipe);
bool startMenu_b = false;
bool mainMenu_b = false;
bool langPL = false;

char input;
char menuItem{};

float volumeLevel = 15;

int score = 0;

int main(){
    _setmode(_fileno(stdout), 0x00020000);
    canSize.X = 32;
    canSize.Y = 32;
    short pixSize = 10;
    HWND myConsole = GetConsoleWindow();
    cc = new ConsoleController(myConsole);
    //console size
    Vector2 * cs = new Vector2(400,420);
    cc->setWindowSize(*cs);
    cc->centerWindow();

    canvas = new Canvas(canSize,pixSize,myConsole);

    for(int i = 0; i < canvas->pixels.size(); i++){
        canvas->pixels[i].state = true;
    }

    prepareGraphics();

    PlaySound(sMenu_m, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    setVolume(volumeLevel);

    sMenu();


    ReleaseDC(myConsole,cc->dc);

    delete cc;
    delete cs;
    delete canvas;

    return 0;
}

void loadGraphics(vector<VectorRGB> vec, bool _update){
    if(vec.size()<canvas->pixels.size()){
        //Invalid size of bmp file
        return;
    }
    for(short y = canSize.Y-1; y >= 0; y--){
        for(short x = 0; x < canSize.X; x++){
            canvas->pixels[x + y*canSize.X].color = RGB(vec[x + (canSize.Y-y-1)*canSize.X].R,vec[x + (canSize.Y-y-1)*canSize.X].G,vec[x + (canSize.Y-y-1)*canSize.X].B);
        }
    }
    if(_update)
    canvas->update();

    active_g = vec;
}

void sMenu(){
    if(!startMenu_b){
        startMenu_b = true;
        loadGraphics(cyberPunkMenu_g,true);
        if(langPL){
            gotoXY(10,1);
            std::wcout<<"Naciœnij ENTER aby rozpocz¹æ!";
            gotoXY(0,0);
        }
        else
        {
            gotoXY(14,1);
            std::wcout<<"Press ENTER to begin!";
            gotoXY(0,0);
        }
        while(1){
            input = getch();
            if(input == 13){
                clearHeader();
                startMenu_b = false;
                mMenu();
                break;
            }
        }
    }
}

void mMenu(){
    clearHeader();
    if(langPL){
        loadGraphics(mainMenuPL1_g,true);
        gotoXY(5,1);
        std::wcout<<"U¿yj ("<<L"\x25C4"<<" "<<L"\x25BA"<<"), aby poruszaæ siê po menu!";
        gotoXY(0,0);
    }
    else
    {
        loadGraphics(mainMenu1_g,true);
        gotoXY(7,1);
        std::wcout<<"Use ("<<L"\x25C4"<<" "<<L"\x25BA"<<"), to change menu option!";
        gotoXY(0,0);
    }
    menuItem = 0;
    while(1){
        input = getch();
        if(input == 13){
            switch(menuItem)
            {
            case 0 :
                startGame();
                break;
            case 1 :
                options();
                break;
            case 2 :
                break;
            }
            break;
        }
        if(input == 75 && menuItem > 0){
            menuItem--;
            if(menuItem == 0){
                langPL ? loadGraphics(mainMenuPL1_g,true) : loadGraphics(mainMenu1_g,true);
            }
            else
            {
                langPL ? loadGraphics(mainMenuPL2_g,true) : loadGraphics(mainMenu2_g,true);
            }
        }
        if(input == 77 && menuItem < 2){
            menuItem++;
            if(menuItem == 1){
                langPL ? loadGraphics(mainMenuPL2_g,true) : loadGraphics(mainMenu2_g,true);
            }
            else
            {
                langPL ? loadGraphics(mainMenuPL3_g,true) : loadGraphics(mainMenu3_g,true);
            }
        }
    }
}

void loadHeader(char mode){
    if(mode == 0){
        gotoXY(4,0);
        std::wcout<<L"\x2554";
        for(int i = 0; i < 38; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x2557";
        gotoXY(4,1);
        std::wcout<<L"\x2551";
        gotoXY(43,1);
        std::wcout<<L"\x2551";
        gotoXY(4,2);
        std::wcout<<L"\x255A";
        for(int i = 0; i < 38; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x255D";
        gotoXY(0,0);
    }else if(mode == 1){
        gotoXY(4,0);
        std::wcout<<L"\x2554";
        for(int i = 0; i < 32; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x2566";
        for(int i = 0; i < 5; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x2557";
        gotoXY(4,1);
        std::wcout<<L"\x2551";
        gotoXY(37,1);
        std::wcout<<L"\x2551";
        gotoXY(43,1);
        std::wcout<<L"\x2551";
        gotoXY(4,2);
        std::wcout<<L"\x255A";
        for(int i = 0; i < 32; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x2569";
        for(int i = 0; i < 5; i++){
            std::wcout<<L"\x2550";
        }
        std::wcout<<L"\x255D";
        gotoXY(0,0);
    }

}

void clearHeader(){
    gotoXY(5,1);
    for(int i = 0; i < 38; i++){
        std::wcout<<" ";
    }
    gotoXY(0,0);
}

void startGame(){
    score = 0;
    Vector2 startPos(5,9);
    VectorRGB playerColor(255,0,0);
    player = new Player(startPos,1,1,playerColor);
    pipe1 = new Pipe(Vector2(10,10),13,VectorRGB(0,255,0));
    pipe2 = new Pipe(Vector2(26,15),10,VectorRGB(0,255,0));
    clearHeader();
    loadHeader(1);
    if(langPL){
        gotoXY(5,1);
        std::wcout<<"U¿yj SPACJI, aby polecieæ wy¿ej!";
    }else{
        gotoXY(6,1);
        std::wcout<<"Use SPACEBAR, to fly higher!";
    }
    gotoXY(0,0);
    loadGraphics(lvl1_g,true);
    player->draw(*canvas);
    pipe1->draw(*canvas);
    pipe2->draw(*canvas);

    bool spacePressed = false;

    while(1){
        if(GetAsyncKeyState(VK_SPACE) & 0x8000){
            if(!spacePressed){
                player->flap();
                spacePressed = true;
            }
        }else{
            spacePressed = false;
        }
        player->move(0.1);
        pipe1->move(0.3);
        pipe2->move(0.3);
        player->updatePosOnCanvas(*canvas,active_g);
        pipe1->updatePosOnCanvas(*canvas,active_g);
        pipe2->updatePosOnCanvas(*canvas,active_g);
        if(collision(*player,*pipe1) || collision(*player,*pipe2) || (int)player->pos.y == 31){
            gameOver();
            break;
        }
        if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){
            gameOver();
            break;
        }
        if(pipe1->checkPassed(*player) || pipe2->checkPassed(*player)){
            score++;
        }
        gotoXY(40,1);
        std::wcout<<score;
        gotoXY(0,0);
        Sleep(60);
    }
}

void options(){
    langPL ? loadGraphics(optionsPL1_g,true) : loadGraphics(options1_g,true);
    //za³aduj g³oœnoœæ
    loadVolume();
    short tempOpt = 0;
    while(1){
        input = getch();
        if(input == 75 && tempOpt > 0){
            tempOpt--;
            langPL ? loadGraphics(optionsPL1_g,true) : loadGraphics(options1_g,true);
            //za³aduj g³oœnoœæ
            loadVolume();
        }
        if(input == 77 && tempOpt < 1){
            tempOpt++;
            langPL ? loadGraphics(optionsPL2_g,true) : loadGraphics(options2_g,true);
            //za³aduj g³oœnoœæ
            loadVolume();
        }
        if(input == 72 && tempOpt == 0 && volumeLevel < 16){
            volumeLevel++;
            setVolume(volumeLevel);
            //za³aduj g³oœnoœæ
            loadVolume();
        }
        if(input == 80 && tempOpt == 0 && volumeLevel > 0){
            volumeLevel--;
            setVolume(volumeLevel);
            //za³aduj g³oœnoœæ
            loadVolume();
        }
        if(input == 13 && tempOpt == 1){
            if(langPL)langPL = false;
            else langPL = true;
            langPL ? loadGraphics(optionsPL2_g,true) : loadGraphics(options2_g,true);
            loadVolume();
        }
        if(input == 27){
            mMenu();
            break;
        }
    }
}

void gameOver(){
    delete player;
    delete pipe1;
    delete pipe2;
    loadGraphics(gameOver_g,true);
    loadHeader(0);
    clearHeader();
    if(langPL){
        gotoXY(8,1);
        std::wcout<<"Spróbuj ponownie (Y/N)";
    }else{
        gotoXY(11,1);
        std::wcout<<"Try again (Y/N)";
    }
    while(1){
        input = getch();
        if(input == 'y'){
            startGame();
            break;
        }else if(input == 'n'){
            mMenu();
            break;
        }
    }
}

void prepareGraphics(){
    ReadBMP("graphics/test.bmp",gameOver_g);
    ReadBMP("graphics/flappy.bmp",cyberPunkMenu_g);
    ReadBMP("graphics/menuEN1.bmp",mainMenu1_g);
    ReadBMP("graphics/menuEN2.bmp",mainMenu2_g);
    ReadBMP("graphics/menuEN3.bmp",mainMenu3_g);
    ReadBMP("graphics/menuPL1.bmp",mainMenuPL1_g);
    ReadBMP("graphics/menuPL2.bmp",mainMenuPL2_g);
    ReadBMP("graphics/menuPL3.bmp",mainMenuPL3_g);
    ReadBMP("graphics/optionsEN1.bmp",options1_g);
    ReadBMP("graphics/optionsEN2.bmp",options2_g);
    ReadBMP("graphics/optionsPL1.bmp",optionsPL1_g);
    ReadBMP("graphics/optionsPL2.bmp",optionsPL2_g);
    ReadBMP("graphics/blank.bmp",lvl1_g);
    loadHeader(0);
}

void setVolume(float _volumeLevel){
    waveOutSetVolume(NULL,_volumeLevel * 4095);
}

bool collision(Player &player, Pipe &pipe){
    if((int)player.pos.x == (int)pipe.pos.x){
        if((int)player.pos.y <= (int)pipe.pos.y || (int)player.pos.y >= (int)pipe.pos.y + pipe.gape){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

void loadVolume(){
    loadGraphics(active_g,true);
    for(int i = 0; i < volumeLevel; i++){
        canvas->pixels[(12 + i) + 11 * canvas->canSize.X].color = RGB(0,255,0);
        canvas->pixels[(12 + i) + 12 * canvas->canSize.X].color = RGB(0,255,0);
    }
    canvas->update();
}
