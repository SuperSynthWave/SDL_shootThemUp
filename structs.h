#if !INCLUDESTRUCTS
#define INCLUDESTRUCTS true

#include "main.h"

typedef enum{
    startpage,
    gameloop,
    gameoverpage
}gameMode;

typedef struct {
 SDL_Window* w;
 SDL_Renderer* r;
 SDL_Texture* background;
 TTF_Font *font;
 gameMode mode;
 int score;
 bool KEYBOARDEVENTS[350];
} App;

enum EntityType{Type_Player,Type_Enemey};

struct Entity {
 SDL_Rect box;
 SDL_Texture* t;
 int health;
 int dx;
 int dy;
 int reloadRate;
 int lifesteal;
 enum EntityType side;
 struct Entity *next;
};


App app;
struct Entity player;
struct Entity bullet;
struct Entity *bullethead;
struct Entity *enemyhead;
struct Entity *astHead;
#endif