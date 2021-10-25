#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;

typedef struct
{
    int offsetX;
    IntRect animCoord;
    int frameLimit;
    float delayMillis;
    //SoundBuffer animBuffer;
} Anim;

typedef struct
{
    bool attack = false;
    bool attackStand = false;
} AttackType;

typedef struct
{
    bool normal = false;
    bool special = false;
    bool attackBurst; //Donne l'information si l'attaque est une rafale de coups
    int burstCount;
}Attack;

typedef struct
{
    char nom[20]; //Nom du personnage
    int vie = 10; //Vie actuelle/Temporaire
    int VIE_MAX = 10; //vie Maximum NE PAS TOUCHER DANS LE MAIN OU LES SOUS PROGRAMMES
    int degats = 0; //Degats re�us par le personnage
    bool attackInst = true; //Permet d'attaquer avec la premi�re ou seconde attaque normale
    bool theWorld = false;
    bool jump = false; //Permet de savoir si le personnage est en saut
    int fighterAnim; //Permet de s�lectiionner l'animation du personnage
    int standAnim; //Permet de s�lectiionner l'animation du stand (si s�par�)
    Sprite fighter; //Sprite du personnage
    Texture fighterText;
    bool externStand; //Donne l'information si le stand est sur un sprite externe lors des attaques
    Sprite stand; //Sprite du stand
    Clock fighterCl; //Horloge d'animation du combattant
    Clock standCl; //Horloge d'animation du stand
    Clock trueAnimCl; //Horloge des animations bloquant les commandes
    Clock cooldown;//Horloge du cooldown
    bool canAnim = false; //Permet de ne pas r�p�ter une animation (a remetre sur true pour activer !!!)
    bool animBegin = true; //Permet de savoir si l'animation est termin�e
    bool guardActive = 0; //Permet de savoir si la garde du personnage est activ�e
    bool standActive = 0; //Permet de savoir si la garde du stand est activ�e
    Vector2f velocity = Vector2f(0.f,0.f); //D�placements du personnage sous forme de vecteur
    Vector2f standOffset; //D�callage du sprite du stand
    Anim animation[15]; //Coordon�es des animations
    Attack attackQualification; //Permet de savoir si on effectue une attaque sp�ciale ou normale
    AttackType attackMode; //Permet de savoir si on attaque avec le stand
    bool controlAcces = true; //Permet d'activer ou non les controles du personnage
    bool isAlive = true; // Permet de savoir s'il est en vie
    float pasSaut = SAUT_MAX;
    //int fighterFrame = 0;
    //Sound fighterSound;
} Fighter;

typedef struct
{
    Sprite mapSprite;
    Clock montreSprite;
    int standOffset = 0;
    Texture mapTexture;
    Anim animation;
} Map;


