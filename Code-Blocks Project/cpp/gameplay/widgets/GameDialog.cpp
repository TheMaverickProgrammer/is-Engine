#include "GameDialog.h"

GameDialog::GameDialog() :
    MainObject(),
    m_showDialog(false),
    m_mouseInCollison(false),
    m_dialogEnd(false),
    m_newLine(true),
    m_msgIndex(0),
    m_msgIndexMax(0),
    m_size(0),
    m_blindTime(0.f),
    m_dialogIndex(DIALOG_NONE)
{
    m_imageScale = 0.f;
}

void GameDialog::loadResources(sf::Texture &tex, sf::Font &fnt, GameSystemExtended &gameSystem)
{
    is::createText(fnt, m_txtDialog, "", m_x, m_y, 16, false, false);
    is::createText(fnt, m_txtSkip, "", m_x, m_y, 13, false, true);
    is::setSFMLObjColor(m_txtSkip, sf::Color(0, 0, 255, 255));
    m_strDialog = L"";
    is::createSprite(tex, m_sprParent, sf::IntRect(0, 0, 480, 96), sf::Vector2f(0.f, 0.f), sf::Vector2f(240.f, 48.f), true);
    is::createSprite(tex, m_sprNext, sf::IntRect(64, 96, 32, 32), sf::Vector2f(0.f, 0.f), sf::Vector2f(16.f, 16.f));
    is::createSprite(tex, m_sprSkip, sf::IntRect(0, 96, 64, 24), sf::Vector2f(0.f, 0.f), sf::Vector2f(32.f, 12.f));
    is::setSFMLObjScale(m_txtDialog, 0.f);
    m_txtSkip.setString(lang::pad_dialog_skip[gameSystem.m_gameLanguage]);
    is::centerSFMLObj(m_txtSkip);
    is::setSFMLObjScale(m_txtSkip, 0.f);
    is::setSFMLObjScale(m_sprParent, 0.f);
    is::setSFMLObjScale(m_sprNext, 0.f);
}

void GameDialog::step(const float &DELTA_TIME, is::GameDisplay *scene, sf::Sound &sndSwitch, sf::Sound &sndCancel)
{
    if (m_showDialog)
    {
        m_mouseInCollison = false;
        float const _VAL(is::getMSecond(DELTA_TIME));
        m_time += (0.8f * is::VALUE_CONVERSION) * DELTA_TIME;
        m_blindTime += _VAL;
        if (m_blindTime > 30.f) m_blindTime = 0.f;

        auto setMsg = [this](std::wstring txt)
        {
            m_strDialog = txt;
        };
        auto getDialogChar = [this](int index = -1)
        {
            int n = m_strDialog.length();
            wchar_t char_array[n + 1];
            for (int i(0); i < n; i++) char_array[i] = m_strDialog[i];
            return char_array[((index == -1) ? m_size + 1: index)];
        };
        auto checkMsg =[this, scene, &setMsg](std::wstring txt[])
        {
            if (m_msgIndex < m_msgIndexMax) setMsg(txt[m_msgIndex + scene->getGameSystem().m_gameLanguage]);
        };

        switch (m_dialogIndex)
        {
            case DIALOG_PLAYER_MOVE:
                m_msgIndexMax = is::arraySize(lang::dialog_player_move);
                checkMsg(lang::dialog_player_move);
            break;

            case DIALOG_PLAYER_ATTACK:
                m_msgIndexMax = is::arraySize(lang::dialog_player_attack);
                checkMsg(lang::dialog_player_attack);
            break;

            default:
            break;
        }
        if (m_size < static_cast<int>(m_strDialog.size()) - 1)
        {
            if (m_time > 1.f)
            {
                std::wstring tempoStr = m_txtDialog.getString();
                if (m_newLine)
                {
                    scene->getGameSystem().playSound(sndSwitch);
                    m_txtDialog.setString(getDialogChar(0));
                    m_newLine = false;
                }
                else
                {
                    m_txtDialog.setString(tempoStr + getDialogChar());
                    m_size++;
                }
                m_time = 0.f;
            }
        }

        bool mouseInCollisonSkip(false);
        if (is::mouseCollision((*scene), m_sprSkip)) mouseInCollisonSkip = true;
        if (scene->getGameSystem().isPressed() && mouseInCollisonSkip && !m_dialogEnd)
        {
            scene->getGameSystem().playSound(sndCancel);
            scene->getGameSystem().useVibrate(60);
            m_dialogEnd = true;
        }

        if (is::mouseCollision((*scene), m_sprParent)) m_mouseInCollison = true;
        if (scene->getGameSystem().isPressed() && m_mouseInCollison && !scene->getGameSystem().m_keyIsPressed && !m_dialogEnd)
        {
            scene->getGameSystem().m_keyIsPressed = true;
            scene->getGameSystem().useVibrate(60);

            if (m_size < static_cast<int>(m_strDialog.size()) - 1)
            {
                m_txtDialog.setString(m_strDialog);
                m_size = static_cast<int>(m_strDialog.size()) - 1;
            }
            else
            {
                m_msgIndex += 2;
                if (m_msgIndex == m_msgIndexMax)
                {
                    scene->getGameSystem().playSound(sndCancel);
                    m_dialogEnd = true;
                }
                else
                {
                    m_newLine = true;
                    m_size = 0;
                    m_txtDialog.setString("");
                }
            }
        }

        if (!m_dialogEnd)
        {
            is::increaseVar(DELTA_TIME, m_imageScale, 0.1f, 1.f, 1.f);
        }
        else
        {
            is::decreaseVar(DELTA_TIME, m_imageScale, 0.1f, 0.f, 0.f);
            if (m_imageScale < 0.05f) m_showDialog = false;
        }

        is::setSFMLObjX_Y(m_sprParent, m_x, m_y - 90.f);
        is::setSFMLObjX_Y(m_txtDialog, is::getSFMLObjX(m_sprParent) - 225.f,
                          is::getSFMLObjY(m_sprParent) - 38.f);
        is::setSFMLObjX_Y(m_sprNext, is::getSFMLObjX(m_sprParent) + 220.f,
                          is::getSFMLObjY(m_sprParent) + 29.f);

        is::setSFMLObjX_Y(m_sprSkip, m_x, m_y + 145.f);
        is::setSFMLObjX_Y(m_txtSkip, m_x, is::getSFMLObjY(m_sprSkip) - ((scene->getGameSystem().m_gameLanguage == 1) ? 4.f : 1.4f));
    }

    is::setSFMLObjScale(m_txtDialog, m_imageScale);
    is::setSFMLObjScale(m_txtSkip, m_imageScale);
    is::setSFMLObjScale(m_sprSkip, m_imageScale);
    is::setSFMLObjScale(m_sprParent, m_imageScale);
    is::setSFMLObjScale(m_sprNext, m_imageScale);
}

void GameDialog::setDialog(DialogIndex dialogIndex)
{
    m_dialogEnd = false;
    m_msgIndex = 0;
    m_size = 0;
    m_dialogIndex = dialogIndex;
    m_strDialog = L"";
    m_txtDialog.setString("");
    m_showDialog = true;
    m_newLine = true;
}

void GameDialog::setMouseInCollison(bool val)
{
    m_mouseInCollison = val;
}

void GameDialog::draw(sf::RenderTexture &surface)
{
    if (m_imageScale > 0.05)
    {
        surface.draw(m_sprParent);
        surface.draw(m_txtDialog);
        surface.draw(m_txtSkip);
        surface.draw(m_sprSkip);
        if (m_blindTime < 18.f && m_size == (static_cast<int>(m_strDialog.size()) - 1)) surface.draw(m_sprNext);
    }
}
