#ifndef GAMEMENU_H_INCLUDED
#define GAMEMENU_H_INCLUDED

#include "../system/function/GameFunction.h"
#include "../system/display/GameDisplay.h"
#include "../language/GameLanguage.h"
#include "../gameplay/widgets/CancelButton.h"
#include "../level/Level.h"

class GameMenu : public is::GameDisplay
{
public:
    enum GamePage
    {
        PAGE_MAIN_MENU,
        PAGE_PAD_CONFIG
    };

    GameMenu(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt);
    ~GameMenu();

    void step();
    void draw();

    bool loadResources();

private:
    std::wstring getABPlace() const
    {
        std::wstring strAnd = L"and";
        if (m_gameSysExt.m_gameLanguage == 1) strAnd = L"et";
        return ((!m_gameSysExt.m_permutePadAB) ? L"A " + strAnd + L" B" : L"B " + strAnd + L" A");
    }
    sf::Font m_fontTitle;

    sf::Texture m_texPad, m_texPad2, m_texToolsPad, m_texScreenBG;
    sf::Texture m_texGamePad;

    sf::Sprite m_sprPad1, m_sprPad2, m_sprPad3, m_sprScreenBG;
    sf::Sprite m_sprJoystick[2], m_sprPermuteAB, m_sprAddAlpha, m_sprReduceAlpha;

    sf::Text m_txtGameTitle, m_txtStartGame, m_txtContinue, m_txtPadConfig;
    sf::Text m_txtSetAlpha, m_txtOptionValue[5], m_txtPermuteAB, m_txtMovePad;

    sf::RectangleShape m_recLine, m_recSelectPad, m_recCfgBg;

    sf::Keyboard::Key m_keyBack;

    CancelButton m_cancelBt;

    int m_gameOptionIndex;
    int const TXT_OPTION_NUMBER;
    float m_oscillation, m_xPos, m_yPos, m_bgX, m_bgY, m_sprButtonPemuteScale, m_timeClick;
    float m_txtOptionScale[6];

    bool m_isClicked, m_padCfgClicked;
    bool m_isStart, m_saveFileOpen;
    bool m_closeApplication;

    GamePage m_pageName;
};

#endif // GAMEMENU_H_INCLUDED
