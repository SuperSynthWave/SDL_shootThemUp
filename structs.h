#if !INCLUDESTRUCTS
#define INCLUDESTRUCTS

#include "main.h"

typedef struct {
 SDL_Window* w;
 SDL_Renderer* r;
 SDL_Texture* background;
 TTF_Font *font;
 bool KEYBOARDEVENTS[350];
 bool up;
 bool down;
 bool left;
 bool right;
 bool fire;
} App;

enum EntityType{Type_Player,Type_Enemey};

struct Entity {
 SDL_Rect box;
 SDL_Texture* t;
 int health;
 int dx;
 int dy;
 int reloadRate;
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