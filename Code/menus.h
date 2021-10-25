#include <SFML/Graphics.hpp>
#include <iostream>
#define FOND_PRINCIPALE "imgs/Menus/fond1.jpg"
#define FOND_SECONDAIRE "imgs/Menus/fond2.png"
#define IMG_ZQSD "imgs/Menus/ZQSD.PNG"
#define IMG_FLECHE "imgs/Menus/fleches.PNG"
#define IMG_RT "imgs/Menus/RT.PNG"
#define IMG_02 "imgs/Menus/02.PNG"
#define TAILLES_IMGS_OPTION 250
#define TAILLE_ICONES 65

using namespace sf; // On utilise l'environnement sf

void afficherMenuP(RenderWindow &fen, int selectionMenu, Font f); // affiche le menu principale
int choixMenuP(int x, int y); // permet de faire un choix de menu en fonction de la position de la souris en x et en y

void afficherSelectionMap(RenderWindow &fen, int selectionMap,int mapId ,int selectionMenu, Font f); // affiche le menu de selection de Map
int choixMap(int x, int y);

void afficherSelectionPerso(RenderWindow &fen, int selectionPerso, int idPersoJ1, int idPersoJ2, int selectionMenu, Font f); // affiche le menu de selection de Perso
int choixPerso(int x, int y);
int choixMenuP(int x, int y);

void afficherMenuO(RenderWindow &fen, int selectionMenu, Font f); // affiche le menu des options

void afficherMenuC(RenderWindow &fen, int selectionMenu, Font f); // affiche le menu des credits

void afficherBoutonQuitter(RenderWindow &fen, int selectionMenu, Font f, int idMenuPrecedent, char text[]);
void afficherBoutonSuivant(RenderWindow &fen, int selectionMenu, Font f, int idMenuSuivant, char text[]);

void afficherTitre(char titreC[], Font f, RenderWindow &fen);

int detectionRetourMenuPrecedent(int x, int y, int idMenu, int idMenuPrecedent, int selecetion);
//int detectionRetourMenuSuivant(int x, int y, int idMenu, int idMenuSuivant, int selection);
int detectionChangementMenu(int x, int y, int idMenuPrecedent, int idMenu, int idMenuSuivant, int selection);

void dessinerBackground(char nomImg[], RenderWindow &fen);

void afficherCurseurJoueur(int idJoueur, int x, int y, Font f, RenderWindow &fen);

void affichagePause(int selection, Font f, RenderWindow &fen);
int choixPause(int x, int y);
