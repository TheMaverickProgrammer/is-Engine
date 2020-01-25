#include "../function/GameSystem.h"

namespace is
{
GameSystem::GameSystem()
{
    m_disableKey = false;
    m_enableSound = true;
    m_enableMusic = true;
    m_enableVibrate = true;
    m_keyIsPressed = false;
    m_keyMouse = sf::Mouse::Left;
}

bool GameSystem::validationKeyPressed(
                               #if defined(__ANDROID__)
                               unsigned int finger
                               #endif // defined
                               ) const
{
    if (m_disableKey) return false;
    //////////////////////////////////////////////////////////
    // Android version code
    #if defined(__ANDROID__)
    if (sf::Touch::isDown(finger)) return true;
    #else
    //////////////////////////////////////////////////////////
    if (sf::Mouse::isButtonPressed(m_keyMouse)) return true;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) return true;
    #endif // defined
    else return false;
}

bool GameSystem::isPressed(
                           #if defined(__ANDROID__)
                           unsigned int finger
                           #endif // defined
                           ) const
{
    if (m_disableKey) return false;
    //////////////////////////////////////////////////////////
    // Android version code
    #if defined(__ANDROID__)
    if (sf::Touch::isDown(finger)) return true;
    #else
    //////////////////////////////////////////////////////////
    if (sf::Mouse::isButtonPressed(m_keyMouse)) return true;
    #endif // defined
    else return false;
}

bool GameSystem::fileExist(std::string const &fileName) const
{
    std::ifstream file(fileName.c_str());
    return !file.fail();
}

void GameSystem::playSound(sf::Sound &m_snd)
{
    if (m_enableSound) m_snd.play();
}

void GameSystem::stopSound(sf::Sound &m_snd)
{
    if (m_enableSound)
    {
        if (m_snd.getStatus() == sf::Sound::Playing) m_snd.stop();
    }
}

void GameSystem::useVibrate(short ms)
{
    if (m_enableVibrate) is::vibrate(sf::milliseconds(ms));
}
}
