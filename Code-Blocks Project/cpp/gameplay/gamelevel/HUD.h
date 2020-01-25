#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "../../system/entity/MainObject.h"
#include "../../system/function/GameTime.h"
#include "../../core/GameSystemExtended.h"
#include "Player.h"

class HUD : public is::MainObject
{
public:
    HUD(unsigned int &playerHealth, is::GameDisplay &scene);

    void loadResources(sf::Font const &fnt);
    void step(float const &DELTA_TIME, GameTime &gameTime, Player &player, bool &levelComplete);
    void draw(sf::RenderTexture &surface);
    void setScore(int val);

private:
    float m_timeScoreCount;
    int m_score;
    is::GameDisplay &m_scene;
    unsigned int &m_playerHealth;

    sf::Text m_txtHealth, m_txtBonus, m_txtLives, m_txtLevelTime, m_txtScore;
};

#endif // HUD_H_INCLUDED
