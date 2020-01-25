#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "../../core/GameSystemExtended.h"
#include "../../system/entity/MainObject.h"
#include "../../system/entity/Destructible.h"
#include "../parents/ScorePoint.h"
#include "../parents/Health.h"

class Enemy : public is::MainObject, public is::Destructible, public ScorePoint
{
public:
    enum EnemyType
    {
        ENEMY_SPIKE_BALL,
        ENEMY_BALL
    };

    Enemy(sf::Texture &tex, EnemyType type, float x, float y, int scorePoint = 0);
    ~Enemy();

    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

    bool getIsDestructible() const;
    EnemyType getType() const;

private:
    EnemyType m_type;
    float m_animationTime;
    bool m_destructible;
    int m_playSndTime;
};

#endif // ENEMY_H_INCLUDED
