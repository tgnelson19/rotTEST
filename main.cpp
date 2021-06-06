/// Programmer : Tyler Nelson
/// Date : March 10th 2021
/// Program : rotTest

/// This project is called Rot*Test* for a reason, it is simply a proof of concept for various systems that I want
/// to implement in my final project that I am creating based upon one of my favorite computer games RotMG.
/// If there are inefficiencies in my system they will be worked out in the final product, and systems that have
/// been displayed but not implemented are meant to just be there as a placeholder, i.e. the inventory system.
///
/// Overall, don't expect much from this initial design, but also, don't expect too little.

/// Goals Accomplished :
/// 1. Movement System (Smooth movement, speed logic, changing sprite texture relative to direction, shooting animation)
/// 2. Background movement (Changes in opposite direction relative to character movement.)
/// 3. Minimap system (Changes in relative direction to background, player indicator is precise.)
/// 4. Projectile system (Speed set to unit vector in direction of click, rotated to direction, projectile logic implemented.)
/// 5. Basic chasing enemy implemented
/// 6. Basic bounding box system implemented
/// 7. Basic health system implemented
/// 8. Basic mana system implemented

/// Goals to be accomplished :
/// Upgrade system
/// Class/Object system redesign to create modular code for
///     -New Map(s)


#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "Projectile.h"
#include "Character.h"

using namespace sf;

int main() {

    RenderWindow window(VideoMode(1500, 800), "RotTest"); ///Main window initialized

    window.setFramerateLimit(60);

    View minimap; ///Initializing minimap

    minimap.setViewport(
            FloatRect(0.8f, 0.f, 0.2f, 0.35f)); ///Creating minimap viewport (how much of the whole window it takes up)

    ///Initializing Textures

    Texture bg, Arrow, SArcherSpriteSheet, Inventory, Stats, CrystalBoi, GoldTexture, upgradedBowShot;

    ///Loading textures in
    ///
    ///NONE OF THE SPRITES ARE MY OWN
    ///THIS PROGRAM IS NOT FOR SALE OR DISTRIBUTION FOR PROFIT
    ///ALL CREATORS WILL BE CITED
    ///
    ///See https://www.tigsource.com/2010/02/07/assemblee-competition-results/ for source of sprites

    /// Oryx -> oryxdesignlab.com
    /// Wildshadow Studios -> https://www.realmeye.com/wiki/wild-shadow-studios
    /// Deca Games -> https://decagames.com/
    /// IExisted's original post -> https://www.realmeye.com/forum/t/the-ranger-a-close-quarters-bow-class/13789

    if (!bg.loadFromFile("images/World1.png")) { /// Released for non-commercial use by Oryx
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!Arrow.loadFromFile("images/Arrow.png")) { /// Released for non-commercial use by Oryx
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!SArcherSpriteSheet.loadFromFile("images/SlimeArcherSpriteSheet.png")) { /// Created by IExisted
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!Inventory.loadFromFile("images/Inventory.png")) { /// Created by Wildshadow Studios
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!Stats.loadFromFile("images/Stats.png")) { /// Created by Deca Games
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!CrystalBoi.loadFromFile("images/Crystal_Crawler.png")) { /// Created by Deca Games
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!GoldTexture.loadFromFile("images/gold.png")) { /// Released for non-commercial use by Oryx
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }
    if (!upgradedBowShot.loadFromFile("images/dBowShot.png")) { /// Created by Deca Games
        std::cout << "Error: Failed to load file" << std::endl;
        return EXIT_FAILURE;
    }

    ///Creating sidebar "U.I." and minimapdude

    RectangleShape sidebar(Vector2f(5.f, 800.f)); ///Black bar to separate play area and U.I.
    sidebar.setFillColor(Color::Black);

    RectangleShape miniDude(Vector2f(22.f, 25.f)); ///Yellow dot on minimap, which represents player
    miniDude.setFillColor(Color::Yellow);

    RectangleShape minienemy(Vector2f(22.f, 25.f)); ///Red dot on minimap, representing enemy
    minienemy.setFillColor(Color::Red);

    RectangleShape healthBar(Vector2f(286.f, 30.f)); ///Green bar on side, for Health Bar
    healthBar.setFillColor(Color::Green);
    healthBar.setPosition(1210, 425);

    RectangleShape inTheRed(Vector2f(286.f, 30.f)); ///Red behind healthbar for easy visualization of hp
    inTheRed.setFillColor(Color::Red);
    inTheRed.setPosition(1210, 425);

    RectangleShape manaBar(Vector2f(286.f, 30.f)); ///Blue bar on side, for Mana Bar
    manaBar.setFillColor(Color::Cyan);
    manaBar.setPosition(1210, 460);

    RectangleShape inTheBlack(Vector2f(286.f, 30.f)); ///Black bar on the side behind the mana bar
    inTheBlack.setFillColor(Color::Black);
    inTheBlack.setPosition(1210, 460);

    ///Initializing texts for Enemy Damage Indicator and Gold

    Text enemyDamageText, gold;

    ///Loading Font into game

    Font font;

    if (!font.loadFromFile("Images/Oswald-Bold.ttf")) { /// Free for commercial use from fontsquirrel.com
        return EXIT_FAILURE;
    }

    ///Damage text initialized with font, size, and color

    enemyDamageText.setString("DAMAGED");
    enemyDamageText.setCharacterSize(24);
    enemyDamageText.setFillColor(Color::Red);
    enemyDamageText.setStyle(Text::Bold);
    enemyDamageText.setFont(font);

    ///Gold count initialized

    gold.setString("0");
    gold.setCharacterSize(30);
    gold.setFillColor(Color::White);
    gold.setFont(font);
    gold.setPosition(1100, 15);

    Clock eDamageClock;
    float eDamageTimer = 0;
    bool eDamaged = false;

    ///
    ///Establishing basic sprites
    ///

    ///How this works is I went through the spritesheet and took the coordinates of the crosses, allowing me
    ///to split apart the sheet into separate sprites. Then I created sprites for each of the areas with respect
    ///what that area represents.
    ///
    ///The Naming Convention is c (character), (direction character is facing) ,
    ///(NW = Not Walking, W = Walking, S = Shooting), (1, 2 for alternate sprites that are swapped between)

    SArcherSpriteSheet.setSmooth(true);

    Sprite

    cBNW(SArcherSpriteSheet, sf::IntRect(0, 0, 150, 140)),
    cBW1(SArcherSpriteSheet, sf::IntRect(150, 0, 120, 140)),
    cBW2(SArcherSpriteSheet, sf::IntRect(270, 0, 150, 140)),
    cBS1(SArcherSpriteSheet, sf::IntRect(420, 0, 135, 140)),
    cBS2(SArcherSpriteSheet, sf::IntRect(555, 0, 165, 140)),
    cRNW(SArcherSpriteSheet, sf::IntRect(0, 140, 150, 140)),
    cRW2(SArcherSpriteSheet, sf::IntRect(150, 140, 120, 140)),
    cRS1(SArcherSpriteSheet, sf::IntRect(420, 140, 135, 140)),
    cRS2(SArcherSpriteSheet, sf::IntRect(555, 140, 165, 140)),

    cLNW(SArcherSpriteSheet, sf::IntRect(0, 140, 150, 140)),
    cLW2(SArcherSpriteSheet, sf::IntRect(150, 140, 120, 140)),
    cLS1(SArcherSpriteSheet, sf::IntRect(420, 140, 135, 140)),
    cLS2(SArcherSpriteSheet, sf::IntRect(555, 140, 165, 140));

    cLNW.setOrigin(75, 0);
    cLW2.setOrigin(60, 0);
    cLS1.setOrigin(67.5, 0);
    cLS2.setOrigin(82.5, 0);

    Sprite

    cFNW(SArcherSpriteSheet, sf::IntRect(0, 280, 150, 140)),
    cFW1(SArcherSpriteSheet, sf::IntRect(150, 280, 120, 140)),
    cFW2(SArcherSpriteSheet, sf::IntRect(270, 280, 150, 140)),
    cFS1(SArcherSpriteSheet, sf::IntRect(420, 280, 135, 140)),
    cFS2(SArcherSpriteSheet, sf::IntRect(555, 280, 165, 140));

    ///Offsets in case some sprites are too far off center and look bad, also for character positioning

    cBNW.setPosition(575, 375);
    cBW1.setPosition(584, 375);
    cBW2.setPosition(575, 375);
    cBS1.setPosition(575, 375);
    cBS2.setPosition(575, 375);
    cRNW.setPosition(575, 375);
    cRW2.setPosition(575, 375);
    cRS1.setPosition(575, 375);
    cRS2.setPosition(575, 375);
    cLNW.setPosition(600, 375);
    cLW2.setPosition(607, 375);
    cLS1.setPosition(600, 375);
    cLS2.setPosition(600, 375);
    cFNW.setPosition(575, 375);
    cFW1.setPosition(584, 375);
    cFW2.setPosition(575, 375);
    cFS1.setPosition(575, 375);
    cFS2.setPosition(575, 375);

    cBNW.setScale(0.5, 0.5);
    cBW1.setScale(0.5, 0.5);
    cBW2.setScale(0.5, 0.5);
    cBS1.setScale(0.5, 0.5);
    cBS2.setScale(0.5, 0.5);
    cRNW.setScale(0.5, 0.5);
    cRW2.setScale(0.5, 0.5);
    cRS1.setScale(0.5, 0.5);
    cRS2.setScale(0.5, 0.5);
    cLNW.setScale(-0.5, 0.5);
    cLW2.setScale(-0.5, 0.5);
    cLS1.setScale(-0.5, 0.5);
    cLS2.setScale(-0.5, 0.5);
    cFNW.setScale(0.5, 0.5);
    cFW1.setScale(0.5, 0.5);
    cFW2.setScale(0.5, 0.5);
    cFS1.setScale(0.5, 0.5);
    cFS2.setScale(0.5, 0.5);

    Sprite mainCharacter = cFNW; ///Main Sprite

    ///Initializing other sprites

    Sprite background(bg), inventorySprite(Inventory), statPage(Stats), map(bg), enemy(CrystalBoi),
    goldIcon(GoldTexture), superShot(upgradedBowShot);

    ///Creation of "U.I."

    sidebar.setPosition(1200, 0);
    miniDude.setPosition(485, 485);
    inventorySprite.setPosition(1200, 350);
    statPage.setPosition(1200, 280);
    goldIcon.setScale(0.75, 0.75);
    goldIcon.setPosition(1050, 10);

    /*
    Projectile pD;
    Sprite arrow(Arrow);
     */

    ///Creating projectile arrays of data, sprites, and hitboxes

    std::vector<Projectile> projectileDatas;
    std::vector<Sprite> projectiles;
    std::vector<FloatRect> arrowBoxes;

    int projectileCount = 20;

    Sprite arrowSprite(upgradedBowShot); ///Making default arrow sprite
    arrowSprite.scale(0.5, 0.5); ///Making the sprite not too big

    for (int i = 0; i < projectileCount; i++) {
        projectileDatas.emplace_back();
        projectiles.push_back(arrowSprite);
        arrowBoxes.emplace_back();
    }

    ///Establishing Variables for game control

    ///
    ///BASE VARIABLES ON WHICH THE GAME WAS BUILT
    ///

    float charPosX = -40000; ///Character's current X position relative to the map (only negative) (as the map moves, not the player)
    float charPosY = -40000; ///Character's current Y position relative to the map (only negative)

    float minimapOffsetX = 292, minimapOffsetY = 363; ///Offset variables for placement logic

    float minimapScale = 3; ///Scale of the minimap

    float overallBackgroundScale = 60;

    ///
    ///PLAYER CONTROL VARIABLES
    ///

    float dCX; ///Change in character X
    float dCY; ///Change in character Y

    float defX = 600, defY = 400; ///Default X and Y in center of play area

    float charSpeed = 100; ///Base character speed

    bool forward = false, backward = false, left = false, right = false, isMoving = false, gotShot = false;

    std::string lastDirection = "front"; ///Tells which direction was the last to be used

    ///
    ///ENEMY CONTROL VARIABLES
    ///

    float enemySpeedMultiplier = 20; ///Multiplier to the enemies speed

    float enemySpeedX = 0; //FIX
    float enemySpeedY = 0;

    float enemyDfromCX = 0; ///Enemy distance from the character in X
    float enemyDfromCY = 0; ///^ same in Y

    float randX = 0; ///Random x coordinate for when an enemy is generated
    float randY = 0; ///Random y coordinate for when an enemy is generated

    int eDamageCounter = 0; ///Counter for how much damage the enemy has taken
    int enemyHP = 5; ///Total health points of the enemy when instantiated

    bool alive = false; ///Determines if enemy is alive

    float eDamageTextLifespan = 0.5; ///How long damage is displayed above the enemy

    float hitDelay = 1; ///Time before the player can be hit again by the enemy

    float enemyHitTimer = 0; ///Counter used for the above variable.

    ///
    ///PROJECTILE CONTROL VARIABLES
    ///

    float projSpeed = 20; ///Base projectile speed

    bool projectileFound = false, superActive = false, superCanDamageEnemy = false; ///Determines if a projectile has been found for a click

    float projTimer, attackDelay = 0.125; ///Determines attack speed

    float superXSpeed, superYSpeed;

    std::vector<float> mousePos = {0, 0}; ///Mouse position in (x, y) format

    int i = 0; ///Universal for loop big boy

    ///
    ///UI CONTROL VARIABLES
    ///

    int goldAsNum = 0; ///Gold counter

    ///Controls flow of game

    float timer = 0, delay = 0.05, walkDelay = 0.2, walkTimer = 0, shootingDelay = 0.35, shootCycleDelay = 0.125; ///Ticker timers (explained later on)

    Clock gameClock, projClock, hitClock, walkCycle, shootCycle; ///Clocks (explained later on)

    bool walk2 = false, isShooting = false, shot2 = false; ///Animation booleans

    bool justStarted = true;

    ///Scaling to fit screen better

    background.scale(overallBackgroundScale, overallBackgroundScale); ///Makes the ground very large

    statPage.scale(0.86, 1.2); ///Makes the Side larger

    map.scale(overallBackgroundScale / minimapScale,
              overallBackgroundScale / minimapScale); ///Makes the minimap scaled of the main map


    ///Start Screen Stuff

    Text startText;
    startText.setString("Welcome to RotTest!");
    startText.setFont(font);
    startText.setPosition(550, 200);
    startText.setCharacterSize(50);
    startText.setFillColor(Color::Black);

    Text playButtonText;
    playButtonText.setString("Play");
    playButtonText.setFont(font);
    playButtonText.setPosition(700, 475);
    playButtonText.setCharacterSize(50);
    playButtonText.setFillColor(Color::Black);

    RectangleShape openingButton;
    openingButton.setSize(Vector2f(400.f, 200.f));
    openingButton.setPosition(550, 400);
    openingButton.setFillColor(Color::Blue);
    openingButton.setOutlineColor(Color::Cyan);

    ///Creating game logic (A.K.A. THE GAME)

    Mouse mouse;

    while (window.isOpen()) {

        if (justStarted) {

            window.clear(Color::Cyan);

            window.draw(openingButton);

            window.draw(startText);

            window.draw(playButtonText);

            window.display();

            float walkTime = walkCycle.getElapsedTime().asSeconds();
            walkCycle.restart();
            walkTimer += walkTime;

            if(walkTimer > 1){
                walkTimer = 0;

            }

            if (sf::Mouse::isButtonPressed(mouse.Left)) {

                Vector2i mvec = sf::Mouse::getPosition(window);

                if( mvec.x > 550 && mvec.x < 950){
                    if(mvec.y > 400 && mvec.y < 600) {
                        justStarted = false;
                    }
                }
            }

        } else {

            srand(time(nullptr)); /// Seeding random number generator

            ///
            ///MOVEMENT CONTROL SYSTEM
            ///

            Event move{};

            ///Basically this turns on/off booleans based on which keys are pressed/released for smooth movement of the player

            while (window.pollEvent(move)) {
                if (move.type == Event::Closed) {
                    window.close(); /// Closes window when window is closed
                }

                if (move.type == move.KeyPressed) { /// When the key is pressed of each type

                    if (move.key.code == Keyboard::W) forward = true;

                    if (move.key.code == Keyboard::S) backward = true;

                    if (move.key.code == Keyboard::A) left = true;

                    if (move.key.code == Keyboard::D) right = true;

                } else if (move.type == move.KeyReleased) { /// When the key is released of each type

                    if (move.key.code == Keyboard::W) forward = false;

                    if (move.key.code == Keyboard::S) backward = false;

                    if (move.key.code == Keyboard::A) left = false;

                    if (move.key.code == Keyboard::D) right = false;
                }

            }

            ///
            ///Character movement speed logic
            ///

            ///Creation of movement animation

            ///Determines Walk Cycle based on how much time has passed

            float walkTime = walkCycle.getElapsedTime().asSeconds();
            walkCycle.restart();
            walkTimer += walkTime;

            if (walkTimer > walkDelay) {

                ///Basic flopper switch to make the walk cycle cycle through both walking sprites

                if (walk2) {
                    walk2 = false;
                } else {
                    walk2 = true;
                }

                walkTimer = 0;
            }

            ///Long but self-explanatory system of changing the sprite based on direction of character and instance of the walk cycle

            if (forward) {
                isMoving = true; /// States that the character is moving
                dCY = charSpeed; /// Sets change in Y to character speed
                lastDirection = "back"; /// Sets last direction to backwards
                if (!isShooting) { /// If the character is not shooting
                    if (walk2) { /// If the walk cycle is Flopped
                        mainCharacter = cBW1; /// Do walk sprite 1
                    } else { /// Unless it's Flipped
                        mainCharacter = cBW2; /// Then do walk sprite 2
                    }
                }

                ///Exact same logic with the next few blocks

            } else if (backward) {
                isMoving = true;
                dCY = -charSpeed;
                lastDirection = "forward";
                if (!isShooting) {
                    if (walk2) {
                        mainCharacter = cFW1;
                    } else {
                        mainCharacter = cFW2;
                    }
                }
            } else {
                dCY = 0; /// If it's not moving up or down, its not moving in the Y direction
            }

            if (left) {
                isMoving = true;
                dCX = charSpeed;
                lastDirection = "left";
                if (!isShooting) {
                    if (walk2) {
                        mainCharacter = cLW2;
                    } else {
                        mainCharacter = cLNW;
                    }
                }

            } else if (right) {
                isMoving = true;
                dCX = -charSpeed;
                lastDirection = "right";
                if (!isShooting) {
                    if (walk2) {
                        mainCharacter = cRW2;
                    } else {
                        mainCharacter = cRNW;
                    }
                }

            } else {
                dCX = 0; /// If it's not moving left or right, its not moving in the X direction
            }

            if ((dCX == 0) && (dCY == 0)) {
                isMoving = false; /// If you're not moving in any direction, you're not moving
            }

            ///Determines which direction was last used, and sets character to idle sprite in that direction

            if (!isShooting) { /// If you're not shooting
                if (!isMoving) { /// Nor moving
                    if (lastDirection == "forward") { /// And your last direction moved was forward
                        mainCharacter = cFNW; /// Then your idle sprite is c(haracter) F(orward) N(ot) W(alking)
                    } else if (lastDirection == "back") { /// Same logic applies below
                        mainCharacter = cBNW;
                    } else if (lastDirection == "right") {
                        mainCharacter = cRNW;
                    } else if (lastDirection == "left") {
                        mainCharacter = cLNW;
                    }
                }
            }

            /// Sets movement speed to Speed / (2)^(1/2) in each direction if both keys are,
            /// pressed for diagonals due to oversight in speed increase when both dCX and dCY were = Character Speed

            if ((dCX == charSpeed) && (dCY == charSpeed)) {
                dCX = charSpeed / 1.6;
                dCY = charSpeed / 1.6;
            }
            if ((dCX == -charSpeed) && (dCY == charSpeed)) {
                dCX = -charSpeed / 1.6;
                dCY = charSpeed / 1.6;
            }
            if ((dCX == charSpeed) && (dCY == -charSpeed)) {
                dCX = charSpeed / 1.6;
                dCY = -charSpeed / 1.6;
            }
            if ((dCX == -charSpeed) && (dCY == -charSpeed)) {
                dCX = -charSpeed / 1.6;
                dCY = -charSpeed / 1.6;
            }

            ///End of movement speed logic

            ///Changing position of the map relative to the character

            charPosX += dCX / 15;
            charPosY += dCY / 15;

            ///
            ///End of movement control system
            ///



            ///
            ///Projectile control system
            ///

            ///Activation of Arrows



            ///Projectile control system

            ///Determines if player can shoot another shot
            float projTime = projClock.getElapsedTime().asSeconds();
            projClock.restart();
            projTimer += projTime;

            if (sf::Mouse::isButtonPressed(mouse.Left)) { ///When left click

                if (projTimer > attackDelay) { ///If attack delay has been achieved

                    projTimer = 0; ///Restart timer to prevent infinite shots

                    projectileFound = false; ///Start looking for a new projectile to use (that's not in use)

                    i = 0; ///The Big Boy

                    while (!projectileFound) {

                        if (!projectileDatas.at(i).isActive()) {

                            ///Vector algebra to determine direction and set velocity of the projectile

                            Vector2i mvec = sf::Mouse::getPosition(window); ///Mouse position in vector form

                            mousePos.at(0) = mvec.x - defX; ///Mouse position relative to the character
                            mousePos.at(1) = mvec.y - defY;

                            projectileDatas.at(i).setPosX(
                                    defX); ///Setting the projectiles postion to the character position
                            projectileDatas.at(i).setPosY(defY);

                            ///Vector algebra that finds the x/y position devided by the magnitude of the vector to get
                            ///x or y velocity as a norm, then multiplying by projectile speed

                            projectileDatas.at(i).setXSpeed(projSpeed * (mousePos.at(0) /
                                                                         (sqrtf((mousePos.at(0) * mousePos.at(0)) +
                                                                                (mousePos.at(1) * mousePos.at(1))))));
                            projectileDatas.at(i).setYSpeed(projSpeed * (mousePos.at(1) /
                                                                         (sqrtf((mousePos.at(0) * mousePos.at(0)) +
                                                                                (mousePos.at(1) * mousePos.at(1))))));

                            ///Setting the current projectile as active to avoid being used again

                            projectileDatas.at(i).setActive(true);

                            ///Resetting the scale of the arrow

                            projectiles.at(i).setScale(0.5, 0.5);

                            ///Changing rotation of the arrow to avoid having arrows being shot backwards

                            projectiles.at(i).setRotation(
                                    (atan(mousePos.at(1) / mousePos.at(0)) * 57.29)); ///Setting angle of rotation

                            if (mousePos.at(0) < 0) { ///If x is negative
                                projectiles.at(i).setScale(-0.5, -0.5); ///Flop the arrow on x and y axis
                            }

                            projectileFound = true; ///State that we've found a projectile to break loop

                            ///Creating the shooting animation below (same logic as used in character sprite swapping)

                            isShooting = true; ///Used to determine when the player stops shooting

                            shot2 = true;

                        }

                        i++; ///Finds a new projectile if an active one isn't found
                        if (i == (projectileCount - 1)) {
                            i = 0; ///Restarts i when all 10 have been shot
                        }

                    }
                }
            }

            ///Used to determine when to stop the shooting animation

            if (projTimer > attackDelay) {
                isShooting = false; /// Sets to idle sprites (See in character logic)
            } else if (projTimer > attackDelay / 2) {
                shot2 = false;
            }

            if (isShooting) {
                if (mousePos.at(0) < 0) { /// If your mouse position is negative with respect to character (to the left)
                    if (std::abs(mousePos.at(0)) > std::abs(
                            mousePos.at(1))) { ///And the absolute value of x is greater than the absolute value of y
                        lastDirection = "left"; /// Cycle the shots on the left
                        if (shot2) {
                            mainCharacter = cLS1;
                        } else {
                            mainCharacter = cLS2;
                        }
                    } else if (mousePos.at(1) > 0) { /// If in the upper triangle
                        lastDirection = "forward";
                        if (shot2) {
                            mainCharacter = cFS1;
                        } else {
                            mainCharacter = cFS2;
                        }
                    } else { /// In the lower triangle
                        lastDirection = "backward";
                        if (shot2) {
                            mainCharacter = cBS1;
                        } else {
                            mainCharacter = cBS2;
                        }
                    }

                } else if (mousePos.at(0) > 0) { /// Same logic but on right side
                    if (mousePos.at(0) > std::abs(mousePos.at(1))) {
                        lastDirection = "right";
                        if (shot2) {
                            mainCharacter = cRS1;
                        } else {
                            mainCharacter = cRS2;
                        }
                    } else if (mousePos.at(1) > 0) {
                        lastDirection = "forward";
                        if (shot2) {
                            mainCharacter = cFS1;
                        } else {
                            mainCharacter = cFS2;
                        }
                    } else {
                        lastDirection = "back";
                        if (shot2) {
                            mainCharacter = cBS1;
                        } else {
                            mainCharacter = cBS2;
                        }
                    }
                }
            }


            for (i = (projectileCount - 1); i > -1; i--) { ///Goes through each arrow

                ///Sets its new hitbox location

                arrowBoxes.push_back(projectiles.at(i).getGlobalBounds());

                ///Checking for active projectiles and increasing their position accordingly

                if (projectileDatas.at(i).isActive()) {

                    ///If it's active, it's position is increased

                    projectileDatas.at(i).setPosX(
                            projectileDatas.at(i).getPosX() + projectileDatas.at(i).getXSpeed() + dCX / 15);
                    projectileDatas.at(i).setPosY(
                            projectileDatas.at(i).getPosY() + projectileDatas.at(i).getYSpeed() + dCY / 15);

                }

                ///Checking for out of bounds Projectiles and making them defaulted (not drawn and inactive)

                if ((projectileDatas.at(i).getPosX() > 1200) || (projectileDatas.at(i).getPosX() < 0) ||
                    (projectileDatas.at(i).getPosY() > 800) ||
                    (projectileDatas.at(i).getPosY() < 0) || !projectileDatas.at(i).isActive()) {

                    projectileDatas.at(i).setXSpeed(0);
                    projectileDatas.at(i).setYSpeed(0);
                    projectileDatas.at(i).setPosX(defX);
                    projectileDatas.at(i).setPosY(defY + 10);
                    projectileDatas.at(i).setActive(false);
                    projectileDatas.at(i).setCanDamageEnemy(true);

                    arrowBoxes.clear();

                }

            }

            ///
            ///End of projectile control system
            ///



            ///
            ///Start of enemy AI system
            ///

            ///Creating enemy within render distance

            if (!alive) {  ///When the enemy isn't alive

                randX = rand() % 2200 - 1000; ///Get a random X
                randY = rand() % 1800 - 1000; ///And a random Y

                enemy.setPosition(randX, randY); ///Set the enemy there

                enemyDfromCX = defX - 30 - enemy.getPosition().x; ///Enemy distance from character in X direction
                enemyDfromCY = defY - 30 - enemy.getPosition().y; ///Same in Y

                minienemy.setPosition((randX / minimapScale) + 300,
                                      (randY / minimapScale) + 360); ///Setting red dot on minimap where enemy is

                alive = true; ///Stating that the enemy is alive
            }

            ///Moving enemy relative to the player

            enemyDfromCX = defX - 30 - enemy.getPosition().x; ///Setting current distance from player in X direction
            enemyDfromCY = defY - 30 - enemy.getPosition().y; ///In Y direction

            ///Same algebra as projectile system, simply gets relative speed in x and y and multiplies by the multiplier

            enemySpeedX = ((enemySpeedMultiplier * enemyDfromCX /
                            sqrtf((enemyDfromCX * enemyDfromCX) + (enemyDfromCY * enemyDfromCY))) / 15);
            enemySpeedY = ((enemySpeedMultiplier * enemyDfromCY /
                            sqrtf((enemyDfromCX * enemyDfromCX) + (enemyDfromCY * enemyDfromCY))) / 15);

            ///Prevents division by infinity when computing the speed

            if ((enemy.getPosition().x == defX - 30) && (enemy.getPosition().y == defY - 30)) {
                enemySpeedX = 0;
                enemySpeedY = 0;
            }

            ///Places the enemy further in it's speed direction

            enemy.setPosition(enemy.getPosition().x + enemySpeedX + (dCX / 15),
                              enemy.getPosition().y + enemySpeedY + (dCY / 15));

            ///Timer for player getting hit by the enemy logic, to prevent instant death

            float enemyHitTime = hitClock.getElapsedTime().asSeconds();
            hitClock.restart();
            enemyHitTimer += enemyHitTime;

            ///Getting the current hitboxes of the character and the enemy

            FloatRect charBox = mainCharacter.getGlobalBounds();
            FloatRect enemyBox = enemy.getGlobalBounds();

            if (charBox.intersects(enemyBox)) { ///If the player intersects the enemy
                if (enemyHitTimer > hitDelay) { ///And the hit delay has been achieved
                    healthBar.setSize(
                            Vector2f(healthBar.getSize().x - 30, healthBar.getSize().y)); ///Lower the player health
                    enemyHitTimer = 0; ///restart the timer
                    if (healthBar.getSize().x < 0) { ///If player health goes below 0
                        return 0; ///End the game
                    }
                }
            }

            ///Loop to check if enemy has been hit by projectiles

            for (i = 0; i < arrowBoxes.size(); i++) {
                if (arrowBoxes.at(i).intersects(enemyBox) && projectileDatas.at(
                        i).isCanDamageEnemy()) { ///If the current arrow being checked intersects the enemy, and the arrow hasn't hit the enemy yet

                    eDamaged = true;
                    enemyDamageText.setPosition(enemy.getPosition().x,
                                                enemy.getPosition().y - 10); ///Make a hit indicator

                    eDamageCounter += 1; ///Increase the enemy damage counter

                    projectileDatas.at(i).setCanDamageEnemy(false); ///Make it so the arrow can't hit the enemy again

                    enemyDamageText.setString("Damaged");


                }
            }

            if (eDamageCounter > enemyHP) { ///If the enemy has taken more damage than its health
                alive = false; ///Kill it
                eDamageCounter = 0; ///Restart the damage counter
                enemySpeedMultiplier += 5; ///Increase its speed

                if (enemySpeedMultiplier > 20) {
                    enemySpeedMultiplier = 20; ///Don't let the multiplier go over 20
                    enemyHP += 1; ///Give the enemy more hp the next time it comes alive
                    goldAsNum += 1; ///Give the player some gold for the kill
                }
            }

            ///
            ///End of enemy AI section
            ///



            ///
            ///Start of mana control section
            ///

            ///Ticker counter initialized (This prevents the game from shorting out, displaying too many frames and crashing computer
            ///Conveniently I can also use this to help me regen mana over time.

            float time = gameClock.getElapsedTime().asSeconds();
            gameClock.restart();
            timer += time;

            if (timer > delay) {
                if (manaBar.getSize().x != 286) {
                    manaBar.setSize(Vector2f(manaBar.getSize().x + 1, manaBar.getSize().y));
                }
                timer = 0;
            }


            if (sf::Mouse::isButtonPressed(mouse.Right)) { ///When Right Click
                if (manaBar.getSize().x > 80) {
                    if (!superActive) {

                        manaBar.setSize(Vector2f(manaBar.getSize().x - 70, manaBar.getSize().y));

                        Vector2i mvec = sf::Mouse::getPosition(window); ///Mouse position in vector form

                        mousePos.at(0) = mvec.x - defX; ///Mouse position relative to the character
                        mousePos.at(1) = mvec.y - defY;

                        superShot.setPosition(defX, defY);

                        ///Vector algebra that finds the x/y position devided by the magnitude of the vector to get
                        ///x or y velocity as a norm, then multiplying by projectile speed

                        superXSpeed = projSpeed * (mousePos.at(0) / (sqrtf((mousePos.at(0) * mousePos.at(0)) +
                                                                           (mousePos.at(1) * mousePos.at(1)))));
                        superYSpeed = projSpeed * (mousePos.at(1) / (sqrtf((mousePos.at(0) * mousePos.at(0)) +
                                                                           (mousePos.at(1) * mousePos.at(1)))));

                        superShot.setRotation(
                                (atan(mousePos.at(1) / mousePos.at(0)) * 57.29)); ///Setting angle of rotation

                        if (mousePos.at(0) < 0) { ///If x is negative
                            superShot.setScale(-1, -1); ///Flop the arrow on x and y axis
                        }

                        superActive = true;
                        superCanDamageEnemy = true;

                    }
                }
            }


            if (superActive) {

                superShot.setPosition(superShot.getPosition().x + superXSpeed + dCX / 15,
                                      superShot.getPosition().y + superYSpeed + dCY / 15);

                FloatRect superBox = superShot.getGlobalBounds();

                if (superCanDamageEnemy) {
                    if (superBox.intersects(enemyBox)) {

                        eDamageCounter += 2; ///Increase the enemy damage counter

                        enemySpeedMultiplier -= 0.05; /// Slows the enemy permanently

                        eDamaged = true;
                        enemyDamageText.setPosition(enemy.getPosition().x,
                                                    enemy.getPosition().y - 10); ///Make a hit indicator

                        enemyDamageText.setString("SUPER HIT");

                        superCanDamageEnemy = false;

                    }
                }

                if ((superShot.getPosition().x > 1200) || (superShot.getPosition().x < 0) ||
                    (superShot.getPosition().y > 800) ||
                    (superShot.getPosition().y < 0) || !superActive) {

                    superXSpeed = 0;
                    superYSpeed = 0;
                    superShot.setPosition(defX, defY);
                    superActive = false;
                    superCanDamageEnemy = false;

                }

            }

            ///
            ///End of Mana Control
            ///

            ///
            ///Drawing to screen section start
            ///

            ///Clearing screen

            window.clear(Color::Black);

            ///Drawing Background

            window.draw(background);
            background.setPosition(charPosX, charPosY);

            ///Drawing projectiles

            for (i = 0; i < projectileCount; i++) {
                if (projectileDatas.at(i).isActive()) {
                    window.draw(projectiles.at(i));
                    projectiles.at(i).setPosition(projectileDatas.at(i).getPosX(), projectileDatas.at(i).getPosY());
                }
            }

            if (superActive) {
                window.draw(superShot);
            }

            ///Drawing the character at the default location
            window.draw(mainCharacter);

            ///Drawing enemies when within visualRenderDistance

            if ((enemy.getPosition().x > -600) && (enemy.getPosition().x < 1800) && (enemy.getPosition().y > -600) &&
                (enemy.getPosition().y < 1400)) {
                window.draw(enemy);
            }

            ///Drawing damage indicators

            float eDamageTime = eDamageClock.getElapsedTime().asSeconds();
            eDamageClock.restart();
            eDamageTimer += eDamageTime;

            ///System that prevents the damage text from lasting too long, but also makes it a bit glitchy...

            if (eDamaged) { /// If the enemy is damaged
                enemyDamageText.setPosition(enemyDamageText.getPosition().x + (dCX / 15),
                                            enemyDamageText.getPosition().y + (dCY / 15) -
                                            (0.1)); /// Set the text above the enemy
                window.draw(enemyDamageText); /// And draw the text
            }

            if (eDamageTimer > eDamageTextLifespan) { /// If the text life span has been achieved
                eDamaged = false; /// Reset the logic
                eDamageTimer = 0; /// And set the timer to 0
            }

            ///Creating inventory/stats page, self explanatory code

            gold.setString(std::to_string(goldAsNum));

            window.draw(statPage);
            window.draw(inventorySprite);

            window.draw(inTheRed);
            window.draw(healthBar);

            window.draw(inTheBlack);
            window.draw(manaBar);

            window.draw(goldIcon);

            window.draw(gold);

            ///Minimap creation and movement

            map.setPosition((charPosX / 3) + minimapOffsetX, (charPosY / 3) + minimapOffsetY);

            window.setView(minimap); ///Setting the view to the minimap view
            window.draw(map);

            window.draw(minienemy);

            ///Places the mini enemy where the enemy would be on the minimap

            minienemy.setPosition(minienemy.getPosition().x + ((enemySpeedX + (dCX / 15)) / minimapScale),
                                  (minienemy.getPosition().y + ((enemySpeedY + (dCY / 15)) / minimapScale)));

            window.draw(miniDude); ///Character dot (Yellow dot in middle of minimap)

            ///Adding splitter sidebar for better looks

            window.setView(window.getDefaultView());
            window.draw(sidebar);

            ///Displaying to window

            window.display();
        }
    }
    return 0;
}