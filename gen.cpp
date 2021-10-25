#include "gen.h"
#include <time.h>

using namespace sf;

Sprite iconePersonnage [NOMBRE_DE_PERSO];
Texture textureIconePerso[NOMBRE_DE_PERSO];

Texture spriteSheet[NOMBRE_DE_PERSO];

RectangleShape barreVie1;
RectangleShape barreVie2;

int boucle = 0;

Fighter listeFighter[NOMBRE_DE_PERSO];
Fighter joueur1;
Fighter joueur2;

Map maps[NOMBRE_DE_MAPS];
int choixMusique = 0;

void jeu(int idperso1, int idperso2, int idMap, Font f,RenderWindow &app, Event &event)
{
    SoundBuffer goBuffer;
    if(!goBuffer.loadFromFile(SON_GO))
    {
        printf("Le sons %s n'est pas disponible", SON_GO);
        exit(SON_NON_EXISTANT);
    }

    Sound go;
    go.setBuffer(goBuffer);
    go.play();

    int inGame = 1;

    joueur1 = listeFighter[idperso1];
    joueur1.vie = MAX_HP;
    joueur2 = listeFighter[idperso2];
    joueur2.vie = MAX_HP;

    Sprite iconeJoueur1 = iconePersonnage[idperso1];
    iconeJoueur1.setPosition(0,0);
    iconeJoueur1.scale( 100.0 / textureIconePerso[idperso1].getSize().x, 100.0 / textureIconePerso[idperso1].getSize().y);

    Sprite iconeJoueur2 = iconePersonnage[idperso2];
    iconeJoueur2.setPosition(SCREEN_WIDTH - 100, 0);
    iconeJoueur2.scale( 100.0 / textureIconePerso[idperso2].getSize().x, 100.0 / textureIconePerso[idperso2].getSize().y);

    RectangleShape sol(Vector2f(SCREEN_WIDTH, 50));
    sol.setPosition(0,SCREEN_HEIGHT - 50);
    sol.setFillColor(Color::Transparent);

    RectangleShape lWall(Vector2f(50, SCREEN_HEIGHT));
    lWall.setPosition(0, 0);
    lWall.setFillColor(Color::Transparent);

    RectangleShape rWall(Vector2f(50, SCREEN_HEIGHT));
    rWall.setPosition(SCREEN_WIDTH-50,0);
    rWall.setFillColor(Color::Transparent);

    joueur1.canAnim = true;
    joueur2.canAnim = true;

    joueur1.fighter.setPosition(200, SCREEN_HEIGHT-200);
    joueur2.fighter.setPosition(1100,SCREEN_HEIGHT-200);

    joueur1.fighter.setScale(SCALE, SCALE);
    joueur2.fighter.setScale(-SCALE,SCALE);
    joueur2.stand.setScale(-SCALE,SCALE);


    const float maxY = 50.f;
    const Vector2f gravity(0.f, 2.f);
    Vector2f velocity(2.f, 5.f);

    int tempVieJ1, tempVieJ2;

    while(inGame)
    {
        tempVieJ1 = joueur1.vie;
        tempVieJ2 = joueur2.vie;

        while(app.pollEvent(event))
        {
            switch(event.type)
            {
            case Event::Closed:
                inGame = 0;
                app.close();
                break;
            case Event::MouseButtonPressed:
                if (joueur1.vie <= 0 || joueur2.vie <= 0)
                {
                    inGame = !detectionRetourMenuPrecedent(event.mouseButton.x, event.mouseButton.y, 1, 0, 1);
                }
            case Event::KeyPressed:
            default :
                break;
            }

        }

        input(&joueur1, sol, Keyboard::D, Keyboard::Q, Keyboard::Z, Keyboard::S, Keyboard::R, Keyboard::T);
        input(&joueur2, sol, Keyboard::Right, Keyboard::Left, Keyboard::Up, Keyboard::Down, Keyboard::Numpad0, Keyboard::Numpad2);

        hit(&joueur1, &joueur2);

        if(joueur1.vie <= 0 || joueur2.vie<= 0)
        {
            afficheGagnant(joueur1.vie, joueur2.vie, f, app);
            afficherBoutonQuitter(app, 0, f, 0, "Retour");
        }


        if (!isOnGround(joueur1.fighter, sol))
        {
            joueur1.fighter.move(gravity);
            joueur1.fighterAnim = IN_AIR;
        }
        else
        {
            if (joueur1.velocity.x == 0 && (joueur1.attackMode.attack == false && joueur1.attackMode.attackStand == false))
            {
                joueur1.fighterAnim = IDLE;
            }
        }

        if (!isOnGround(joueur2.fighter, sol))
        {
            joueur2.fighter.move(gravity);
            joueur2.fighterAnim = IN_AIR;
        }
        else
        {
            if (joueur2.velocity.x == 0 && (joueur2.attackMode.attack == false && joueur2.attackMode.attackStand == false))
            {
                joueur2.fighterAnim = IDLE;
            }

        }

        if (isOnGround(joueur1.fighter, lWall))
        {
            joueur1.fighter.move(Vector2f(11,0));
        }
        else if (isOnGround(joueur1.fighter, rWall))
        {
            joueur1.fighter.move(Vector2f(-11,0));
        }

        if (isOnGround(joueur2.fighter, lWall))
        {
            joueur2.fighter.move(Vector2f(11,0));
        }
        else if (isOnGround(joueur2.fighter, rWall))
        {
            joueur2.fighter.move(Vector2f(-11,0));
        }

        if (Keyboard::isKeyPressed(Keyboard::M))
            inGame = false;


        if (tempVieJ1 != joueur1.vie)
        {
            joueur1.fighterAnim = HIT;
            joueur1.controlAcces = false;
        }
        else
        {
            joueur1.controlAcces = true;
        }

        if (tempVieJ2 != joueur2.vie)
        {
            joueur2.fighterAnim = HIT;
            joueur2.controlAcces = false;
        }
        else
        {
            joueur2.controlAcces = true;
        }


        if (joueur2.vie > 0 )
        {
            animation(&joueur2.fighter, &joueur2.animation[joueur2.fighterAnim], &joueur2.fighterCl, true);
            animation(&joueur2.stand, &joueur2.animation[joueur2.standAnim], &joueur2.standCl, true);
        }
        else
        {
            joueur2.controlAcces = false;
            joueur2.isAlive = false;
            trueAnimation(&joueur2, FALL, 1);
        }
        if (joueur1.vie > 0 )
        {
            animation(&joueur1.fighter, &joueur1.animation[joueur1.fighterAnim], &joueur1.fighterCl, true);
            animation(&joueur1.stand, &joueur1.animation[joueur1.standAnim], &joueur1.standCl, true);
        }
        else
        {
            joueur1.controlAcces = false;
            joueur1.isAlive = false;
            trueAnimation(&joueur1, FALL, 1);
        }



        app.clear();
        affichageMap(idMap, app);
        app.draw(joueur1.fighter);
        app.draw(joueur2.fighter);
        affichageVie(joueur1.vie,joueur2.vie,app);
        app.draw(iconeJoueur1);
        app.draw(iconeJoueur2);
        if ((joueur1.guardActive == 1 || joueur1.standActive == 1) && joueur1.externStand == true)
        {
            app.draw(joueur1.stand);
        }
        if ((joueur2.guardActive == 1 || joueur2.standActive == 1) && joueur2.externStand == true)
        {
            app.draw(joueur2.stand);
        }
        app.display();

    }


}


void initImgs()
{
    barreVie2.setSize(Vector2f(250,10));
    barreVie2.setPosition(1053,100);
    barreVie2.setFillColor(Color :: Green);

    barreVie1.setSize(Vector2f(250,10));
    barreVie1.setPosition(0,100);
    barreVie1.setFillColor(Color :: Green);

    textureIconePerso[0].loadFromFile(PERSO_AVDOL);
    textureIconePerso[1].loadFromFile(PERSO_DIO);
    textureIconePerso[2].loadFromFile(PERSO_JOSEPH);
    textureIconePerso[3].loadFromFile(PERSO_JOSUKE);
    textureIconePerso[4].loadFromFile(PERSO_JOTARO);
    textureIconePerso[5].loadFromFile(PERSO_KAKYOIN);
    textureIconePerso[6].loadFromFile(PERSO_POLNAREFF);
    textureIconePerso[7].loadFromFile(PERSO_HOLHORSE);

    int i;
    for (i = 0; i < NOMBRE_DE_PERSO; i++)
        iconePersonnage[i].setTexture(textureIconePerso[i]);

}

void affichageMap(int idMap, RenderWindow &app)
{
    animation(&maps[idMap - 1].mapSprite, &maps[idMap - 1].animation, &maps[idMap - 1].montreSprite, false);
    app.draw(maps[idMap - 1].mapSprite);

}
void affichageVie(int vieJ1, int vieJ2, RenderWindow &app)
{
    float longeurBarreJ1 = vieJ1 * 250.0 / MAX_HP;
    float longeurBarreJ2 = vieJ2 * 250.0 / MAX_HP;

    float pourcentageRougeJ1, pourcentageVertJ1;
    pourcentageVertJ1 = vieJ1 * 255 / MAX_HP;
    pourcentageRougeJ1 = 255 - pourcentageVertJ1;

    float pourcentageRougeJ2, pourcentageVertJ2;
    pourcentageVertJ2 = vieJ2 * 255 / MAX_HP;
    pourcentageRougeJ2 = 255 - pourcentageVertJ2;

    barreVie1.setSize(Vector2f(longeurBarreJ1, 10));
    barreVie2.setSize(Vector2f(longeurBarreJ2, 10));

    barreVie1.setFillColor(Color(pourcentageRougeJ1, pourcentageVertJ1, 0));
    barreVie2.setFillColor(Color(pourcentageRougeJ2, pourcentageVertJ2, 0));

    barreVie2.setPosition(SCREEN_WIDTH - longeurBarreJ2, 100);

    app.draw(barreVie1);
    app.draw(barreVie2);
}
void afficheGagnant(int vieJ1, int vieJ2, Font f, RenderWindow &app)
{

    Text KO;
    KO.setCharacterSize(50);
    KO.setColor(Color::Red);
    KO.setFont(f);
    KO.setString("KO");
    KO.setPosition(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 4);

    Text vainqueur;
    vainqueur.setCharacterSize(50);
    vainqueur.setColor(Color::Red);
    vainqueur.setFont(f);
    if (vieJ1 <= 0 && vieJ2 <= 0)
    {
        vainqueur.setString("Egalite");
        vainqueur.setPosition(SCREEN_WIDTH / 2 - 50 * 3.5, SCREEN_HEIGHT / 2);
    }
    else
    {
        vainqueur.setPosition(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
        if(vieJ1 <= 0)
            vainqueur.setString("J2 Gagne");
        else
            vainqueur.setString("J1 Gagne");
    }
    app.draw(KO);
    app.draw(vainqueur);
}

void animation(Sprite *sprite, Anim *coord, Clock *clock, bool resetOrigin)
{
    if(clock->getElapsedTime().asMilliseconds()  > coord-> delayMillis)
    {
        if(coord->animCoord.left > (coord->frameLimit-2)*(coord->animCoord.width)+coord->offsetX)
        {
            coord->animCoord.left = coord->offsetX;
        }

        else
            coord->animCoord.left += coord->animCoord.width;
        clock->restart();
    }
    sprite->setTextureRect(coord->animCoord);
    if (resetOrigin)
        sprite->setOrigin(coord->animCoord.width/2, coord->animCoord.height);
}

void trueAnimation(Fighter *fighterPointer, int animation, int nbAnim)
{
    Fighter theFighter = *fighterPointer;
    if(theFighter.canAnim)
    {
        //system("PAUSE");
        if(theFighter.animBegin)
        {
            theFighter.trueAnimCl.restart();
            theFighter.fighterCl.restart();
            //theFighter.controlAcces = false;
            theFighter.animBegin = false;
        }

        float dureeAnim;
        dureeAnim = theFighter.animation[animation].delayMillis*theFighter.animation[animation].frameLimit;

        if (theFighter.trueAnimCl.getElapsedTime().asMilliseconds() < dureeAnim*nbAnim)
        {
            if(theFighter.fighterCl.getElapsedTime().asMilliseconds()  > theFighter.animation[animation].delayMillis)
            {
                if(theFighter.animation[animation].animCoord.left > (theFighter.animation[animation].frameLimit-2)*(theFighter.animation[animation].animCoord.width)+theFighter.animation[animation].offsetX)
                {
                    theFighter.animation[animation].animCoord.left = theFighter.animation[animation].offsetX;
                }

                else
                    theFighter.animation[animation].animCoord.left += theFighter.animation[animation].animCoord.width;

                theFighter.fighterCl.restart();
            }
            if (theFighter.attackQualification.attackBurst == true && theFighter.attackQualification.special == true)
            {
                theFighter.stand.setTextureRect(theFighter.animation[animation].animCoord);
                theFighter.fighter.setOrigin(theFighter.animation[animation].animCoord.width/2, theFighter.animation[animation].animCoord.height);
            }
            else
            {
                theFighter.fighter.setTextureRect(theFighter.animation[animation].animCoord);
                theFighter.fighter.setOrigin(theFighter.animation[animation].animCoord.width/2, theFighter.animation[animation].animCoord.height);
            }

        }
        else
        {
            theFighter.canAnim = false;
            theFighter.animBegin = true;
            //theFighter.controlAcces = true;
        }
    }
    *fighterPointer = theFighter;
}

void hit(Fighter *fighterOne, Fighter *fighterTwo)
{
    Fighter fighterOneTemp = *fighterOne;
    Fighter fighterTwoTemp = *fighterTwo;
    int degatsOne = 0, degatsTwo = 0;

    if(fighterOneTemp.isAlive && fighterTwoTemp.isAlive)
    {
        if (fighterOneTemp.attackMode.attack == true || fighterOneTemp.attackMode.attackStand == true || fighterTwoTemp.attackMode.attack == true || fighterTwoTemp.attackMode.attackStand == true)
        {

            if (fighterOneTemp.externStand == true) //Si le joueur 1 (stand détaché) frappe le joueur 2 avec une attaque spéciale
            {
                if (fighterOneTemp.standActive == true && fighterOneTemp.attackQualification.special == true && fighterOneTemp.attackMode.attackStand == true)
                {
                    if(fighterOneTemp.stand.getGlobalBounds().intersects(fighterTwoTemp.fighter.getGlobalBounds()))
                    {
                        degatsTwo = DEGATS_SUPER;
                        fighterTwoTemp.fighter.move(fighterOneTemp.fighter.getScale().x/2*RECUL, 0);
                    }
                }
            }

            if (fighterTwoTemp.externStand == true) //Si le joueur 2 (stand détaché) frappe le joueur 1 avec une attaque spéciale
            {
                if (fighterTwoTemp.standActive == true && fighterTwoTemp.attackQualification.special == true && fighterTwoTemp.attackMode.attackStand == true)
                {
                    if(fighterTwoTemp.stand.getGlobalBounds().intersects(fighterOneTemp.fighter.getGlobalBounds()))
                    {
                        degatsOne = DEGATS_SUPER;
                        fighterOneTemp.fighter.move(fighterTwoTemp.fighter.getScale().x/2*RECUL, 0);
                    }
                }
            }

            if (fighterOneTemp.externStand == false) // Attaques spéciales stand attaché
            {
                if (fighterOneTemp.attackQualification.special == true && fighterOneTemp.attackMode.attack == true)
                {
                    if(fighterOneTemp.fighter.getGlobalBounds().intersects(fighterTwoTemp.fighter.getGlobalBounds()))
                    {
                        degatsTwo = DEGATS_SUPER;
                        fighterTwoTemp.fighter.move(fighterOneTemp.fighter.getScale().x/2*RECUL, 0);
                    }
                }
            }

            if (fighterTwoTemp.externStand == false) // Attaques spéciales stand attaché
            {
                if (fighterTwoTemp.attackQualification.special == true && fighterTwoTemp.attackMode.attack == true)
                {
                    if(fighterTwoTemp.fighter.getGlobalBounds().intersects(fighterOneTemp.fighter.getGlobalBounds()))
                    {
                        degatsOne = DEGATS_SUPER;
                        fighterOneTemp.fighter.move(fighterTwoTemp.fighter.getScale().x/2*RECUL, 0);
                    }
                }
            }
            if (fighterOneTemp.standActive == false && fighterOneTemp.attackQualification.normal == true && fighterOneTemp.attackMode.attack == true)
            {
                if(fighterOneTemp.fighter.getGlobalBounds().intersects(fighterTwoTemp.fighter.getGlobalBounds()))
                {
                    degatsTwo = DEGATS_NORMAL;
                    fighterTwoTemp.fighter.move(fighterOneTemp.fighter.getScale().x/2*RECUL_COUP_NORMAL, 0);
                }
            }
            if (fighterTwoTemp.standActive == false && fighterTwoTemp.attackQualification.normal == true && fighterTwoTemp.attackMode.attack == true)
            {
                if(fighterTwoTemp.fighter.getGlobalBounds().intersects(fighterOneTemp.fighter.getGlobalBounds()))
                {
                    degatsOne = DEGATS_NORMAL;
                    fighterOneTemp.fighter.move(fighterTwoTemp.fighter.getScale().x/2*RECUL_COUP_NORMAL, 0);
                }
            }

        }
        if (fighterOneTemp.guardActive == true)
            degatsOne = degatsOne/2;
        if (fighterTwoTemp.guardActive == true)
            degatsTwo = degatsTwo/2;
    }
    else
    {
        degatsOne = 0;
        degatsTwo = 0;
    }


    fighterOneTemp.vie -= degatsOne;
    fighterTwoTemp.vie -= degatsTwo;
    *fighterOne = fighterOneTemp;
    *fighterTwo = fighterTwoTemp;
}

bool isOnGround(Sprite object, RectangleShape ground)
{
    bool results = false;
    if (object.getGlobalBounds().intersects(ground.getGlobalBounds()))
    {
        results = true;
    }
    return results;
}

void input(Fighter *player, RectangleShape sol,Keyboard::Key moveRight, Keyboard::Key moveLeft, Keyboard::Key jump, Keyboard::Key guard, Keyboard::Key attack, Keyboard::Key specialAttack)
{
    Fighter theFighter = *player;
    const float maxY = 50.f;
    const Vector2f gravity(0.f, 8.f);
    Vector2f velocity(2.f, 5.f);

    if (theFighter.controlAcces == true)
    {
        if (Keyboard::isKeyPressed(moveRight))
        {
            theFighter.velocity.x = PAS;
            theFighter.fighterAnim = MOVE;
            theFighter.fighter.setScale(SCALE,SCALE);
            if (theFighter.externStand == true)
            {
                theFighter.stand.setScale(SCALE,SCALE);
                //theFighter.standOffset = STAND_OFFSET_MINUS;
            }
            theFighter.guardActive = false;
            theFighter.attackQualification.special = false;
            theFighter.attackMode.attack = false;
            if(theFighter.externStand == true)
            {
                theFighter.standActive = false;
                theFighter.attackMode.attackStand = false;
            }
        }
        else if (!Keyboard::isKeyPressed(moveRight))
        {
            theFighter.velocity.x = 0;
        }
        if (Keyboard::isKeyPressed(moveLeft))
        {
            theFighter.velocity.x = - PAS;
            theFighter.fighterAnim = MOVE;
            theFighter.fighter.setScale(-SCALE,SCALE);
            if (theFighter.externStand == true)
            {
                theFighter.stand.setScale(-SCALE,SCALE);
                //theFighter.standOffset = STAND_OFFSET_MINUS;
            }
            theFighter.guardActive = false;
            theFighter.attackQualification.special = false;
            theFighter.attackMode.attack = false;
            if(theFighter.externStand == true)
            {
                theFighter.standActive = false;
                theFighter.attackMode.attackStand = false;
            }
        }
        if (Keyboard::isKeyPressed(attack) && theFighter.velocity.x == 0 && theFighter.velocity.y == 0)
        {
            theFighter.guardActive = false;
            theFighter.attackQualification.normal = true;
            theFighter.attackMode.attack = true;
            theFighter.fighterAnim = NORMAL_ATTACK_TWO;
        }
        else if (!Keyboard::isKeyPressed(attack) || theFighter.velocity.x != 0 || theFighter.velocity.y != 0)
        {
            theFighter.guardActive = false;
            theFighter.attackMode.attack = false;
            theFighter.attackQualification.normal = false;
        }
        if (Keyboard::isKeyPressed(specialAttack) && theFighter.velocity.x == 0 && theFighter.velocity.y == 0)
        {
            theFighter.velocity.x = 0;
            theFighter.guardActive = false;
            theFighter.attackQualification.special = true;

            if (theFighter.externStand == true)
            {
                theFighter.fighterAnim = IDLE;
                theFighter.standAnim = SPECIAL;
                theFighter.standActive = true;
                theFighter.attackMode.attackStand = true;
            }
            else if (theFighter.externStand == false)
            {
                theFighter.fighterAnim = SPECIAL;
                theFighter.attackMode.attack = true;
            }
        }
        else if (!Keyboard::isKeyPressed(specialAttack) || theFighter.velocity.x != 0 || theFighter.velocity.y != 0)
        {
            theFighter.attackQualification.special = false;
            if(theFighter.externStand == true)
            {
                theFighter.standActive = false;
                theFighter.attackMode.attackStand = false;
            }
        }
        if (Keyboard::isKeyPressed(jump))
        {
            theFighter.fighter.move(0, theFighter.pasSaut);
            if(theFighter.pasSaut<VITESSE_SAUT_MAX)
                theFighter.pasSaut += GRAVITY;
        }
    }
    else
    {
        printf("Acces a moi desactive\n");
        theFighter.velocity.x = 0;
    }

    theFighter.fighter.move(theFighter.velocity);

    if (theFighter.externStand == true)
    {
        theFighter.standOffset = Vector2f(theFighter.animation[theFighter.fighterAnim].animCoord.width/2, theFighter.animation[theFighter.fighterAnim].animCoord.height/2);
        theFighter.stand.setPosition(theFighter.fighter.getPosition().x+(theFighter.standOffset.x*2)*theFighter.fighter.getScale().x, theFighter.fighter.getPosition().y);
    }

    if(theFighter.fighter.getPosition().y >= sol.getPosition().y)
        theFighter.pasSaut = SAUT_MAX;
    if(theFighter.fighter.getPosition().y < sol.getPosition().y)
    {
        theFighter.fighter.move(0, theFighter.pasSaut);
        if(theFighter.pasSaut<VITESSE_SAUT_MAX)
            theFighter.pasSaut += GRAVITY;
    }

    printf("SECOND VELOCITY : %i\n", theFighter.velocity.x);

    *player = theFighter;
}

Anim setAnimation(IntRect coord, int frameLimit, float delayInterFrames)
{
    Anim results;
    results.animCoord = coord;
    results.delayMillis = delayInterFrames;
    results.frameLimit = frameLimit;
    results.offsetX = coord.left;
    return results;
}

void initFighters()
{
    printf("Initialisation fighter : ");

    sprintf(listeFighter[0].nom, "Avdol");
    sprintf(listeFighter[1].nom, "Dio");
    sprintf(listeFighter[2].nom, "Joseph");
    sprintf(listeFighter[3].nom, "Josuke");
    sprintf(listeFighter[4].nom, "Jotaro");
    sprintf(listeFighter[5].nom, "Kakyoin");
    sprintf(listeFighter[6].nom, "Polnareff");
    sprintf(listeFighter[7].nom, "Hol Horse");

    listeFighter[0].externStand = true;
    listeFighter[1].externStand = true;
    listeFighter[2].externStand = false;
    listeFighter[3].externStand = true;
    listeFighter[4].externStand = true;
    listeFighter[5].externStand = false;
    listeFighter[6].externStand = false;
    listeFighter[7].externStand = false;


    if(!(listeFighter[0].fighterText.loadFromFile(TEXTURE_AVDOL)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_AVDOL);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[1].fighterText.loadFromFile(TEXTURE_DIO)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_DIO);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[2].fighterText.loadFromFile(TEXTURE_JOSEPH)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_JOSEPH);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[3].fighterText.loadFromFile(TEXTURE_JOSUKE)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_JOSUKE);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[4].fighterText.loadFromFile(TEXTURE_JOTARO)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_JOTARO);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[5].fighterText.loadFromFile(TEXTURE_KAKYOIN)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_KAKYOIN);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[6].fighterText.loadFromFile(TEXTURE_POLNAREFF)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_POLNAREFF);
        exit(IMG_NON_EXISTANTE);
    }

    if(!(listeFighter[7].fighterText.loadFromFile(TEXTURE_HOLHORSE)))
    {
        printf("l'image %s n est pas disponible", TEXTURE_HOLHORSE);
        exit(IMG_NON_EXISTANTE);
    }

    int i;

    for(i = 0; i < NOMBRE_DE_PERSO; i ++)
    {
        listeFighter[i].fighter.setTexture(listeFighter[i].fighterText);
        //listeFighter[i].spriteSheet.setTexture(listeFighter[i].fighterText);
        listeFighter[i].stand.setTexture(listeFighter[i].fighterText);
        listeFighter[i].fighter.scale(SCALE,SCALE);
        listeFighter[i].stand.scale(SCALE,SCALE);
    }
    /*
    [0] Idle
    [1] Move
    [2] Guard
    [3] StandGuard
    [4] Jump start
    [5] Jump in ai
    [6] Jump end
    [7] NorlmalAttack1
    [8] NorlmalAttack2
    [9] UpSpecial
    [10] Special
    [11 DownSpecial
    [12] Get Hit
    [13] Fall by hit
    */

    // Avdol
    listeFighter[0].animation[IDLE] = setAnimation(IntRect(26,31,44,64), 3, 250.0f);
    listeFighter[0].animation[MOVE] = setAnimation(IntRect(14,192,54,63), 8, 250.0f);
    listeFighter[0].animation[GUARD] = setAnimation(IntRect(25,110,63,59), 2, 250.0f);
    //listeFighter[0].animation[3] = setAnimation(IntRect(0,0,33,59), 4, 250.0f);
    listeFighter[0].animation[JUMP_START] = setAnimation(IntRect(32,299,45,69), 2, 250.0f);
    listeFighter[0].animation[IN_AIR] = setAnimation(IntRect(126,297,45,71), 2, 250.0f);
    listeFighter[0].animation[JUMP_END] = setAnimation(IntRect(216,299,45,69), 2, 250.0f);
    listeFighter[0].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(2,667,68,65), 4, 250.0f);
    listeFighter[0].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(7,578,57,75), 4, 250.0f);
    listeFighter[0].animation[UP_SPECIAL] = setAnimation(IntRect(12,1013,67,121), 7, 250.0f);
    listeFighter[0].animation[SPECIAL] = setAnimation(IntRect(25,916,65,61), 4, 250.0f);
    listeFighter[0].animation[DOWN_SPECIAL] = setAnimation(IntRect(28,1197,107,92), 7, 250.0f);
    listeFighter[0].animation[HIT] = setAnimation(IntRect(162,402,68,59), 7, 250.0f);
    listeFighter[0].animation[FALL] = setAnimation(IntRect(10,400,68,61), 9, 250.0f);
    listeFighter[0].animation[BATTU] = setAnimation(IntRect(10,400,68,61), 1, 250.0f);

    //Dio
    listeFighter[1].animation[IDLE] = setAnimation(IntRect(1, 7, 25, 64), 10, 100.0); //idle
    listeFighter[1].animation[MOVE] = setAnimation(IntRect(2, 102, 53, 47), 4, 100.0); //walk
    listeFighter[1].animation[GUARD] = setAnimation(IntRect(23, 1635, 28, 65), 3, 100.0); //Guard
    // listeFighter[1].animation[3] = setAnimation(IntRect(0, 0, 0, 0), 0, 100.0) //StandGuard
    listeFighter[1].animation[JUMP_START] = setAnimation(IntRect(4, 221, 48, 71), 3, 100.0); //start jump
    listeFighter[1].animation[IN_AIR] = setAnimation(IntRect(100, 221, 48, 71), 1, 100.0); //jump in air
    listeFighter[1].animation[JUMP_END] = setAnimation(IntRect(196, 221, 48, 71), 4, 100.0); //end jump
    listeFighter[1].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(1, 439, 60, 63), 9, 100.0); //attaque1
    listeFighter[1].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(1, 1316, 60, 65), 9, 100.0); //attaque2
    listeFighter[1].animation[UP_SPECIAL] = setAnimation(IntRect(1, 840, 68, 62), 6, 100.0); //specialHaut
    listeFighter[1].animation[SPECIAL] = setAnimation(IntRect(1, 1494, 101, 86), 7, 100.0); //Special
    listeFighter[1].animation[DOWN_SPECIAL] = setAnimation(IntRect(1, 1129, 69, 58), 5, 100.0); //SpecialBas
    listeFighter[1].animation[HIT] = setAnimation(IntRect(123, 3328, 61, 66), 2, 100.0); //EstTapé
    listeFighter[1].animation[FALL] = setAnimation(IntRect(245, 3328, 61, 66), 5, 100.0); //Tombe
    listeFighter[1].animation[BATTU] = setAnimation(IntRect(306, 3328, 61, 66), 1, 100.0); //est a terre

    //Joseph
    listeFighter[2].animation[0] = setAnimation(IntRect(0, 0, 27, 63), 5, 100.0f);
    listeFighter[2].animation[1] = setAnimation(IntRect(0, 125, 52, 60), 8, 100.0f);
    listeFighter[2].animation[2] = setAnimation(IntRect(0, 63, 39, 62), 4, 100.0f);
// listeFighter[2].animation[3] = setAnimation(IntRect(0, 0, 33, 39), 4, 100.0f);
    listeFighter[2].animation[JUMP_START] = setAnimation(IntRect(0, 185, 41, 65), 2, 200.0f);
    listeFighter[2].animation[IN_AIR] = setAnimation(IntRect(41, 185, 41, 65), 1, 100.0f);
    listeFighter[2].animation[JUMP_END] = setAnimation(IntRect(82, 185, 41, 65), 3, 200.0f);
    listeFighter[2].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(0, 550, 58, 64), 5, 100.0f);
    listeFighter[2].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(0, 781, 62, 60), 5, 100.0f);
    listeFighter[2].animation[UP_SPECIAL] = setAnimation(IntRect(0, 620, 51, 67), 8, 100.0f);
    listeFighter[2].animation[SPECIAL] = setAnimation(IntRect(0, 1325, 97, 60), 7, 100.0f);
    listeFighter[2].animation[DOWN_SPECIAL] = setAnimation(IntRect(0, 1221, 55, 75), 8, 100.0f);
    listeFighter[2].animation[HIT] = setAnimation(IntRect(0, 308, 49, 64), 4, 100.0f);
    listeFighter[2].animation[FALL] = setAnimation(IntRect(0, 418, 68, 57), 7, 100.0f);
    listeFighter[2].animation[BATTU] = setAnimation(IntRect(136, 453, 68, 57), 1, 100.0f);

    //Josuke
    listeFighter[3].animation[IDLE] = setAnimation(IntRect(7, 11, 31, 58), 4, 100.0f);
    listeFighter[3].animation[MOVE] = setAnimation(IntRect(31, 156, 47, 44), 8, 100.0f);
    listeFighter[3].animation[GUARD] = setAnimation(IntRect(15, 88, 57, 56), 4, 100.0f);
// listeFighter[3].animation[3] = setAnimation(IntRect(0, 0, 33, 39), 4, 100.0f);
    listeFighter[3].animation[JUMP_START] = setAnimation(IntRect(32, 233, 39, 55), 2, 100.0f);
    listeFighter[3].animation[IN_AIR] = setAnimation(IntRect(110, 233, 39, 55), 1, 100.0f);
    listeFighter[3].animation[JUMP_END] = setAnimation(IntRect(110, 233, 39, 55), 2, 100.0f);
    listeFighter[3].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(43, 441, 46, 48), 4, 100.0f);
    listeFighter[3].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(28, 567, 42, 57), 6, 100.0f);
    listeFighter[3].animation[UP_SPECIAL] = setAnimation(IntRect(76, 733, 68, 62), 5, 100.0f);
    listeFighter[3].animation[SPECIAL] = setAnimation(IntRect(0, 1165, 89, 61), 5, 100.0f);
    listeFighter[3].animation[DOWN_SPECIAL] = setAnimation(IntRect(80, 902, 74, 62), 4, 100.0f);
    listeFighter[3].animation[HIT] = setAnimation(IntRect(30, 307, 44, 52), 2, 100.0f);
    listeFighter[3].animation[FALL] = setAnimation(IntRect(121, 315, 59, 47), 5, 100.0f);
    listeFighter[3].animation[BATTU] = setAnimation(IntRect(239, 315, 59, 47), 1, 100.0f);

    // Jotaro
    listeFighter[4].animation[IDLE] = setAnimation(IntRect(6,34,37,57), 6, 180.0f); //Idle
    listeFighter[4].animation[MOVE] = setAnimation(IntRect(647,34,46,57), 8, 70.0f); //Run
    listeFighter[4].animation[GUARD] = setAnimation(IntRect(201,141,35,56), 1, 600.0f); //Guard
    listeFighter[4].animation[STAND_GUARD] = setAnimation(IntRect(284,135,38,62), 2, 600.0f); //Stand Guard
    listeFighter[4].animation[JUMP_START] = setAnimation(IntRect(422,140,44,57), 2, 50.0f);// Jump start
    listeFighter[4].animation[IN_AIR] = setAnimation(IntRect(466, 140, 44, 57), 2, 100.f); //In air
    listeFighter[4].animation[JUMP_END] = setAnimation(IntRect(598,140,44,57), 4, 50.0f); //Jump end
    listeFighter[4].animation[SPECIAL] = setAnimation(IntRect(208,936,101,66), 6, 80.0f); //Special
    listeFighter[4].animation[HIT] = setAnimation(IntRect(8,234, 40,56), 4, 80.0f); // Hit
    listeFighter[4].animation[FALL] = setAnimation(IntRect(228,245, 64,45), 7, 80.0f); // Fall
    listeFighter[4].animation[BATTU] = setAnimation(IntRect(356,245, 64,45), 1, 80.0f); // u are ded not a big suprise
    listeFighter[4].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(1,711,53,59), 4, 80);

    // Kakyoin
    listeFighter[5].animation[IDLE] = setAnimation(IntRect(0, 0, 45, 57), 4, 100.0f);
    listeFighter[5].animation[MOVE] = setAnimation(IntRect(0, 166, 46, 56), 8, 100.0f);
    listeFighter[5].animation[GUARD] = setAnimation(IntRect(0, 92, 46, 58), 4, 100.0f);
// listFighter[5].animation[3] = setAnimation(IntRect(0, 0, 33, 39), 4, 100.0f);
    listeFighter[5].animation[JUMP_START] = setAnimation(IntRect(0, 270, 44, 59), 3, 100.0f);
    listeFighter[5].animation[IN_AIR] = setAnimation(IntRect(132, 270, 44, 59), 2, 100.0f);
    listeFighter[5].animation[JUMP_END] = setAnimation(IntRect(220, 270, 44, 59), 3, 100.0f);
    listeFighter[5].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(3, 457, 53, 58), 4, 100.0f);
    listeFighter[5].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(1, 526, 53, 58), 5, 100.0f);
    listeFighter[5].animation[UP_SPECIAL] = setAnimation(IntRect(4, 664, 74, 75), 4, 100.0f);
    listeFighter[5].animation[SPECIAL] = setAnimation(IntRect(4, 774, 162, 62), 4, 100.0f);
    listeFighter[5].animation[DOWN_SPECIAL] = setAnimation(IntRect(10, 857, 118, 83), 2, 100.0f);
    listeFighter[5].animation[HIT] = setAnimation(IntRect(1, 369, 64, 54), 3, 100.0f);
    listeFighter[5].animation[FALL] = setAnimation(IntRect(263, 369, 65, 54), 7, 100.0f);
    listeFighter[5].animation[BATTU] = setAnimation(IntRect(393, 369, 65, 54), 1, 100.0f);

    // Polnareff
    listeFighter[6].animation[IDLE] = setAnimation(IntRect(0,0,33,59), 4, 250.0f);
    listeFighter[6].animation[MOVE] = setAnimation(IntRect(0,181,52,55), 8, 250.0f);
    listeFighter[6].animation[GUARD] = setAnimation(IntRect(0,120,59,61), 4, 250.0f);
    //listeFighter[6].animation[3] = setAnimation(IntRect(0,0,33,59), 4, 250.0f);
    listeFighter[6].animation[JUMP_START] = setAnimation(IntRect(0,59,48,61), 7, 250.0f);
    //listeFighter[6].animation[5] = setAnimation(IntRect(0,0,33,59), 4, 250.0f);
    //listeFighter[6].animation[6] = setAnimation(IntRect(0,0,33,59), 4, 250.0f);
    listeFighter[6].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(0,591,67,57), 5, 250.0f);
    listeFighter[6].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(0,530,61,61), 4, 250.0f);
    listeFighter[6].animation[UP_SPECIAL] = setAnimation(IntRect(0,428,105,102), 3, 250.0f);
    listeFighter[6].animation[SPECIAL] = setAnimation(IntRect(0,356,106,72), 3, 250.0f);
    listeFighter[6].animation[DOWN_SPECIAL] = setAnimation(IntRect(0,648,87,72), 4, 250.0f);
    listeFighter[6].animation[HIT] = setAnimation(IntRect(0,298,46,58), 2, 250.0f);
    listeFighter[6].animation[FALL] = setAnimation(IntRect(0,242,71,55), 7, 250.0f);
    listeFighter[6].animation[BATTU] = setAnimation(IntRect(142,242,71,55), 1, 250.0f);

    //Hol Horse
    listeFighter[7].animation[IDLE] = setAnimation(IntRect(15, 27, 41, 62), 3, 100.0); //idle
    listeFighter[7].animation[MOVE] = setAnimation(IntRect(3, 105, 53, 59), 8, 100.0); //walk
    listeFighter[7].animation[GUARD] = setAnimation(IntRect(64, 272, 37, 60), 1, 100.0); //Guard
// listeFighter[7].animation[3] = setAnimation(IntRect(0, 0, 0, 0), 0, 100.0); //StandGuard
    listeFighter[7].animation[JUMP_START] = setAnimation(IntRect(12, 181, 41, 64), 3, 100.0); //start jump
    listeFighter[7].animation[IN_AIR] = setAnimation(IntRect(135, 181, 41, 64), 2, 100.0); //jump in air
    listeFighter[7].animation[JUMP_END] = setAnimation(IntRect(176, 181, 41, 64), 4, 100.0); //end jump
    listeFighter[7].animation[NORMAL_ATTACK_ONE] = setAnimation(IntRect(12, 476, 46, 63), 5, 100.0); //attaque1
    listeFighter[7].animation[NORMAL_ATTACK_TWO] = setAnimation(IntRect(2, 940, 56, 68), 6, 100.0); //attaque2
    listeFighter[7].animation[UP_SPECIAL] = setAnimation(IntRect(36, 1046, 60, 82), 11, 100.0); //specialHaut
    listeFighter[7].animation[SPECIAL] = setAnimation(IntRect(6, 1156, 191, 66), 8, 100.0); //Special
    listeFighter[7].animation[DOWN_SPECIAL] = setAnimation(IntRect(0, 1408, 88, 71), 5, 100.0); //SpecialBas
    listeFighter[7].animation[HIT] = setAnimation(IntRect(1, 2171, 68, 63), 3, 100.0); //EstTapé
    listeFighter[7].animation[FALL] = setAnimation(IntRect(205, 2171, 68, 63), 8, 100.0); //Tombe
    listeFighter[7].animation[BATTU] = setAnimation(IntRect(409, 2171, 68, 63), 1, 100.0); //est a terre

    printf("100%\n");
}

void initMaps()
{
    printf("Initialisation des maps : ");
    maps[0].mapTexture.loadFromFile(NOM_ARENE1);
    maps[1].mapTexture.loadFromFile(NOM_ARENE2);
    maps[2].mapTexture.loadFromFile(NOM_ARENE3);
    maps[3].mapTexture.loadFromFile(NOM_ARENE4);
    maps[4].mapTexture.loadFromFile(NOM_ARENE5);

    maps[0].animation = setAnimation(IntRect(0,0,800,336), 8, 85.0f);
    maps[1].animation = setAnimation(IntRect(0,0,768,384), 8, 85.0f);
    maps[2].animation = setAnimation(IntRect(0,0,752,224), 8, 85.0f);
    maps[3].animation = setAnimation(IntRect(0,0,768,256), 8, 85.0f);
    maps[4].animation = setAnimation(IntRect(0,0,752,224), 8, 85.0f);

    int i;
    for(i = 0; i < NOMBRE_DE_MAPS; i ++)
    {
        maps[i].mapSprite.setTexture(maps[i].mapTexture);
        maps[i].animation.delayMillis = 85.0f;
        maps[i].animation.offsetX = 0;
        maps[i].mapSprite.setPosition(0,0);
        maps[i].mapSprite.scale((float)SCREEN_WIDTH / maps[i].animation.animCoord.width,(float) SCREEN_HEIGHT / maps[i].animation.animCoord.height);
    }
    printf("100%\n");


}

int pause(Font f, RenderWindow &fen)
{
    bool enPause = true;
    int enJeu = 1, selection = -1;
    while(enPause)
    {

        Event event;
        while(fen.pollEvent(event))
        {
            switch(event.type)
            {
            case Event::Closed:
                enPause = false;
                break;
            case Event::KeyPressed:
                if(event.key.code == Keyboard::Escape)
                    enPause = false;
                break;
            case Event::MouseMoved:
                selection = choixPause(event.mouseMove.x, event.mouseMove.y);
                break;
            case Event::MouseButtonPressed:
                if(selection != -1)
                {
                    enPause = 0;
                    if (selection == 1)
                        enJeu = 0;
                }

            default:
                break;
            }
            fen.clear();
            affichagePause(selection, f, fen);

            fen.display();

        }
    }
    return enJeu;
}
