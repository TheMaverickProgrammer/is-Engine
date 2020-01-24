#ifndef ELECTROSHOCK_H_INCLUDED
#define ELECTROSHOCK_H_INCLUDED

#include "../../system/entity/MainObject.h"
#include "../../system/entity/Destructible.h"

class ElectroShock : public is::MainObject, public is::Destructible
{
public:
    ElectroShock(float x, float y);

    void step(float const &DELTA_TIME);

    void draw(sf::RenderTexture &surface);

private:
    sf::CircleShape m_circleElectro;
};

#endif // ELECTROSHOCK_H_INCLUDED
