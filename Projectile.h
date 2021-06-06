//
// Created by Tyler on 3/10/2021.
//

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>

#ifndef ROTTEST_PROJECTILE_H
#define ROTTEST_PROJECTILE_H

class Projectile{
private:
    bool active, canDamageEnemy;

    float posX, posY;

    float xSpeed, ySpeed;

public:

    explicit Projectile(bool active = false, bool canDamageEnemy = true, float posX = 590, float posY = 387, float xSpeed = 0, float ySpeed = 0) : active(active), canDamageEnemy(canDamageEnemy), posX(posX), posY(posY), xSpeed(xSpeed), ySpeed(ySpeed) {}

    bool isActive() const {
        return active;
    }

    void setActive(bool act) {
        Projectile::active = act;
    }

    bool isCanDamageEnemy() const {
        return canDamageEnemy;
    }

    void setCanDamageEnemy(bool canDamageE) {
        Projectile::canDamageEnemy = canDamageE;
    }

    float getPosX() const {
        return posX;
    }

    void setPosX(float pX) {
        Projectile::posX = pX;
    }

    float getPosY() const {
        return posY;
    }

    void setPosY(float pY) {
        Projectile::posY = pY;
    }

    float getXSpeed() const {
        return xSpeed;
    }

    void setXSpeed(float xSpd) {
        Projectile::xSpeed = xSpd;
    }

    float getYSpeed() const {
        return ySpeed;
    }

    void setYSpeed(float ySpd) {
        Projectile::ySpeed = ySpd;
    }

};




#endif //ROTTEST_PROJECTILE_H
