//
// Created by Tyler on 3/14/2021.
//

#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>


#ifndef ROTTEST_CHARACTER_H
#define ROTTEST_CHARACTER_H


class Character{
private:

    sf::Sprite currSprite;

public:

    explicit Character(sf::Sprite currSprite = sf::Sprite()) : currSprite(std::move(currSprite)) {}

    const sf::Sprite &getCurrSprite() const {
        return currSprite;
    }

    void setCurrSprite(const sf::Sprite &crrSprite) {
        Character::currSprite = crrSprite;
    }


};




#endif //ROTTEST_CHARACTER_H
