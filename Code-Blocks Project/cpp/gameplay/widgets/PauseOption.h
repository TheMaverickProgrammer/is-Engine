#ifndef PAUSEOPTION_H_INCLUDED
#define PAUSEOPTION_H_INCLUDED

#include "../../system/entity/MainObject.h"
#include "../gamelevel/HUD.h"

class PauseOption : public is::MainObject
{
public:
    enum PauseOptionType
    {
        PAUSE_OPTION_SFX,
        PAUSE_OPTION_MSC,
        PAUSE_OPTION_VIBRATE
    };
    bool m_keyIsLocked;
    PauseOption(sf::Texture &tex);

    void step(float const &DELTA_TIME, is::GameDisplay *scene, sf::Sound &m_sndSelectOption);
    void draw(sf::RenderTexture &surface);

private:
    float m_sprSFXScale, m_sprVibrateScale;
    PauseOptionType m_answer;
    sf::Sprite m_sprIcoSFX, m_sprIcoVibrate;
    sf::RectangleShape m_sprMask[4];
};

#endif // PAUSEOPTION_H_INCLUDED
