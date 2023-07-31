#include "main.h"
#include "structs.h"

int initGame();
void clearEntitySystem(struct Entity **head);

int initGame(){
    SDL_Surface* s;

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_VIDEO) != 0){
        printf("Error while Initializing SDL %s",SDL_GetError());
        return 1;
    }

    if(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) == 0){
        printf("Error while Initializing SDL_Image %s",SDL_GetError());
        SDL_Quit();
        return 1;
    }


    app.w = SDL_CreateWindow("Shoot Them Up!!",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,0);

    if(!app.w){
        printf("Error while creating window %s",SDL_GetError());
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    app.r = SDL_CreateRenderer(app.w,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!app.r){
        printf("Error while creating renderer %s",SDL_GetError());
        SDL_DestroyWindow(app.w);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    bullethead = 0;
    enemyhead = 0;

    s= IMG_Load("assets/player_ship.png");

    if(!s){
        printf("Error while creating player surface %s",SDL_GetError());
        SDL_DestroyWindow(app.w);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    
    player.health = 100;
    player.reloadRate = 10;
    player.t = SDL_CreateTextureFromSurface(app.r,s);
    SDL_FreeSurface(s);

    
    if(!player.t){
        printf("Error while creating player texture %s",SDL_GetError());
        SDL_DestroyWindow(app.w);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }
    SDL_QueryTexture(player.t,NULL,NULL,&player.box.w,&player.box.h);
    player.box.x = SCREEN_WIDTH/2 - player.box.w/2;
    player.box.y = SCREEN_HEIGHT - player.box.h;   
    player.side = Type_Player;

    s= IMG_Load("assets/player_laser.png");
    if(!s){
        printf("Error while creating bullet surface %s",SDL_GetError());
        SDL_DestroyWindow(app.w);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    bullet.box.x = 0;
    bullet.box.y = 0;
    bullet.health = 0;
    bullet.t = SDL_CreateTextureFromSurface(app.r,s);
    SDL_FreeSurface(s);

    if(!bullet.t){
        printf("Error while creating bullet texture %s",SDL_GetError());
        SDL_DestroyWindow(app.w);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    SDL_QueryTexture(bullet.t,NULL,NULL,&bullet.box.w,&bullet.box.h);
    
    s= IMG_Load("assets/Background.jpg");
    app.background = SDL_CreateTextureFromSurface(app.r,s);
    SDL_FreeSurface(s);
    if(!app.background){
        printf("Error while creating background texture %s",SDL_GetError());
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    TTF_Init();
    app.font = TTF_OpenFont("Fonts/Antonio-Regular.ttf",30);
    if(!app.font){
        printf("Error while creating font %s",SDL_GetError());
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    app.score = 0;
    return 0;

}

void ResetGame(){
    player.health = 100;
    player.dx = 0;
    player.dy = 0;
    player.next = NULL;
    player.reloadRate  = 10;

    app.score = 0;

    printf("Clearing enemies..\n");
    clearEntitySystem(&enemyhead);
    if(!enemyhead) printf("Enemy cleared");
    printf("Clearing bullets..\n");
    clearEntitySystem(&bullethead);
    printf("Clearing asteroids..\n");
    clearEntitySystem(&astHead);

}

void clearEntitySystem(struct Entity **head){

    int count = 0;
    struct Entity *curr;
    struct Entity *next;
    next = 0;

    curr = *head;
    while(curr){
        count++;
        printf("Clearing entity %d\n",count);
        next = curr->next;
        memset(curr,0,sizeof(curr));
        free(curr);
        curr = 0;
        curr = next;
    }

   *head = NULL;

}