#include "main.h"
#include "structs.h"

#define DEBUG true
#define test 1

void doEvents();
void updatePlayer();
void updateBullet();
void doKeyDOWN(SDL_KeyboardEvent e);
void doKeyUP(SDL_KeyboardEvent e);
void fireBullet();
void _updateBullet();
void printBulletCount();
void spawnEnemies();
void updateEnemies();
void checkBulletHitsEnemy(struct Entity *bullet);
void spawnAsteroid();
void updateAsteroids();
void DisplayPlayerHealth(struct Entity *player);
void checkBulletHitsPlayer(struct Entity *bullet);

int main(){
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

    while (APPRUN)
    {
    SDL_SetRenderDrawColor(app.r,255,255,255,255);
    SDL_RenderClear(app.r);
    SDL_RenderCopy(app.r,app.background,NULL,NULL);

    doEvents();

    updatePlayer();    
    updateEnemies();
    // if(bullet.health > 0 || app.KEYBOARDEVENTS[SDL_SCANCODE_LCTRL]){
    //     updateBullet();
    // }
    _updateBullet();
    updateAsteroids();
    spawnEnemies();
    spawnAsteroid();
    SDL_RenderPresent(app.r);
    }

    SDL_DestroyRenderer(app.r);
    SDL_DestroyWindow(app.w);
    SDL_Quit();
    IMG_Quit();
    return 0;
}

void doEvents(){
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            APPRUN = false;
            break;
        case SDL_KEYDOWN:
            doKeyDOWN(e.key);
            break;
        case SDL_KEYUP:
            doKeyUP(e.key);
            break;
        default:
            break;
        }
    }
    
}

void doKeyDOWN(SDL_KeyboardEvent e){
    if(e.keysym.scancode < 350){
        app.KEYBOARDEVENTS[e.keysym.scancode] = true;
    }
}


void doKeyUP(SDL_KeyboardEvent e){
    if(e.keysym.scancode < 350){
        app.KEYBOARDEVENTS[e.keysym.scancode] = false;
    }
}

void updatePlayer(){

    if(app.KEYBOARDEVENTS[SDL_SCANCODE_UP]){
        #if DEBUG
        printf("update player SDL_SCANCODE_UP\n");
        #endif
        player.box.y -= player.box.h/4;
        if(player.box.y < 0) player.box.y = 0;
    }

    if(app.KEYBOARDEVENTS[SDL_SCANCODE_DOWN]){
        #if DEBUG
        printf("update player SDL_SCANCODE_DOWN\n");
        #endif
        player.box.y += player.box.h/4;
        if((player.box.y + player.box.h) >= SCREEN_HEIGHT) player.box.y = SCREEN_HEIGHT - player.box.h;
    }

    if(app.KEYBOARDEVENTS[SDL_SCANCODE_RIGHT]){
        #if DEBUG
        printf("update player SDL_SCANCODE_RIGHT\n");
        #endif
        player.box.x += player.box.w/4;
        if((player.box.x + player.box.w) >= SCREEN_WIDTH) player.box.x = SCREEN_WIDTH - player.box.w;
    }

    if(app.KEYBOARDEVENTS[SDL_SCANCODE_LEFT]){
        #if DEBUG
        printf("update player SDL_SCANCODE_LEFT\n");
        #endif
        player.box.x -= player.box.w/4;
        if(player.box.x < 0) player.box.x = 0;
    }

    if(app.KEYBOARDEVENTS[SDL_SCANCODE_LCTRL]){
        #if DEBUG
        printf("update player SDL_SCANCODE_LCTRL\n");
        #endif
        if(--player.reloadRate <=0){
            fireBullet(&player);
        }
        
    }

    SDL_RenderCopy(app.r,player.t,NULL,&player.box);
    DisplayPlayerHealth(&player);
}

void fireBullet(struct Entity *sender){
    
    struct Entity *newbullet;
    newbullet = (struct Entity*)malloc(sizeof(struct Entity));
    newbullet->next = bullethead;
    bullethead = newbullet;

    SDL_Surface *s; 
    if(sender->side == Type_Player){
        s= IMG_Load("assets/player_laser.png");
    }else{
        s= IMG_Load("assets/enemy_laser.png");
    }
    
    newbullet->t = SDL_CreateTextureFromSurface(app.r,s);
    SDL_FreeSurface(s);
    if(!newbullet->t){
         printf("Error while creating new bullet texture %s",SDL_GetError());
         free(newbullet);
    }

    SDL_QueryTexture(newbullet->t,NULL,NULL,&newbullet->box.w,&newbullet->box.h);
    newbullet->box.x = sender->box.x + (sender->box.h/2 - newbullet->box.h/2);
    if(sender->side == Type_Player){
        newbullet->box.y = sender->box.y;
        newbullet->dy = -20;
        sender->reloadRate = 10;
    }else{
        newbullet->box.y = sender->box.y + sender->box.h;
        newbullet->dy = sender->dy * 2;
        sender->reloadRate = 30;
    }
    
    newbullet->dx = 0;
    newbullet->health = 1;
    newbullet->side = sender->side;
}

void spawnEnemies(){

    if(--enemySpwawnCounter <= 0){
    struct Entity *enemy;
    SDL_Surface* s;
    enemy = (struct Entity*)malloc(sizeof(struct Entity));
    memset(enemy,0,sizeof(struct Entity));

    enemy->next = enemyhead;
    enemyhead = enemy;

    s= IMG_Load("assets/enemy_1.png");
    enemy->t = SDL_CreateTextureFromSurface(app.r,s);
    SDL_FreeSurface(s);
    if(!enemy->t){
        printf("Error while creating enemy texture %s",SDL_GetError());
        enemyhead = enemy->next;
        free(enemy);
        return;
    }

    SDL_QueryTexture(enemy->t,NULL,NULL,&enemy->box.w,&enemy->box.h);
    enemy->box.x = rand() % (SCREEN_WIDTH - enemy->box.w);
    enemy->dx = 0;
    enemy->box.y = 0 - enemy->box.h;
    enemy->dy = (4+ rand() % 6);
    enemy->health = 100;
    enemy->side = Type_Enemey;
    enemy->reloadRate = 30;

    enemySpwawnCounter = 20 + rand() % 60;
    }
}

void spawnAsteroid(){

    if(--asteroidSpwawnCounter <= 0){

    
    struct Entity *asteroid;
    SDL_Surface* s;
    asteroid = (struct Entity*)malloc(sizeof(struct Entity));
    memset(asteroid,0,sizeof(struct Entity));
    s = IMG_Load("assets/rock1.png");
    asteroid->t = SDL_CreateTextureFromSurface(app.r,s);

    if(!asteroid->t){
        printf("Error while creating asteroid texture %s",SDL_GetError());
        free(asteroid);
        exit(1);
    }

    asteroid->next = astHead;
    astHead = asteroid;

    asteroid->box.h = 30;
    asteroid->box.w = 30;
    asteroid->box.x = SCREEN_WIDTH;
    asteroid->box.y = rand() % SCREEN_HEIGHT;
    asteroid->dx = -5 - rand() % 5;
    asteroid->dy = 1 + rand() % 5;
    asteroid->health = 1;
    asteroid->side = Type_Enemey;
    asteroidSpwawnCounter = 50 + rand() % 100;
    }
}
void updateEnemies(){

    struct Entity *prevenemy, *currenemy;
    int count = 0;
    prevenemy = 0;
    currenemy = enemyhead;

    while(currenemy){
        count++;

        if(currenemy->health > 0){
            if(currenemy->box.y > SCREEN_HEIGHT){
                currenemy->health = 0;
            }else{
                currenemy->box.y += currenemy->dy;
                currenemy->box.x += currenemy->dx;
                if(--currenemy->reloadRate <=0 ){
                    fireBullet(currenemy);
                }
            }
        }


        if(currenemy->health <= 0){
            
            if(prevenemy){
                
                prevenemy->next = currenemy->next;
                free(currenemy);
                currenemy = prevenemy->next;
            }else{
               enemyhead = currenemy->next;
                free(currenemy);
                currenemy = enemyhead;
            }
        
        }
        else{
            //SDL_RenderCopy(app.r,currenemy->t,NULL,&currenemy->box);
            SDL_RenderCopyEx(app.r,currenemy->t,NULL,&currenemy->box,0,NULL,SDL_FLIP_VERTICAL);
            prevenemy = currenemy;
            currenemy = currenemy->next;
        }
    }
}

void printBulletCount(){
    struct Entity* currBullet;
    int count = 0;
    currBullet = bullethead;
    while (currBullet)
    {
        count++;
        if(!currBullet->next)
            break;
        else
            currBullet = currBullet->next;
    }
}



void _updateBullet(){
    struct Entity *prevBullet, *currBullet;
    int count = 0;
    prevBullet = 0;
    currBullet = bullethead;

    while (currBullet)
    {
        count++;
        if(currBullet->health > 0){
            if(currBullet->box.y < 0){
                currBullet->health = 0;
            }
           else{
                currBullet->box.y += currBullet->dy;
                currBullet->box.x += currBullet->dx;
                if(currBullet->side == Type_Player){
                    checkBulletHitsEnemy(currBullet);
                }
                else{
                    checkBulletHitsPlayer(currBullet);
                }
            }
        }
        
        if(currBullet->health <= 0){
            if(prevBullet){
                prevBullet->next = currBullet->next;
                free(currBullet);
                currBullet = prevBullet->next;
            }
            else{
                bullethead = currBullet->next;
                free(currBullet);
                currBullet = bullethead;
            }
        }else{
            SDL_RenderCopy(app.r,currBullet->t,NULL,&currBullet->box);
            prevBullet = currBullet;
            currBullet = currBullet->next;
        }
    }
}

void updateAsteroids(){
    struct Entity *prevAst, *currAst;
    int count = 0;
    prevAst = 0;
    currAst = astHead;

    while (currAst)
    {
        count++;
        if(currAst->health > 0){
            if(currAst->box.y > SCREEN_HEIGHT || currAst->box.x < 0){
                currAst->health = 0;
            }
           else{
                currAst->box.y += currAst->dy;
                currAst->box.x += currAst->dx;
                //checkBulletHitsEnemy(currBullet);
            }
        }
        
        if(currAst->health <= 0){
            if(prevAst){
                prevAst->next = currAst->next;
                free(currAst);
                currAst = prevAst->next;
            }
            else{
                astHead = currAst->next;
                free(currAst);
                currAst = astHead;
            }
        }else{
            SDL_RenderCopyEx(app.r,currAst->t,NULL,&currAst->box,90,NULL,SDL_FLIP_NONE);
            prevAst = currAst;
            currAst = currAst->next;
        }
    }
}

void checkBulletHitsEnemy(struct Entity *bullet){
    struct Entity *enemy;
    enemy = enemyhead;

    while (enemy)
    {
        if((bullet->box.y > enemy->box.y && bullet->box.y < enemy->box.y + enemy->box.h)
            &&
            (bullet->box.x > enemy->box.x && bullet->box.x < enemy->box.x + enemy->box.w)
         ){
            bullet->health = 0;
            enemy->health -= 50;
            break;
        }

        enemy = enemy->next;
    }
    
}


void checkBulletHitsPlayer(struct Entity *bullet){
     if((bullet->box.y > player.box.y && bullet->box.y < player.box.y + player.box.h)
            &&
            (bullet->box.x > player.box.x && bullet->box.x < player.box.x + player.box.w)
         ){
            player.health -=5;
            bullet->health = 0;
         }
}

void DisplayPlayerHealth(struct Entity *player){
    SDL_Color colr;
    colr.a =255;
    colr.r = (int)((float)255 * (float)((float)(100 -player->health)/100));
    colr.g = (int)((float)255 * (float)((float)player->health/100));
    colr.b = 0;

    printf("%d -- %d\n",player->health,colr.g);
    
    SDL_Rect fontrect;
    fontrect.h = 30;
    fontrect.w = 10 * 20;
    fontrect.x = 0;
    fontrect.y = 0;
    char heatlstr[10] ;
    sprintf(heatlstr,"HP: %d",player->health);

    SDL_Surface *s = TTF_RenderText_Solid(app.font,heatlstr,colr);
    SDL_Texture *t = SDL_CreateTextureFromSurface(app.r,s);
    SDL_RenderCopy(app.r,t,NULL,&fontrect);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

void updateBullet(){

    if(bullet.health > 0){
        bullet.box.y += bullet.dy;
        bullet.box.x  += bullet.dx;
    }else{
        bullet.health = 1;
        bullet.box.x = player.box.x + player.box.w/2;
        bullet.box.y = player.box.y;
        bullet.dy = -10;
    }

    if(bullet.box.y < 0){
        bullet.health = 0;
    }

    if(bullet.health > 0){
        SDL_RenderCopy(app.r,bullet.t,NULL,&bullet.box);
    }
}
