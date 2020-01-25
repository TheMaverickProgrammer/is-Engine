#include "GameIntro.h"

GameIntro::GameIntro(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt):
    GameDisplay(window, view, surface, m_launchOption, gameSysExt, sf::Color::White),
    m_recScale(0.f),
    m_alphaRec(255),
    m_timeEv(0),
    m_introStep(0),
    m_imgAlpha(0),
    m_blind(false),
    m_isOnPad(false),
    m_openLanguageMenu(false)
{
    // transition rectangle
    is::createRectangle(m_recTransition, sf::Vector2f(m_viewW + 10.f, m_viewH + 10.f), sf::Color(0, 0, 0, m_alphaRec));
    is::setSFMLObjX_Y(m_recTransition, 320.f, 240.f);
    is::createRectangle(m_recChooseLanguage, sf::Vector2f(650.f, 190.f), sf::Color(0, 0, 0), 320.f, 240.f, true);
    is::setSFMLObjScale(m_recChooseLanguage, m_recScale);
}

void GameIntro::step()
{
    // DELTA_TIME
    DELTA_TIME = getDeltaTime();

    // even loop
    while (m_app.pollEvent(m_event)) controlEventFocusClosing();

    // Camera
    setView();

    if (m_appIsActive)
    {
        updateTimeWait(DELTA_TIME);

        // intro step
        switch(m_introStep)
        {
            case 0 :
                if (m_openLanguageMenu)
                {
                    bool mouseInCollison(false);
                    is::increaseVar(DELTA_TIME, m_recScale, 0.08f, 1.f, 1.f);

                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !m_gameSysExt.isPressed()) m_gameSysExt.m_keyIsPressed = false;

                    if (mouseCollision(m_sprButtonSelect)) mouseInCollison = true;
                    else if (mouseCollision(m_sprPadFr)) mouseInCollison = true;
                    else m_isOnPad = false;

                    if (!mouseInCollison && m_gameSysExt.isPressed()) m_gameSysExt.m_keyIsPressed = true;
                    auto tempFnct = [this](int index)
                    {
                        m_optionIndex = index;
                        m_isOnPad = true;
                    };

                    if (!m_isOnPad)
                    {
                        if (mouseCollision(m_sprButtonSelect)) tempFnct(0);
                        else if (mouseCollision(m_sprPadFr)) tempFnct(1);
                    }

                    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || (m_gameSysExt.isPressed() && mouseInCollison)) &&
                        (m_waitTime == 0 && !m_gameSysExt.m_keyIsPressed))
                    {
                        m_gameSysExt.playSound(m_sndSelectOption);
                        m_gameSysExt.useVibrate(m_vibrateTimeDuration);
                        m_gameSysExt.m_gameLanguage = m_optionIndex;
                        m_gameSysExt.m_firstLaunch = false;
                        m_gameSysExt.saveConfig(is::CONFIG_FILE);
                        m_openLanguageMenu = false;
                    }
                }
                else
                {
                    is::decreaseVar(DELTA_TIME, m_recScale, 0.09f, 0.f, 0.f);

                    if (!m_blind)
                    {
                        if (m_imgAlpha < 250) m_imgAlpha += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                        else
                        {
                            m_imgAlpha = 0;
                            m_gameSysExt.useVibrate(m_vibrateTimeDuration);
                            m_sprLogo.setColor(sf::Color(255, 255, 255, m_imgAlpha));
                            is::setSFMLObjTexRec(m_sprLogo, 256, 0, 256, 128);
                            m_blind = true;
                        }
                    }
                    else
                    {
                        if (m_imgAlpha < 240) m_imgAlpha += static_cast<int>((10.f * is::VALUE_CONVERSION) * DELTA_TIME);
                        else m_imgAlpha = 255;
                        m_sprLogo.setColor(sf::Color(255, 255, 255, m_imgAlpha));
                    }

                    if (m_alphaRec > 5) m_alphaRec -= static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                    else
                    {
                        m_alphaRec = 0;
                        if (m_timeEv < 50) m_timeEv += static_cast<int>(is::VALUE_CONVERSION * DELTA_TIME);
                        else m_introStep++;
                    }
                }
            break;

            case 1:
                if (m_alphaRec < 250) m_alphaRec += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                else
                {
                    if (m_timeEv < 55)
                    {
                        m_alphaRec = 255;
                        m_timeEv += static_cast<int>(is::VALUE_CONVERSION * DELTA_TIME);
                        is::setSFMLObjTexRec(m_sprLogo, 0, 128, 512, 128);
                        is::centerSFMLObj(m_sprLogo);
                    }
                    else m_introStep++;
                }
            break;

            case 2:
                if (m_alphaRec > 5) m_alphaRec -= static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                else
                {
                    m_alphaRec = 0;
                    if (m_timeEv < 95) m_timeEv += static_cast<int>(is::VALUE_CONVERSION * DELTA_TIME);
                    else m_introStep++;
                }
            break;

            case 3:
                if (m_alphaRec < 250) m_alphaRec += static_cast<int>((4.f * is::VALUE_CONVERSION) * DELTA_TIME);
                else
                {
                    m_isRunning = false;  // quit the scene main loop
                    m_launchOption = is::GameDisplay::MAIN_MENU;  // go to main menu scene
                    m_introStep++;
                }
            break;
        }

        is::setSFMLObjScale(m_recChooseLanguage, m_recScale);
        if (m_introStep < 3) m_recTransition.setFillColor(sf::Color(255, 255, 255, m_alphaRec));
        else m_recTransition.setFillColor(sf::Color(0, 0, 0, m_alphaRec));
    }
}

void GameIntro::draw()
{
    if (m_recScale > 0.f)
    {
        m_surface.draw(m_recChooseLanguage);
        if (m_openLanguageMenu)
        {
            m_surface.draw(m_sprButtonSelect);
            m_surface.draw(m_sprPadFr);
            m_surface.draw(m_txtChooseLanguage);
            m_surface.draw(m_txtLangEng);
            m_surface.draw(m_txtLangFr);
        }
    }
    else
    {
        m_surface.draw(m_sprLogo);
        if (m_introStep < 3) m_surface.draw(m_recTransition);
    }
}

bool GameIntro::loadResources()
{
    if (!GameDisplay::loadParentResources()) return false;

    m_gameSysExt.loadConfig(is::CONFIG_FILE);

    // load textures
    if (!m_texLogo.loadFromFile(is::GUI_DIR + "start_img.tex")) return false;

    if (m_gameSysExt.m_firstLaunch)
    {
        if (!m_texPad.loadFromFile(is::GUI_DIR + "main_menu_pad.png"))              return false;
        if (!m_fontTitle.loadFromFile(is::FONT_DIR + "space_ranger_3d_mp_pv.otf"))  return false;

        is::createWText(m_fontTitle, m_txtChooseLanguage, L"Choose Language", 0.f, 0.f, sf::Color(255, 255, 255), 48);
        is::centerSFMLObj(m_txtChooseLanguage);
        is::setSFMLObjX_Y(m_txtChooseLanguage, m_viewX, m_viewY - 90.f);

        // create sprites
        float btX(230.f), btY(205.f);
        is::createSprite(m_texPad, m_sprButtonSelect, sf::IntRect(192, 0, 192, 48), sf::Vector2f(btX, btY), sf::Vector2f(96.f, 24.f));
        is::createSprite(m_texPad, m_sprPadFr, sf::IntRect(192, 0, 192, 48), sf::Vector2f(btX, btY + 66.f), sf::Vector2f(96.f, 24.f));
        is::createText(m_fontSystem, m_txtLangEng, "English",
                       is::getSFMLObjX(m_sprButtonSelect), is::getSFMLObjY(m_sprButtonSelect) - 6.f, sf::Color(255, 255, 255), true, 25);
        is::createText(m_fontSystem, m_txtLangFr, "French",
                       is::getSFMLObjX(m_sprPadFr), is::getSFMLObjY(m_sprPadFr) - 6.f, sf::Color(255, 255, 255), true, 25);

        m_openLanguageMenu = true;
    }

    is::createSprite(m_texLogo, m_sprLogo, sf::IntRect(0, 0, 256, 128), sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    is::centerSFMLObj(m_sprLogo);
    m_sprLogo.setPosition(m_viewX, m_viewY);

    return true;
}
