#ifndef GAMEKEYDATA_H_INCLUDED
#define GAMEKEYDATA_H_INCLUDED

#include "../entity/MainObject.h"

class GameKeyData : public is::MainObject
{
public:
    enum VirtualKeyIndex
    {
        V_KEY_LEFT,
        V_KEY_RIGHT,
        V_KEY_UP,
        V_KEY_DOWN,
        V_KEY_A,
        V_KEY_B,
        V_KEY_NONE
    };

    bool m_keyPausePressed,
         m_keyLeftPressed,
         m_keyRightPressed,
         m_keyUpPressed,
         m_keyDownPressed,
         m_keyAPressed,
         m_keyBPressed,
         m_keyAUsed,
         m_keyBUsed,
         m_disableAllKey,
         m_hideGamePad;

    sf::Keyboard::Key m_keyboardA,
        m_keyboardB,
        m_keyboardLeft,
        m_keyboardRight,
        m_keyboardUp,
        m_keyboardDown;

    VirtualKeyIndex m_moveKeyPressed, m_actionKeyPressed;

    GameKeyData(is::GameDisplay *scene);

    void loadResources(sf::Texture &tex);
    void step(float const &DELTA_TIME);
    void draw(sf::RenderTexture &surface);

    bool keyLeftPressed();
    bool keyRightPressed();
    bool keyUpPressed();
    bool keyDownPressed();
    bool keyAPressed();
    bool keyBPressed();

private:
    bool virtualKeyPressed(VirtualKeyIndex virtualKeyIndex);
    float m_moveObj;
    is::GameDisplay *m_scene;

    sf::Sprite m_sprJoystick[2];
    sf::RectangleShape m_recJoystickMask[2];
    sf::RectangleShape m_recKeyLeftMask, m_recKeyRightMask, m_recKeyUpMask, m_recKeyDownMask, m_recKeyAMask, m_recKeyBMask;
    sf::RectangleShape m_recLeft, m_recRight;
};

#endif // GAMEKEYDATA_H_INCLUDED
