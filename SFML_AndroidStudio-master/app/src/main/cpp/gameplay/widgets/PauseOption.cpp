#include "PauseOption.h"

PauseOption::PauseOption(sf::Texture &tex):
    MainObject(),
    m_keyIsLocked(false),
    m_sprSFXScale(1.f),
    m_sprVibrateScale(1.f)
{
    for (int i(0); i < 4; i++)
    {
        m_sprMask[i].setSize(sf::Vector2f(42.f, 42.f));
        is::centerSFMLObj(m_sprMask[i]);
    }
    is::createSprite(tex, m_sprIcoSFX, sf::IntRect(0, 0, 48, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));
    is::createSprite(tex, m_sprParent, sf::IntRect(96, 0, 48, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));
    is::createSprite(tex, m_sprIcoVibrate, sf::IntRect(192, 0, 48, 32), sf::Vector2f(m_x, m_y), sf::Vector2f(16.f, 16.f));
}

void PauseOption::step(float const &DELTA_TIME, is::GameDisplay *scene, sf::Sound &m_sndSelectOption)
{
    if (scene->getGameSystem().isPressed() && !m_keyIsLocked)
    {
        bool collision(false);
        auto setVariable = [this](PauseOptionType value)
        {
            if (m_answer != value) m_answer = value;
        };
        if (is::mouseCollision((*scene), m_sprMask[0]))
        {
            setVariable(PAUSE_OPTION_SFX);
            collision = true;
        }
        else if (is::mouseCollision((*scene), m_sprMask[1]))
        {
            setVariable(PAUSE_OPTION_MSC);
            collision = true;
        }
        else if (is::mouseCollision((*scene), m_sprMask[2]))
        {
            setVariable(PAUSE_OPTION_VIBRATE);
            collision = true;
        }

        if (collision)
        {
            auto setObjVar = [this, scene, &m_sndSelectOption](bool &varIndex, float &scaleVar)
            {
                if (varIndex) varIndex = false; else varIndex = true;
                scaleVar = 1.5f;
                scene->getGameSystem().playSound(m_sndSelectOption);
            };
            if (m_answer != PAUSE_OPTION_VIBRATE) scene->getGameSystem().useVibrate(60);
            switch (m_answer)
            {
                case PAUSE_OPTION_SFX:
                    setObjVar(scene->getGameSystem().m_enableSound, m_sprSFXScale);
                break;
                case PAUSE_OPTION_MSC:
                    setObjVar(scene->getGameSystem().m_enableMusic, m_imageScale);
                break;
                case PAUSE_OPTION_VIBRATE:
                    setObjVar(scene->getGameSystem().m_enableVibrate, m_sprVibrateScale);
                    scene->getGameSystem().useVibrate(120);
                break;
            }
            scene->getGameSystem().saveConfig(is::CONFIG_FILE);
            m_keyIsLocked = true;
        }
    }

    if (scene->getGameSystem().m_enableSound) m_sprIcoSFX.setTextureRect(sf::IntRect(0, 0, 48, 32));
    else m_sprIcoSFX.setTextureRect(sf::IntRect(48, 0, 48, 32));
    if (scene->getGameSystem().m_enableMusic) m_sprParent.setTextureRect(sf::IntRect(96, 0, 48, 32));
    else m_sprParent.setTextureRect(sf::IntRect(144, 0, 48, 32));
    if (scene->getGameSystem().m_enableVibrate) m_sprIcoVibrate.setTextureRect(sf::IntRect(192, 0, 48, 32));
    else m_sprIcoVibrate.setTextureRect(sf::IntRect(240, 0, 48, 32));

    float _space(64.f);
    m_sprIcoSFX.setPosition(m_x - _space, m_y);
    m_sprParent.setPosition(m_x, m_y);
    m_sprIcoVibrate.setPosition(m_x + _space, m_y);

    is::setSFMLObjX_Y(m_sprMask[0], is::getSFMLObjX(m_sprIcoSFX), is::getSFMLObjY(m_sprIcoSFX));
    is::setSFMLObjX_Y(m_sprMask[1], is::getSFMLObjX(m_sprParent), is::getSFMLObjY(m_sprParent));
    is::setSFMLObjX_Y(m_sprMask[2], is::getSFMLObjX(m_sprIcoVibrate), is::getSFMLObjY(m_sprIcoVibrate));

    is::scaleAnimation(DELTA_TIME, m_sprSFXScale, m_sprIcoSFX);
    is::scaleAnimation(DELTA_TIME, m_imageScale, m_sprParent);
    is::scaleAnimation(DELTA_TIME, m_sprVibrateScale, m_sprIcoVibrate);
}

void PauseOption::draw(sf::RenderTexture &surface)
{
    surface.draw(m_sprIcoSFX);
    surface.draw(m_sprIcoVibrate);
    surface.draw(m_sprParent);
}
