//
// Created by Tyler on 3/15/2021.
//

#ifndef ROTTEST_ENEMY_H
#define ROTTEST_ENEMY_H

class Enemy{
private:
    bool isAlive, canDamage;
    int damage;
    float speedX, speedY;
    float posX, posY;
    float dFromCX, dFromCY;

public:
    explicit Enemy(bool isAlive = false, bool canDamage = true, int damage = 0, float speedX = 0, float speedY = 0, float posX = 0, float posY = 0, float dFromCX = 0, float dFromCY = 0) :
    isAlive(isAlive), canDamage(canDamage), damage(damage), speedX(speedX), speedY(speedY), posX(posX), posY(posY), dFromCX(dFromCX), dFromCY(dFromCY){}

    bool isAlive1() const {
        return isAlive;
    }

    void setIsAlive(bool isAlie) {
        Enemy::isAlive = isAlie;
    }

    bool isCanDamage() const {
        return canDamage;
    }

    void setCanDamage(bool canDmage) {
        Enemy::canDamage = canDmage;
    }


    int getDamage() const {
        return damage;
    }

    void setDamage(int damge) {
        Enemy::damage = damge;
    }

    float getSpeedX() const {
        return speedX;
    }

    void setSpeedX(float spdX) {
        Enemy::speedX = spdX;
    }

    float getSpeedY() const {
        return speedY;
    }

    void setSpeedY(float spdY) {
        Enemy::speedY = spdY;
    }

    float getPosX() const {
        return posX;
    }

    void setPosX(float psX) {
        Enemy::posX = psX;
    }

    float getPosY() const {
        return posY;
    }

    void setPosY(float psY) {
        Enemy::posY = psY;
    }

    float getDFromCx() const {
        return dFromCX;
    }

    void setDFromCx(float dFrmCx) {
        dFromCX = dFrmCx;
    }

    float getDFromCy() const {
        return dFromCY;
    }

    void setDFromCy(float dFrmCy) {
        dFromCY = dFrmCy;
    }


};


#endif //ROTTEST_ENEMY_H
