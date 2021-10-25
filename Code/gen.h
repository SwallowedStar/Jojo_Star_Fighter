#include <SFML/Graphics.hpp> // On appel SFML/Graphics.hpp
#include <SFML/Audio.hpp> // On appel SHML/Audio
#include <iostream>
#include <string.h>
#include <time.h>

#include "menus.h" // On appel le header menus.h qui regroupe toutes les fonctions liées au menu
#include "affichageStage.h"
#include "erreur.h"
#include "Personnage.h"
#include "Animations.h"

#define SCREEN_WIDTH 1300 // On défini une taille maximum pour notre fenetre
#define SCREEN_HEIGHT 700
#define FONT "font/SF_Fedora.ttf"
#define FPS 120 // On défini un nombre précis de FPS
#define SON_WRY "sons/wry.wav"
#define SON_SELECT "sons/select.wav"
#define MUSIQUE_FOND "sons/musiqueFond.ogg"
#define MUSIQUE_FOND2 "sons/musiqueFond2.ogg"
#define MUSIQUE_FOND3 "sons/musiqueFond3.ogg"
#define MUSIQUE_FOND4 "sons/musiqueFond4.ogg"
#define MUSIQUE_COMBAT "sons/incursione.ogg"
#define MUSIQUE_COMBAT2 "sons/endOfTheWorld.ogg"
#define MUSIQUE_COMBAT3 "sons/giorno.ogg"
#define SON_GO "sons/go.wav"

 void jeu(int idperso1, int idperso2, int idMap, Font f,RenderWindow &app, Event &event);
//void affichageMap(int idMap, Clock &montre, RenderWindow &app);
void affichageMap(int idMap, RenderWindow &app);
void affichageVie(int vieJ1, int vieJ2, RenderWindow &app);
void afficheGagnant(int vieJ1, int virJ2, Font f, RenderWindow &app);
void initImgs();

void animation(Sprite *sprite, Anim *coord, Clock *clock, bool resetOrigin);
void trueAnimation(Fighter *fighterPointer, int animation, int nbAnim);
void hit(Fighter *fighterOne, Fighter *fighterTwo);
bool isOnGround(Sprite object, RectangleShape ground);
void input(Fighter *player, RectangleShape sol ,Keyboard::Key moveRight, Keyboard::Key moveLeft, Keyboard::Key jump, Keyboard::Key guard, Keyboard::Key attack, Keyboard::Key specialAttack);
Anim setAnimation(IntRect coord, int frameLimit, float delayInterFrames);

void initFighters();
Anim setAnimation(IntRect coord, int frameLimit, float delayInterFrames);

void initMaps();
int pause(Font f, RenderWindow &fen);

void trueAnimation(Fighter *fighterPointer, int animation, int nbAnim);
void hit(Fighter *fighterOne, Fighter *fighterTwo);

typedef struct
{
    int x;
    int y;
} Point;

