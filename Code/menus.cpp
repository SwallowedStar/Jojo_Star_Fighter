#include "gen.h"


using namespace sf; // On utilise l'environnement sf

void afficherMenuP(RenderWindow &fen, int selectionMenu, Font f) // permet d'afficher le menu titree
{
    dessinerBackground(FOND_PRINCIPALE, fen);

    Text titre; // On crée le "titre" du jeu
    titre.setFont(f); // on lui applique la police de base
    titre.setCharacterSize(100); // Les lettres auront une taille de 100
    titre.setString("Jojo's Star Fighter"); // On met le nom du jeu
    titre.setPosition(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT * 0.05); // on met le titre au centre
    titre.setColor(Color(247, 247, 7)); // On lui donne une couleur Orange


    fen.draw(titre); // Par dessus on dessine le titre

    Text menus[4]; // On crée du text pour le menu
    String titres[] = {"Jouer", "Options", "Crédits", "Quitter"}; // On crée les chaines de caractère correspondantes

    int i; // afin que le for fonctionne
    for (i = 0; i < 4; i++)
    {
        menus[i].setFont(f);
        menus[i].setCharacterSize(50);
        menus[i].setColor(Color(00, 00, 00));
        menus[i].setString(titres[i]);

        menus[i].setPosition(SCREEN_WIDTH * 0.35 + 100, SCREEN_HEIGHT*(i*0.15 + 0.4));

        /*
            Grâce à la boucle, on setup les cadres qui vont servir pour encadrer les titres
         */
        RectangleShape rectSelection(Vector2f(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.1));
        rectSelection.setOutlineThickness(10);
        rectSelection.setPosition(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT*(i*0.15 + 0.4));

        if (i == selectionMenu-1){
            menus[i].setCharacterSize(60);
            menus[i].setPosition(SCREEN_WIDTH * 0.35 + 90, SCREEN_HEIGHT*(i*0.15 + 0.4));
            rectSelection.setFillColor(Color(247,248, 7));
            rectSelection.setOutlineColor(Color(200,200,7));
        }
        else{

            rectSelection.setFillColor(Color::White);
            rectSelection.setOutlineColor(Color(200,200,200));
        }
        fen.draw(rectSelection);


        fen.draw(menus[i]);
    }

}

int choixMenuP(int x, int y)
{
    int i, selection = 0;
    if (x >= SCREEN_WIDTH * 0.35 && x <= SCREEN_WIDTH * 0.35 + SCREEN_WIDTH * 0.3){
        for (i = 0; i < 4; i++){
            if (y >= SCREEN_HEIGHT*(i*0.15 + 0.4) && y <= SCREEN_HEIGHT*(i*0.15 + 0.5))
                selection = i + 1;
        }
    }
    else
        selection = 0;

    return selection;
}

void afficherSelectionMap(RenderWindow &fen, int selectionMap,int mapId ,int selectionMenu, Font f) // affiche le menu de selection de Map
{
    dessinerBackground(FOND_SECONDAIRE, fen);

    afficherTitre("Choix Map", f, fen);

    int i;
    for (i = 1; i <= NOMBRE_DE_MAPS; i++)
    {
        RectangleShape cadre(Vector2f(SCREEN_WIDTH * 0.17, SCREEN_HEIGHT * 0.2));
        cadre.setOutlineThickness(10);
        cadre.setPosition(SCREEN_WIDTH * 0.2 * (i-1) + 20, SCREEN_HEIGHT * 0.4);

        if (selectionMap == i)
            cadre.setOutlineColor(Color(200,200,7));
        else if(mapId == i)
            cadre.setOutlineColor(Color::Red);
        else
            cadre.setOutlineColor(Color(200,200,200));

        char nomImg[23];
        sprintf(nomImg, "imgs/Arenes/Arene%d.png", i);

        Texture textureMap;
        if(!(textureMap.loadFromFile(nomImg))){
            printf("Impossible de charger l'image : %s", nomImg);
            exit(3);
        }

        Sprite spriteMap(textureMap);
        spriteMap.scale(SCREEN_WIDTH * 0.17 / textureMap.getSize().x, SCREEN_HEIGHT * 0.2 / textureMap.getSize().y);
        spriteMap.setPosition(SCREEN_WIDTH * 0.2 * (i-1) + 20, SCREEN_HEIGHT * 0.4);


        fen.draw(cadre);
        fen.draw(spriteMap);

    }

    afficherBoutonQuitter(fen, selectionMenu, f, 0, "Retour");
    afficherBoutonSuivant(fen, selectionMenu, f, 5, "Suivant");
}
int choixMap(int x, int y)
{
    int selection = -1, i;
    for (i = 1; i <= NOMBRE_DE_MAPS; i++){
        if (y >= SCREEN_HEIGHT * 0.4 && y <=  SCREEN_HEIGHT * 0.6){
            if (x >= SCREEN_WIDTH * 0.2 * (i-1) + 20 && x <= SCREEN_WIDTH * 0.2 * (i-1) + 20 + SCREEN_WIDTH * 0.17 )
                selection = i;
        }
    }
    return selection;
}

void afficherSelectionPerso(RenderWindow &fen, int selectionPerso, int idPersoJ1, int idPersoJ2, int selectionMenu, Font f) // affiche le menu de selection de Perso
{
    dessinerBackground(FOND_SECONDAIRE, fen);

    afficherTitre("Selection Personnage", f, fen);

    afficherBoutonQuitter(fen, selectionMenu, f, 1, "Retour");
    afficherBoutonSuivant(fen, selectionMenu, f, 6, "Suivant");

    Point positionIcone;

    int i, x, y=0;
    for (i = 0; i < NOMBRE_DE_PERSO; i++){
        x = i;
        y = 0;
        while(x > 2){
            x = x - 3;
            y ++;
        }

        positionIcone.x = SCREEN_WIDTH * 0.1 * x + SCREEN_WIDTH * 0.5 - (TAILLE_ICONES * 1.5 + SCREEN_WIDTH * 0.1 ) ;
        positionIcone.y = SCREEN_HEIGHT * 0.3 + y * 0.2 * SCREEN_HEIGHT;

        RectangleShape cadre(Vector2f(TAILLE_ICONES, TAILLE_ICONES));
        cadre.setOutlineThickness(10);
        cadre.setPosition(positionIcone.x, positionIcone.y);

        if (selectionPerso == i)
            cadre.setOutlineColor(Color(200,200,7));
        else if(idPersoJ1 == i)
            cadre.setOutlineColor(Color::Red);
        else if(idPersoJ2 == i)
            cadre.setOutlineColor(Color::Blue);
        else
            cadre.setOutlineColor(Color(200,200,200));

        char nomImg[23];
        if (i == 0)
            sprintf(nomImg, "%s", PERSO_AVDOL);
        else if (i == 1)
            sprintf(nomImg, "%s", PERSO_DIO);
        else if (i == 2)
            sprintf(nomImg, "%s", PERSO_JOSEPH);
        else if (i == 3)
            sprintf(nomImg, "%s", PERSO_JOSUKE);
        else if (i == 4)
            sprintf(nomImg, "%s", PERSO_JOTARO);
        else if (i == 5)
            sprintf(nomImg, "%s", PERSO_KAKYOIN);
        else if (i == 6)
            sprintf(nomImg, "%s", PERSO_POLNAREFF);
        else if (i == 7)
            sprintf(nomImg, "%s", PERSO_HOLHORSE);

        Texture textureIconePerso;
        if(!(textureIconePerso.loadFromFile(nomImg))){
            printf("Impossible de charger l'image : %s", nomImg);
            exit(3);
        }

        Sprite spriteIconePerso(textureIconePerso);
        spriteIconePerso.scale((float)TAILLE_ICONES / textureIconePerso.getSize().x, (float)TAILLE_ICONES / textureIconePerso.getSize().y);
        spriteIconePerso.setPosition(SCREEN_WIDTH * 0.1 * x + SCREEN_WIDTH * 0.5 - (TAILLE_ICONES * 1.5 + SCREEN_WIDTH * 0.1 ), SCREEN_HEIGHT * 0.3 + y * 0.2 * SCREEN_HEIGHT);

        fen.draw(cadre);
        fen.draw(spriteIconePerso);
    }
}
int choixPerso(int x, int y)
{
    int selectionPerso = -1, i;
    int xI;
    int yI = 0;
    Point positionIcone;
    for (i = 0; i < NOMBRE_DE_PERSO; i ++)
    {
        xI = i;
        yI = 0;
        while(xI > 2){
            xI = xI - 3;
            yI ++;
        }

        positionIcone.x = SCREEN_WIDTH * 0.1 * xI + SCREEN_WIDTH * 0.5 - (TAILLE_ICONES * 1.5 + SCREEN_WIDTH * 0.1 );
        positionIcone.y = SCREEN_HEIGHT * 0.3 + yI * 0.2 * SCREEN_HEIGHT;

        if (x >= positionIcone.x && x <= positionIcone.x + TAILLE_ICONES && y >= positionIcone.y && y <= positionIcone.y + TAILLE_ICONES)
            selectionPerso = i;
    }
    return selectionPerso;
}

void afficherMenuO(RenderWindow &fen, int selectionMenu, Font f)
{
    dessinerBackground(FOND_SECONDAIRE, fen);

    afficherTitre("Options", f, fen);

    Text zqsdText;
    zqsdText.setString("J1");
    zqsdText.setFont(f);
    zqsdText.setCharacterSize(50);
    zqsdText.setPosition(SCREEN_WIDTH / 4 - TAILLES_IMGS_OPTION / 2 , SCREEN_HEIGHT / 2 - TAILLES_IMGS_OPTION / 2 - 50 );
    zqsdText.setColor(Color::Red);

    Text flecheText;
    flecheText.setString("J2");
    flecheText.setFont(f);
    flecheText.setCharacterSize(50);
    flecheText.setPosition(SCREEN_WIDTH * 3 / 4 - TAILLES_IMGS_OPTION / 2 , SCREEN_HEIGHT / 2 - TAILLES_IMGS_OPTION / 2 - 50 );
    flecheText.setColor(Color::Blue);

    Text attaqueText;
    attaqueText.setString("Attaques");
    attaqueText.setFont(f);
    attaqueText.setCharacterSize(30);
    attaqueText.setPosition( 100  , SCREEN_HEIGHT * 3 / 4 - 50 );
    attaqueText.setColor(Color::Black);

    Texture Texture02;
    if (!(Texture02.loadFromFile(IMG_02)))
    {
        printf("Image %s non disponible", IMG_02);
        exit(IMG_NON_EXISTANTE);
    }

    Sprite Sprite02(Texture02);
    Sprite02.scale((float)150 / Texture02.getSize().x, (float)50/ Texture02.getSize().y);
    Sprite02.setPosition( 900, SCREEN_HEIGHT * 3/4 -55);

    Texture RTTexture;
    if (!(RTTexture.loadFromFile(IMG_RT)))
    {
        printf("Image %s non disponible", IMG_RT);
        exit(IMG_NON_EXISTANTE);
    }

    Sprite RTSprite(RTTexture);
    RTSprite.scale((float)150 / RTTexture.getSize().x, (float)50/ RTTexture.getSize().y);
    RTSprite.setPosition( 250 , SCREEN_HEIGHT * 3 / 4 - 60);

    Texture zqsdTexture;
    if (!(zqsdTexture.loadFromFile(IMG_ZQSD)))
    {
        printf("Image %s non disponible", IMG_ZQSD);
        exit(IMG_NON_EXISTANTE);
    }

    Sprite zqsdSprite(zqsdTexture);
    zqsdSprite.scale((float)TAILLES_IMGS_OPTION / zqsdTexture.getSize().x, (float)TAILLES_IMGS_OPTION / zqsdTexture.getSize().y);
    zqsdSprite.setPosition(SCREEN_WIDTH / 4 - TAILLES_IMGS_OPTION / 2 , SCREEN_HEIGHT / 2 - TAILLES_IMGS_OPTION / 2);

    Texture flecheTexture;
    if (!(flecheTexture.loadFromFile(IMG_FLECHE)))
    {
        printf("Image %s non disponible", IMG_FLECHE);
        exit(IMG_NON_EXISTANTE);
    }

    Sprite flecheSprite(flecheTexture);
    flecheSprite.scale((float)TAILLES_IMGS_OPTION / flecheTexture.getSize().x, (float)TAILLES_IMGS_OPTION / flecheTexture.getSize().y);
    flecheSprite.setPosition(SCREEN_WIDTH * 3 / 4 - TAILLES_IMGS_OPTION / 2, SCREEN_HEIGHT / 2 - TAILLES_IMGS_OPTION / 2);

    fen.draw(zqsdSprite);
    fen.draw(flecheSprite);

    fen.draw(zqsdText);
    fen.draw(flecheText);

    fen.draw(attaqueText);
    fen.draw(RTSprite);
    fen.draw(Sprite02);
    afficherBoutonQuitter(fen, selectionMenu, f, 0, "Quitter");

}

void afficherMenuC(RenderWindow &fen, int selectionMenu, Font f)
{
    dessinerBackground(FOND_SECONDAIRE, fen);

    Text devs[4];
    devs[0].setString("DASSEUX Damien");
    devs[1].setString("DEPOISIER Antoine");
    devs[2].setString("MALACARNE Etienne");
    devs[3].setString("MOLLINARI Valentin");

    afficherTitre("Crédits", f, fen);

    RectangleShape contenant(Vector2f(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.5));
    contenant.setFillColor(Color::White);
    contenant.setOutlineThickness(10);
    contenant.setOutlineColor(Color(200,200,200));
    contenant.setPosition(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT*0.4);

    fen.draw(contenant);

    int i;
    for(i = 0; i < 4; i ++)
    {
        devs[i].setColor(Color::Red);
        devs[i].setFont(f);
        devs[i].setPosition(SCREEN_WIDTH * 0.35 + 50, SCREEN_HEIGHT*(i*0.15 + 0.4));
        fen.draw(devs[i]);
    }

    afficherBoutonQuitter(fen, selectionMenu, f, 0, "Quitter");

}

void afficherBoutonQuitter(RenderWindow &fen, int selectionMenu, Font f, int idMenuPrecedent, char text[])
{
    Text quit;
    quit.setFont(f);
    quit.setString(text);
    quit.setPosition(SCREEN_WIDTH * 0.1 + 20, SCREEN_HEIGHT * 0.85 + 10);
    quit.setColor(Color::Black);

    RectangleShape boutonQSelect(Vector2f(175,50));
    boutonQSelect.setOutlineThickness(10);
    boutonQSelect.setPosition(SCREEN_WIDTH * 0.1, SCREEN_HEIGHT * 0.85);

    if(selectionMenu == idMenuPrecedent){
        quit.setCharacterSize(45);
        quit.setPosition(SCREEN_WIDTH * 0.1 - 5, SCREEN_HEIGHT * 0.85 - 5);
        boutonQSelect.setFillColor(Color(247,247,7));
        boutonQSelect.setOutlineColor(Color(175,200,7));
    }
    else {
        boutonQSelect.setFillColor(Color::White);
        boutonQSelect.setOutlineColor(Color(200,200,200));
    }

    fen.draw(boutonQSelect);
    fen.draw(quit);
}

void afficherBoutonSuivant(RenderWindow &fen, int selectionMenu, Font f, int idMenuSuivant, char text[])
{
    Text quit;
    quit.setFont(f);
    quit.setString(text);
    quit.setPosition(SCREEN_WIDTH * 0.75 + 20, SCREEN_HEIGHT * 0.85 + 10);
    quit.setColor(Color::Black);

    RectangleShape boutonQSelect(Vector2f(175,50));
    boutonQSelect.setOutlineThickness(10);
    boutonQSelect.setPosition(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.85);

    if(selectionMenu == idMenuSuivant){
        quit.setCharacterSize(45);
        quit.setPosition(SCREEN_WIDTH * 0.75 - 5, SCREEN_HEIGHT * 0.85 - 5);
        boutonQSelect.setFillColor(Color(247,247,7));
        boutonQSelect.setOutlineColor(Color(175,200,7));
    }
    else {
        boutonQSelect.setFillColor(Color::White);
        boutonQSelect.setOutlineColor(Color(200,200,200));
    }

    fen.draw(boutonQSelect);
    fen.draw(quit);
}

void afficherTitre(char titreC[], Font f, RenderWindow &fen)
{
    Text titre;
    titre.setFont(f);
    titre.setCharacterSize(100);
    titre.setString(titreC);
    titre.setPosition(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT * 0.05);
    titre.setColor(Color(247, 76, 7));

    fen.draw(titre);
}

void dessinerBackground(char nomImg[], RenderWindow &fen)
{
    Texture textureFond; // On prend la texture du fond
    Sprite fond; // On crée un sprite
    if (!(textureFond.loadFromFile(nomImg))){
        printf("Impossible de charger l image : %s", nomImg);
        exit(1);
    }

    fond.setTexture(textureFond);
    fond.scale((float)SCREEN_WIDTH /(textureFond.getSize().x), (float)SCREEN_HEIGHT /(textureFond.getSize().y));
    fond.setPosition(0,0);

    fen.draw(fond);
}

int detectionRetourMenuPrecedent(int x, int y, int idMenu, int idMenuPrecedent, int selection)
{
    if (x >= SCREEN_WIDTH * 0.1 && x <= SCREEN_WIDTH * 0.1 + 175 && y >= SCREEN_HEIGHT * 0.85 && y <= SCREEN_HEIGHT * 0.85 + 50)
        selection = idMenuPrecedent;
    else
        selection = idMenu;

    //printf("x : %i, y : %i, p : %i, selection : %i\n", x, y, x >= SCREEN_WIDTH * 0.1 && x <= SCREEN_WIDTH * 0.1 + 175 && y >= SCREEN_HEIGHT * 0.85 && y <= SCREEN_HEIGHT * 0.85 + 50, selection);
    return selection;
}
int detectionChangementMenu(int x, int y, int idMenuPrecedent, int idMenu, int idMenuSuivant, int selection)
{
    if (x >= SCREEN_WIDTH * 0.1 && x <= SCREEN_WIDTH * 0.1 + 175 && y >= SCREEN_HEIGHT * 0.85 && y <= SCREEN_HEIGHT * 0.85 + 50)
        selection = idMenuPrecedent;
    else if (x >= SCREEN_WIDTH * 0.75 && x <= SCREEN_WIDTH * 0.75 + 175 && y >= SCREEN_HEIGHT * 0.85 && y <= SCREEN_HEIGHT * 0.85 + 50)
        selection = idMenuSuivant;
    else
        selection = idMenu;

    return selection;
}
void afficherCurseurJoueur(int idJoueur, int x, int y, Font f, RenderWindow &fen)
{

    Text joueur;

    char joueurC[3];
    sprintf(joueurC, "J%i", idJoueur);
    //printf("xfonction : %i, yfonction : %i\n", x,y);

    joueur.setFont(f);
    joueur.setCharacterSize(100);
    joueur.setString(joueurC);

    joueur.setPosition(x - 20, y - 50);
    if(idJoueur == 1)
        joueur.setColor(Color::Red);
    else
        joueur.setColor(Color::Blue);
    fen.draw(joueur);
}

void affichagePause(int selection, Font f, RenderWindow &fen)
{
    dessinerBackground(FOND_PRINCIPALE, fen);
    afficherTitre("Pause", f, fen);

    RectangleShape boutons[2];
    Text menus[2];
    menus[0].setString("Continuer");
    menus[1].setString("Menu Principale");

    menus[0].setPosition(SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 + 100, 210);
    menus[1].setPosition(SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 + 30, 200 + SCREEN_HEIGHT * 0.2 + 5);
    if(selection == 1)
    {

        menus[1].setPosition(SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 + 5, 200 + SCREEN_HEIGHT * 0.2 + 5);
    }
    else if(selection == 0)
    {
        menus[0].setPosition(SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 + 95, 210);
    }

    int i;
    for(i = 0; i < 2; i ++)
    {
        boutons[i].setSize(Vector2f(SCREEN_WIDTH * 0.3, SCREEN_HEIGHT * 0.1));
        boutons[i].setOutlineThickness(10);
        boutons[i].setPosition(SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2, 200 + SCREEN_HEIGHT * 0.2 * i);

        menus[i].setFont(f);
        menus[i].setColor(Color::Black);


        if(selection == i)
        {
            menus[i].setCharacterSize(45);
            boutons[i].setFillColor(Color(247,247,7));
            boutons[i].setOutlineColor(Color(200,200,7));
        }
        else
        {
            boutons[i].setFillColor(Color::White);
            boutons[i].setOutlineColor(Color(200,200,200));
        }

        fen.draw(boutons[i]);
        fen.draw(menus[i]);
    }
}

int choixPause(int x, int y)
{
    int selection = -1, i;

    for(i = 0; i < 2; i ++)
    {
        if(x >= SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 && x <= SCREEN_WIDTH / 2 - SCREEN_WIDTH * 0.3 / 2 + SCREEN_WIDTH * 0.3 && y >= 200 + SCREEN_HEIGHT * 0.2 * i && y <= 200 + SCREEN_HEIGHT * 0.2 * i + SCREEN_HEIGHT * 0.1)
            selection = i;
    }
    return selection;

}
