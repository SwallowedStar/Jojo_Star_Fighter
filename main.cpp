#include "gen.h" // dans gen.h il y a tous les liens vers nos autres fichiers CPP
#include <time.h>

using namespace sf; // On utilise l'environnement SF
using namespace std;

Event event;

int main()
{
    initImgs();
    initMaps();
    initFighters();
    bool inGame = false;
    srand(time(NULL));
    int choixMusique = 1;


    // la variable selectionMenu permet de surligner les boutons et de selectionner ensuite els menu dans la variable menu
    // La varible selecetionMap permet la selection de la Map surlaquelle on va jouer
    // la variable selectionPerso permet la selection du Personnage
    int selectionMap = 0, selectionMenu = 0, selectionPerso = 0;

    // mapId permet la désignation de la map
    // menu permet la designation du menu dans lequel on se trouve
    int mapId = -1, menu = 0; // La valeur -1 désigne le fait que la map n'est pas initialisée.

    /*
        Voici les valeurs des menus (Celle-ci on été affectées de manière croissante, dès que le menu a eu besoin d'être créé):
            -> 0 : Menu Principale
            -> 1 : Menu de selection de Map
            -> 2 : Menu d'Option
            -> 3 : Menu Crédits
            -> 4 : Cette valeur correspond au boutton quitter du programme :
                On arrete le programme si le menu = 4
            -> 5 : Menu de selection des personnages
            -> 6 : Interface de jeu
    */

    // Les idPerso permettent de savoir quel perso les joueurs ont choisi.
    int idPersoJ1 = -1, idPersoJ2 = -1; // La valeur -1 désigne le fait que les perso ne sont pas initialisés.
    int idJoueurActif = 1;


    // Un simple test en combat
    /*
        int selectionMap = 1, selectionMenu = 6, selectionPerso = 0;
        int mapId = 2, menu = 6;
        int idPersoJ1 = 3, idPersoJ2 = 0;
        int idJoueurActif = 1;
    */

    /*
           Initialisation des sons

            --------------------------------------------------------------------------------------

             On crée un sound effect pour chaque clic
        */
    SoundBuffer bufferSelect;
    if (!bufferSelect.loadFromFile(SON_SELECT))  // Si l'on ne trouve pas le son, on affice une erreur
    {
        printf("Le fichier son : %s est introuvable", SON_SELECT);
        exit(2);
    }

    Sound soundSelect;
    soundSelect.setBuffer(bufferSelect);

    Music musiqueFond; // On met en place la musique de fond
    if (!musiqueFond.openFromFile(MUSIQUE_FOND2))
    {
        printf("Impossible de mettre le son : %s\n", MUSIQUE_FOND);
        exit(4);
    }

    musiqueFond.setLoop(true); // On crée une loop
    musiqueFond.setVolume(100.0); // On baisse le volume
    musiqueFond.play(); // On commance à joueur la musique

    /*
        ---------------------------------------------------------------------------------------
    */


    Font f; // On met en place la police d'écriture
    if (!f.loadFromFile(FONT)) // On vérifie qu'elle existe
    {
        printf("Erreur Impossible de charger la police"); // si l    police n'existe pas, on le dit
        exit(0);
    }

    /*
        ---------------------------------------------------------------------------------------
    */

    RenderWindow fen(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jojo Star Fighter"); // C'est la fenètre principale
    fen.setFramerateLimit(FPS); // on fixe la limite des FPS à 120

    while (fen.isOpen()) // Tant que la fenetre est ouverte
    {
        while (fen.pollEvent(event)) // On prend tout les evenements
        {
            switch(event.type)
            {
            case Event::Closed : // si on veut fermer
                musiqueFond.stop(); // alors on arrete la musique
                fen.close(); // on ferme
                break; // et on quitte la boucle

            case Event::MouseMoved: // On met en place les différentes selections dans les différentes menus
                if (menu == 0) // Si on est dans le menu principale
                    selectionMenu = choixMenuP(event.mouseMove.x,event.mouseMove.y); // on utilise la selection spécifique à ce menu
                else if (menu == 1)  // Menu selection Map
                {
                    selectionMap = choixMap(event.mouseMove.x, event.mouseMove.y);
                    selectionMenu = detectionChangementMenu(event.mouseMove.x, event.mouseMove.y, 0, 1, 5, selectionMenu);
                }
                else if (menu == 2) // Menu Options
                    selectionMenu = detectionRetourMenuPrecedent(event.mouseMove.x,event.mouseMove.y, 2, 0, selectionMenu);
                else if(menu == 3) // Menu Crédits
                    selectionMenu = detectionRetourMenuPrecedent(event.mouseMove.x,event.mouseMove.y, 3, 0, selectionMenu);
                else if(menu == 5) // Menu selection Perso
                {
                    selectionPerso = choixPerso(event.mouseMove.x, event.mouseMove.y);
                    selectionMenu = detectionChangementMenu(event.mouseMove.x,event.mouseMove.y, 1, 5, 6, selectionMenu);
                }
                break; // on quitte le switch

            case Event::MouseButtonPressed: // Si on clique sur un menu, Alors on réagit en conséquence
                if (selectionMenu == 4)   // Si on clique sur le bouton Quitter
                {
                    fen.close(); // La fenetre s'arrete
                    musiqueFond.stop(); // La musique s'arrete
                }

                if (menu == 1 && selectionMap != -1) // Si on clique sur une map et que cette selection n'est pas invalide ...
                    mapId = selectionMap; // Alors on confirme la map de eju

                if (menu == 5 )  // Si l'on est le menu de selection des personnages
                {
                    if( selectionPerso != -1)  // si la selection perso n'est pas invalide
                    {
                        if (idJoueurActif == 1) // Si le joueur qui est en cour de selection est le permier
                            idPersoJ1 = selectionPerso; // Alors on confirme sa selection du joueur 1
                        else // Si le joueur qui est en cour de selection est le deuxième
                            idPersoJ2 = selectionPerso; // Alors on confirme sa selection de joueur 2
                    }
                }

                menu = selectionMenu; // de manière générale, la selectionMenu correpond à l'id du menu que l'on veut visiter

                /*
                    Si le joueur veut aller dans la selection de personnage ( en fait selectionMenu = 5, cela veut dire,
                    qu'avant la ligne 139, menu = 1)
                    et que la mapID n'est pas valide (Pas selectionné par le joueur)...
                */
                if ((menu == 5 && mapId < 0))
                {
                    menu = 1; // Alors on interdit le fait de pouvoir changer de menu
                } // Cela permet de ne pas aller en combat sans avoir selectioné de map

                /*
                    Si les joueurs veulent aller en combat  (menu = 6) mais les ids des persos ne sont pas valides
                    (Les joueurs n'ont pas selectioné de persos), alors on retourne au menu 5;
                */
                else if (menu == 6 &&((idPersoJ1 < 0) || (idPersoJ2 < 0)))
                {
                    menu = 5;
                } // Cela permet de ne pas aller en combat sans personnages

                soundSelect.play(); // On joue le son de selection à chaque fois que l'on clique quelque part
                break; // On quitte le switch

            case Event::MouseWheelMoved: // Cet input permet de changer de "joueur actif", c'est à dire que
                // Si On enclenche cet evenement, on pourra passer de J1 à J2
                if (idJoueurActif ==1) // Si J1 était la avant que l'on tourne la roue de la souris,
                    idJoueurActif = 2; // Alors C'est désormais le J2 qui a la main (pour la selection des persos)
                else
                    idJoueurActif = 1;
                break;

            default : // On ne prend en compte aucun autre evènement
                break;
            }

        }

        fen.clear(); // On efface la fenetre

        if (menu == 0) // Si on est dans le menu d'accueil, on l'affiche
            afficherMenuP(fen, selectionMenu, f);
        else if (menu == 1) // Menu Selection de Map
        {
            afficherSelectionMap(fen, selectionMap, mapId, selectionMenu, f);
        }
        else if (menu == 2) // Menu Options
            afficherMenuO(fen, selectionMenu, f);

        else if (menu == 3) // Menu Credits
            afficherMenuC(fen, selectionMenu, f);

        else if (menu == 5)  // Menu Selection de Personnages
        {
            afficherSelectionPerso(fen, selectionPerso, idPersoJ1, idPersoJ2, selectionMenu, f);
            afficherCurseurJoueur(idJoueurActif, (float)SCREEN_WIDTH / 5, (float)SCREEN_HEIGHT / 2 - 50, f, fen);
        }

        else if (menu == 6) // Interface de jeu
        {
            musiqueFond.stop();
            choixMusique = rand()%3+1;
            if (choixMusique == 1)
            {
                musiqueFond.openFromFile(MUSIQUE_COMBAT);
                musiqueFond.setVolume(100.0);
            }
            else if (choixMusique == 2)
            {
                musiqueFond.openFromFile(MUSIQUE_COMBAT2);
                musiqueFond.setVolume(100.0);
            }
            else if (choixMusique == 3)
            {
                musiqueFond.openFromFile(MUSIQUE_COMBAT3);
                musiqueFond.setVolume(100.0);
            }
            musiqueFond.play();
            jeu(idPersoJ1, idPersoJ2, mapId, f, fen, event); // On lance le jeu qui a sa propre boucle d'evenement
            menu = 1; // à la fin du jeu on retourne au menu principale
            selectionMenu = 0;
            musiqueFond.stop();
            choixMusique = rand()%4+1;

            if (choixMusique == 1)
            {
                musiqueFond.openFromFile(MUSIQUE_FOND);
                musiqueFond.setVolume(50.0);
            }
            else if (choixMusique == 2)
            {
                musiqueFond.openFromFile(MUSIQUE_FOND2);
                musiqueFond.setVolume(100.0);
            }
            else if (choixMusique == 3)
            {
                musiqueFond.openFromFile(MUSIQUE_FOND3);
                musiqueFond.setVolume(100.0);
            }
            else if (choixMusique == 4)
            {
                musiqueFond.openFromFile(MUSIQUE_FOND4);
                musiqueFond.setVolume(100.0);
            }
            musiqueFond.play();
        }
        fen.display(); // On affiche la fenetre
    }

    return EXIT_SUCCESS;
}

