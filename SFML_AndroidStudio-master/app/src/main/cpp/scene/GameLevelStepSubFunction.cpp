#include "GameLevel.h"

void GameLevel::joystickController(const float &DELTA_TIME)
{
    if (!m_gameSysExt.validationKeyPressed()) m_gameSysExt.m_keyIsPressed = false;
    if (!m_gameKey.keyAPressed()) m_gameKey.m_keyAUsed = false;
    if (!m_gameKey.keyBPressed()) m_gameKey.m_keyBUsed = false;

    m_gameKey.m_keyLeftPressed = m_gameKey.keyLeftPressed();
    m_gameKey.m_keyRightPressed = m_gameKey.keyRightPressed();
    m_gameKey.m_keyUpPressed = m_gameKey.keyUpPressed();
    m_gameKey.m_keyDownPressed = m_gameKey.keyDownPressed();
    m_gameKey.m_keyAPressed = m_gameKey.keyAPressed();
    m_gameKey.m_keyBPressed = m_gameKey.keyBPressed();

    if (m_gameKey.m_keyLeftPressed)
    {
        m_gameKey.m_keyRightPressed = false;
        m_gameKey.m_keyUpPressed = false;
        m_gameKey.m_keyDownPressed = false;
    }
    else if (m_gameKey.m_keyRightPressed)
    {
        m_gameKey.m_keyLeftPressed = false;
        m_gameKey.m_keyUpPressed = false;
        m_gameKey.m_keyDownPressed = false;
    }
    else if (m_gameKey.m_keyUpPressed)
    {
        m_gameKey.m_keyLeftPressed = false;
        m_gameKey.m_keyRightPressed = false;
        m_gameKey.m_keyDownPressed = false;
    }
    else if (m_gameKey.m_keyDownPressed)
    {
        m_gameKey.m_keyLeftPressed = false;
        m_gameKey.m_keyRightPressed = false;
        m_gameKey.m_keyUpPressed = false;
    }
    m_gameKey.step(DELTA_TIME);
}

void GameLevel::pauseGame()
{
    if ((((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || m_keyBackPressed) && !m_gameKey.m_keyPausePressed) ||
         (m_gameSysExt.isPressed() && m_cancelBt.isOn())) &&
         (m_levelStart && !m_levelEnd && m_finishObject.getStep() == 0 && m_playerHealth!= 0))
    {
        m_gameSysExt.playSound(m_sndSelectOption);
        if (!m_keyBackPressed) m_gameSysExt.useVibrate(m_vibrateTimeDuration);
        stopSounds(true);
        m_isPlaying = false;
        m_gameKey.m_keyPausePressed = true;
        m_keyBackPressed = false;
    }
}

void GameLevel::pauseObjUpdate(const float &DELTA_TIME, bool useMoveAnimation)
{
    if (useMoveAnimation)
    {
        if (m_pauseObjMove > 35.f) m_pauseObjMove -= ((30.f * is::VALUE_CONVERSION) * DELTA_TIME);
        else m_pauseObjMove = 0.f;
    }
    else m_pauseObjMove = 480.f;

    is::setSFMLObjX_Y(m_sprPad1, m_viewX + m_pauseObjMove, m_viewY - 64.f);
    is::setSFMLObjX_Y(m_sprPad2, m_viewX + (160.f + m_pauseObjMove), m_viewY);
    is::setSFMLObjX_Y(m_sprPad3, m_viewX - (160.f - m_pauseObjMove), m_viewY);
    if (!useMoveAnimation) is::setSFMLObjX_Y(m_sprButtonSelect, m_viewX + m_pauseObjMove, m_viewY);

    float const TXT_Y_POS(6.f);
    is::setSFMLObjX_Y(m_txtContinue, is::getSFMLObjX(m_sprPad1), is::getSFMLObjY(m_sprPad1) - TXT_Y_POS);
    is::setSFMLObjX_Y(m_txtRestart, is::getSFMLObjX(m_sprPad2), is::getSFMLObjY(m_sprPad2) - TXT_Y_POS);
    is::setSFMLObjX_Y(m_txtQuit, is::getSFMLObjX(m_sprPad3), is::getSFMLObjY(m_sprPad3) - TXT_Y_POS);

    m_pauseOption->setPosition(m_viewX - 10.f, (m_viewY - 175.f) - m_pauseObjMove);
    is::setSFMLObjX_Y(m_recPauseBG, m_viewX, m_viewY);
}

void GameLevel::backgroundUpdate(const float &DELTA_TIME)
{
    if ((is::isBetween(CURRENT_LEVEL, LEVEL_1, LEVEL_3)))
    {
        is::setSFMLObjX_Y(m_sprLevelBg, m_viewX * 0.98f - 390.f, m_viewY - 240.f);
    }
}

void GameLevel::stopSounds(bool val)
{
    if (m_gameSysExt.m_enableSound)
    {
        if (val)
        {
            if (m_sndLose.getStatus() == sf::Sound::Playing)      m_sndLose.pause();
            if (m_sndPlayerWin.getStatus() == sf::Sound::Playing) m_sndPlayerWin.pause();
        }
        else
        {
            if (m_sndLose.getStatus() == sf::Sound::Paused)      m_gameSysExt.playSound(m_sndLose);
            if (m_sndPlayerWin.getStatus() == sf::Sound::Paused) m_gameSysExt.playSound(m_sndPlayerWin);
        }
        m_player.stopAllSounds(val);
    }
    if (m_gameSysExt.m_enableMusic)
    {
        if (val)
        {
            if (m_mscLevel.getStatus() == sf::Sound::Playing) m_mscLevel.pause();
        }
        else
        {
            if (m_mscLevel.getStatus() == sf::Sound::Paused) m_mscLevel.play();
        }
    }
}

float GameLevel::getPlayerOtherObjPtDir(is::MainObject const *obj)
{
    return is::pointDirection<float>(m_player.getSpriteX(), m_player.getSpriteY(), obj->getSpriteX(), obj->getSpriteY());
}

void GameLevel::playerHurt(bool canContinue)
{
    if (canContinue)
    {
        if (m_playerHealth > 1)
        {
            m_gameSysExt.playSound(m_sndHurt);
            m_gameSysExt.useVibrate(60);
            m_playerHealth--;
            m_playerInvicibilityTime = 60;
        }
        else
        {
            if (m_player.getIsActive())
            {
                m_gameSysExt.playSound(m_sndLose);
                m_gameSysExt.useVibrate(60);
                m_playerHealth= 0;
                m_restartTime++;
                m_player.setIsActive(false);
            }
        }
    }
    else
    {
        if (m_player.getIsActive())
        {
            m_gameSysExt.playSound(m_sndLose);
            m_gameSysExt.useVibrate(60);
            m_playerHealth= 0;
            m_restartTime++;
            m_player.setIsActive(false);
        }
    }
}

bool GameLevel::clearObject(float const &x) const
{
    return (x < m_viewX - 3200.f);
}

bool GameLevel::isInPlayerRay(is::MainObject *obj) const
{
    return (is::isBetween(m_player.getSpriteX(), obj->getX() + 500.f, obj->getX() - 500.f));
}

bool GameLevel::playerIsVulnerable() const
{
    return (m_playerInvicibilityTime == 0 && m_player.getIsActive());
}
