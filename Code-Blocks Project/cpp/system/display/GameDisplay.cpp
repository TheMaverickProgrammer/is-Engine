#include "GameDisplay.h"

namespace is
{
sf::Vector2f getMapPixelToCoords(GameDisplay const *scene, sf::Vector2i pixelPos)
{
    return scene->getRenderWindow().mapPixelToCoords(pixelPos, scene->getView());
}

std::string GameDisplay::answer_yes[] = {"YES", "OUI"};
std::string GameDisplay::answer_no[]  = {"NO", "NON"};

GameDisplay::GameDisplay(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt, sf::Color bgColor):
    m_isClose(false),
    m_launchOption(m_launchOption),
    m_app(window),
    m_view(view),
    m_surface(surface),
    m_gameSysExt(gameSysExt),
    m_vibrateTimeDuration(40),
    m_optionIndex(0),
    m_waitTime(0),
    m_msgWaitTime(0),
    DELTA_TIME(0.f),
    m_viewW(640.f),
    m_viewH(480.f),
    m_viewX(m_viewW / 2.f),
    m_viewY(m_viewH / 2.f),
    m_sprButtonSelectScale(1.f),
    m_isRunning(true),
    m_appIsActive(true),
    m_keyBackPressed(false),
    m_showMsg(false),
    m_mbYesNo(false),
    m_msgBoxMouseInCollison(false)
{
    m_surface.create(m_viewW, m_viewH);
    m_view.setSize(sf::Vector2f(m_viewW, m_viewH));
    setView();
    m_windowBgColor = bgColor;

#if defined(__ANDROID__)
#if defined(ENABLE_ADMOB)
    m_admobManager = 0;
#endif // definded
#endif // defined
}

GameDisplay::~GameDisplay() {}

void is::GameDisplay::setTextAnimation(sf::Text &txt, sf::Sprite &spr, int val)
{
    if (m_optionIndex == val)
    {
        is::setSFMLObjX_Y(m_sprButtonSelect, is::getSFMLObjX(spr), is::getSFMLObjY(spr));
        is::setSFMLTextColor(txt, sf::Color::Blue);
    }
    else is::setSFMLTextColor(txt, sf::Color(0, 0, 0));
}

void is::GameDisplay::setTextAnimation(sf::Text &txt, int &var, int val)
{
    if (var == val) is::setSFMLTextColor(txt, sf::Color::Blue);
    else is::setSFMLTextColor(txt, sf::Color(0, 0, 0));
}

void is::GameDisplay::setView()
{
    m_view.setCenter(m_viewX, m_viewY);
    m_surface.setView(m_view);
}

void is::GameDisplay::controlEventFocusClosing()
{
    // manage the state of window
    if (m_event.type == sf::Event::GainedFocus) m_appIsActive = true;
    if (m_event.type == sf::Event::LostFocus) m_appIsActive = false;

    // closing the application
    if (m_event.type == sf::Event::Closed)
    {
        m_isRunning = false;  // quit the main render loop
        m_app.close();
    }
}

void is::GameDisplay::updateMsgBox(float const &DELTA_TIME, sf::Color textDefaultColor, sf::Color textSelectedColor)
{
    if (m_msgWaitTime < 240) m_msgWaitTime += static_cast<int>((8.f * is::VALUE_CONVERSION) * DELTA_TIME);
    else m_msgWaitTime = 255;
    if (!m_gameSysExt.isPressed()) m_gameSysExt.m_keyIsPressed = false;

    // check collision with all objects of message box
    if (!mouseCollision(m_sprMsgBoxButton1) &&
        !mouseCollision(m_sprMsgBoxButton2) &&
        !mouseCollision(m_sprMsgBoxButton3))
         m_msgBoxMouseInCollison = false;
     else m_msgBoxMouseInCollison = true;

    // avoid the long pressing button effect
    if (!m_msgBoxMouseInCollison && m_gameSysExt.isPressed()) m_gameSysExt.m_keyIsPressed = true;

    if (m_msgWaitTime == 255 && m_appIsActive)
    {
        // if is YES / NO message box
        if (m_mbYesNo)
        {
            if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                  !mouseCollision(m_sprMsgBoxButton2)) ||
                mouseCollision(m_sprMsgBoxButton1)) && m_msgAnswer == MsgAnswer::NO)
            {
                m_gameSysExt.useVibrate(m_vibrateTimeDuration);
                m_gameSysExt.playSound(m_sndSwitch);
                m_msgAnswer = MsgAnswer::YES; // answer = yes
            }
            else if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                     !mouseCollision(m_sprMsgBoxButton1)) ||
                     mouseCollision(m_sprMsgBoxButton2)) && m_msgAnswer == MsgAnswer::YES)
            {
                m_gameSysExt.useVibrate(m_vibrateTimeDuration);
                m_gameSysExt.playSound(m_sndSwitch);
                m_msgAnswer = MsgAnswer::NO;  // answer = no
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) ||
                    ((mouseCollision(m_sprMsgBoxButton1) ||
                      mouseCollision(m_sprMsgBoxButton2)) &&
                      m_gameSysExt.isPressed() && !m_gameSysExt.m_keyIsPressed))
            {
                m_showMsg = false;
            }
            else if (m_keyBackPressed)
            {
                m_msgAnswer = MsgAnswer::NO; // answer = no (canceled)
                m_showMsg = false;
                m_keyBackPressed = false;
            }

            // texts animations
            if (m_msgAnswer == MsgAnswer::YES)
            {
                m_txtMsgBoxYes.setFillColor(textSelectedColor);
                m_txtMsgBoxNo.setFillColor(textDefaultColor);
            }
            else
            {
                m_txtMsgBoxNo.setFillColor(textSelectedColor);
                m_txtMsgBoxYes.setFillColor(textDefaultColor);
            }
        }
        else // if is OK message box
        {
            if (mouseCollision(m_sprMsgBoxButton3) && m_msgAnswer == MsgAnswer::NO)
            {
                m_gameSysExt.useVibrate(m_vibrateTimeDuration);
                m_gameSysExt.playSound(m_sndSwitch);
                m_msgAnswer = MsgAnswer::YES; // answer = OK
                m_txtMsgBoxOK.setFillColor(textSelectedColor);
            }
            else if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || m_keyBackPressed) &&
                     !mouseCollision(m_sprMsgBoxButton3)) || (mouseCollision(m_sprMsgBoxButton3) &&
                     m_gameSysExt.isPressed() && !m_gameSysExt.m_keyIsPressed)
                     )
            {
                m_showMsg = false;
                m_keyBackPressed = false;
            }
            else if (!mouseCollision(m_sprMsgBoxButton3) && m_msgAnswer == MsgAnswer::YES)
            {
                m_msgAnswer = MsgAnswer::NO; // answer = NO
                m_txtMsgBoxOK.setFillColor(textDefaultColor);
            }
        }
    }

    if (m_msgWaitTime != 255)
    {
        if (m_mbYesNo)
        {
            is::setSFMLObjColor(m_sprMsgBoxButton1, sf::Color(255, 255, 255, m_msgWaitTime));
            is::setSFMLObjColor(m_sprMsgBoxButton2, sf::Color(255, 255, 255, m_msgWaitTime));
            is::setSFMLTextColor(m_txtMsgBoxNo, sf::Color(textSelectedColor.r, textSelectedColor.g, textSelectedColor.b, m_msgWaitTime));
            is::setSFMLTextColor(m_txtMsgBoxYes, sf::Color(textDefaultColor.r, textDefaultColor.g, textDefaultColor.b, m_msgWaitTime));
        }
        else
        {
            is::setSFMLObjColor(m_sprMsgBoxButton3, sf::Color(255, 255, 255, m_msgWaitTime));
            is::setSFMLTextColor(m_txtMsgBoxOK, sf::Color(textDefaultColor.r, textDefaultColor.g, textDefaultColor.b, m_msgWaitTime));
        }
    }
    is::setSFMLObjColor(m_sprMsgBox, sf::Color(255, 255, 255, m_msgWaitTime));
    is::setSFMLTextColor(m_txtMsgBox, sf::Color(textDefaultColor.r, textDefaultColor.g, textDefaultColor.b, m_msgWaitTime));

    if (!m_showMsg)
    {
        if (m_msgAnswer == MsgAnswer::NO)
        {
            // if is OK message box the answer is automatically YES
            if (!m_mbYesNo)
            {
                m_msgAnswer = MsgAnswer::YES;
                m_gameSysExt.playSound(m_sndSelectOption);
                m_gameSysExt.useVibrate(m_vibrateTimeDuration);
            }
            else m_gameSysExt.playSound(m_sndCancel);
        }
        else
        {
            m_gameSysExt.playSound(m_sndSelectOption);
            m_gameSysExt.useVibrate(m_vibrateTimeDuration);
        }
    }
}

void GameDisplay::updateTimeWait(float const &DELTA_TIME)
{
    // waiting time before validating an option
    if (m_waitTime > 0)
    {
        m_waitTime -= is::getMSecond(DELTA_TIME);
    }
    else m_waitTime = 0;
}

void is::GameDisplay::drawMsgBox()
{
    if (m_showMsg)
    {
        m_surface.draw(m_recMsgBox);
        m_surface.draw(m_sprMsgBox);

        if (m_mbYesNo)
        {
            m_surface.draw(m_txtMsgBoxYes);
            m_surface.draw(m_txtMsgBoxNo);
            m_surface.draw(m_sprMsgBoxButton1);
            m_surface.draw(m_sprMsgBoxButton2);
        }
        else
        {
            m_surface.draw(m_txtMsgBoxOK);
            m_surface.draw(m_sprMsgBoxButton3);
        }

        m_surface.draw(m_txtMsgBox);
    }
}

void is::GameDisplay::drawScreen()
{
    m_surface.clear(m_windowBgColor);
    draw();
}

void is::GameDisplay::showTempLoading(float time)
{
    float timeToQuit(0.f);
    sf::Texture texTmploading, texTmploading2;
    texTmploading.loadFromFile(is::GUI_DIR + "temp_loading.png");
    texTmploading2.loadFromFile(is::GUI_DIR + "loading_icon.png");
    sf::Sprite sprTmploading, sprTmploading2;
    is::createSprite(texTmploading, sprTmploading, sf::IntRect(0, 0, 640, 480),
                     sf::Vector2f(0.f, 0.f), sf::Vector2f(320.f, 240.f));
    is::createSprite(texTmploading2, sprTmploading2, sf::IntRect(0, 0, 32, 32),
                     sf::Vector2f(304.f, 240.f), sf::Vector2f(16.f, 16.f));
    while (timeToQuit < time)
    {
        float dTime = getDeltaTime();
        timeToQuit += is::getMSecond(dTime);
        sprTmploading2.rotate((5.f * is::VALUE_CONVERSION) * dTime);
        sf::Event ev;
        while (m_app.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                m_app.close();
                #if defined (__ANDROID__)
                std::terminate();
                #else
                m_isRunning = false;
                #endif // defined
            }
        }
        m_app.clear(sf::Color::Black);
        m_surface.draw(sprTmploading);
        m_surface.draw(sprTmploading2);
        m_app.display();
    }
}

bool is::GameDisplay::loadParentResources()
{
    // load sound
    if (!m_sbWitch.loadFromFile(is::SFX_DIR + "change_option.ogg"))             return false;
    if (!m_sbCancel.loadFromFile(is::SFX_DIR + "cancel.ogg"))                   return false;
    if (!m_sbSelectOption.loadFromFile(is::SFX_DIR + "select_option.ogg"))      return false;

    m_sndSwitch.setBuffer(m_sbWitch);
    m_sndCancel.setBuffer(m_sbCancel);
    m_sndSelectOption.setBuffer(m_sbSelectOption);

    // load message box sprite
    if (!m_texMsgBox.loadFromFile(is::GUI_DIR + "confirm_box.png"))           return false;
    if (!m_sprMsgButton.loadFromFile(is::GUI_DIR + "confirm_box_button.png")) return false;

    is::createSprite(m_texMsgBox, m_sprMsgBox, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    is::createSprite(m_sprMsgButton, m_sprMsgBoxButton1, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    is::createSprite(m_sprMsgButton, m_sprMsgBoxButton2, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));
    is::createSprite(m_sprMsgButton, m_sprMsgBoxButton3, sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f));

    is::createRectangle(m_recMsgBox, sf::Vector2f(m_viewW + 40.f, m_viewH + 40.f), sf::Color(0, 0, 0, 200), 0.f, 0.f);

    is::centerSFMLObj(m_sprMsgBox);
    is::centerSFMLObj(m_sprMsgBoxButton1);
    is::centerSFMLObj(m_sprMsgBoxButton2);
    is::centerSFMLObj(m_sprMsgBoxButton3);

    // load font
    if (!m_fontSystem.loadFromFile(FONT_DIR + "sansation.ttf")) return false;
    if (!m_fontMsg.loadFromFile(FONT_DIR + "brush_script_std_medium.ttf")) return false;

    is::createText(m_fontSystem, m_txtMsgBox, "", 0.f, 0.f, 20);
    is::createText(m_fontSystem, m_txtMsgBoxYes, "YES", 0.f, 0.f, true, 18);
    is::createText(m_fontSystem, m_txtMsgBoxNo, "NO", 0.f, 0.f, true, 18);
    is::createText(m_fontSystem, m_txtMsgBoxOK, "OK", 0.f, 0.f, true, 18);
    is::centerSFMLObj(m_txtMsgBoxYes);
    is::centerSFMLObj(m_txtMsgBoxNo);
    return true;
}

float is::GameDisplay::getDeltaTime()
{
    float dt = m_clock.restart().asSeconds();
    if (dt > is::MAX_CLOCK_TIME) dt = is::MAX_CLOCK_TIME;
    return dt;
}

bool is::GameDisplay::isRunning() const
{
    return m_isRunning;
}
}
