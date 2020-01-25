#ifndef GAMEDISPLAY_H_INCLUDED
#define GAMEDISPLAY_H_INCLUDED

#include "../function/GameFunction.h"
#include "../../core/GameSystemExtended.h"
#if defined(__ANDROID__)
#if defined(ENABLE_ADMOB)
#include "../android/AdmobManager.h"
#endif
#endif // defined

namespace is
{
//////////////////////////////////////////////////////
/// \brief Class for manage game scene
//////////////////////////////////////////////////////
class GameDisplay;

sf::Vector2f getMapPixelToCoords(GameDisplay const *scene, sf::Vector2i pixelPos);

class GameDisplay
{
public:
    static std::string answer_yes[];
    static std::string answer_no[];

    /// represent the state of scene
    enum DisplayOption
    {
        INTRO,
        RESTART_LEVEL,
        GAME_OPTION_RESTART, // when player restart the level
        NEXT_LEVEL,
        MAIN_MENU,
        GAME_LEVEL,
        GAME_OVER,
        GAME_END_SCREEN
    };

    bool m_isClose;
    DisplayOption &m_launchOption;

    GameDisplay(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt, sf::Color bgColor);
    virtual ~GameDisplay();

#if defined(__ANDROID__)
#if defined(ENABLE_ADMOB)
    virtual void setAdmob(AdmobManager *admob)
    {
        m_admobManager = admob;
    }

    virtual int rewardVideoStep()
    {
        int result(0);
        if (checkAdState(firebase::admob::rewarded_video::LoadAdLastResult()))
        {
            bool stopGameTread(true);
            firebase::admob::rewarded_video::Show(m_admobManager->m_activity->clazz);
            showTempLoading();

            if (checkAdState(firebase::admob::rewarded_video::ShowLastResult()))
            {
                while (stopGameTread)
                {
                    float dTime = getDeltaTime();
                    if (firebase::admob::rewarded_video::presentation_state() ==
                        firebase::admob::rewarded_video::kPresentationStateHidden)
                        stopGameTread = false;
                    sf::Event ev;
                    while (m_app.pollEvent(ev))
                    {
                        if (ev.type == sf::Event::Closed)
                        {
                            m_app.close();
                            std::terminate();
                        }
                    }
                    m_app.clear(sf::Color::Black);
                    m_app.display();
                }

                // end of video
                result = 1;
                m_admobManager->checkAdRewardObjReinitialize();
            }
        }
        else m_admobManager->loadRewardVideo();
        return result;
    }
#endif // definded
#endif // defined

    /// update scene behavior
    virtual void step() = 0;

    /// draw scene
    virtual void drawScreen();

    /// draw temporal loading (simulation)
    virtual void showTempLoading(float time = 3.f * 59.f);

    /// load message box resource and fonts
    virtual bool loadParentResources();

    /// load scene resources
    virtual bool loadResources() = 0;

    /// check is scene is running
    virtual bool isRunning() const;

    /// return view
    virtual sf::View& getView() const {return m_view;}

    /// return render window
    virtual sf::RenderWindow& getRenderWindow() const {return m_app;}

    /// return render texture
    virtual sf::RenderTexture& getRenderTexture() const {return m_surface;}

    /// return game system controller
    virtual GameSystemExtended& getGameSystem() {return m_gameSysExt;}

    /// return view X
    virtual float getViewX() const {return m_viewX;}

    /// return view Y
    virtual float getViewY() const {return m_viewY;}

    /// return view W
    virtual float getViewW() const {return m_viewW;}

    /// return view H
    virtual float getViewH() const {return m_viewH;}

    /// return the scene background color
    virtual sf::Color& getBgColor() {return m_windowBgColor;}

    /// test collision on SFML object (Mouse on PC - Touch on mobile)
    template <class T>
    bool mouseCollision(T const &obj
                        #if defined(__ANDROID__)
                        , unsigned int finger = 0
                        #endif // defined
                        )
    {
        sf::RectangleShape rec(sf::Vector2f(3.f, 3.f));
        is::centerSFMLObj(rec);

        #if defined(__ANDROID__)
        sf::Vector2i pixelPos = sf::Touch::getPosition(finger, m_app);
        #else
        sf::Vector2i pixelPos = sf::Mouse::getPosition(m_app);
        #endif // defined

        sf::Vector2f worldPos = getMapPixelToCoords(this, pixelPos);
        float dx = pointDistance(m_viewX, m_viewY, worldPos.x, m_viewY);
        float dy = pointDistance(m_viewX, m_viewY, m_viewX, worldPos.y);

        if (worldPos.x < m_viewX) dx *= -1;
        if (worldPos.y < m_viewY) dy *= -1;

        rec.setPosition(m_viewX + dx, m_viewY + dy);
        if (obj.getGlobalBounds().intersects(rec.getGlobalBounds())) return true;
        else return false;
    }

    /// test collision on SFML object and get cursor object (Mouse on PC - Touch on mobile)
    template <class T>
    bool mouseCollision(T const &obj, sf::RectangleShape &rec
                        #if defined(__ANDROID__)
                        , unsigned int finger = 0
                        #endif // defined
                        )
    {
        // if size is empty set default value
        if (static_cast<int>(is::getSFMLObjWidth(rec)) == 0 && static_cast<int>(is::getSFMLObjHeight(rec)) == 0) rec.setSize(sf::Vector2f(3.f, 3.f));
        is::centerSFMLObj(rec);

        #if defined(__ANDROID__)
        sf::Vector2i pixelPos = sf::Touch::getPosition(finger, m_app);
        #else
        sf::Vector2i pixelPos = sf::Mouse::getPosition(m_app);
        #endif // defined

        sf::Vector2f worldPos = getMapPixelToCoords(this, pixelPos);
        float dx = pointDistance(m_viewX, m_viewY, worldPos.x, m_viewY);
        float dy = pointDistance(m_viewX, m_viewY, m_viewX, worldPos.y);

        if (worldPos.x < m_viewX) dx *= -1;
        if (worldPos.y < m_viewY) dy *= -1;

        rec.setPosition(m_viewX + dx, m_viewY + dy);
        if (obj.getGlobalBounds().intersects(rec.getGlobalBounds())) return true;
        else return false;
    }
protected:
    /// represent the answers return by message box
    enum MsgAnswer
    {
        QUIT = -1,
        YES = 1,
        NO = 0
    };

    /// return the delta time elapsed
    float getDeltaTime();

    /// allow to manage focus and closing events
    void controlEventFocusClosing();

    /// show message box according to type
    template<class T>
    void showMessageBox(T const &msgBody, bool mbYesNo = true)
    {
        m_showMsg = true;
        m_mbYesNo = mbYesNo;
        if (m_mbYesNo) m_msgAnswer = MsgAnswer::NO;
        m_msgWaitTime = 0;
        m_msgBoxMouseInCollison = false;
        m_txtMsgBoxYes.setString(answer_yes[m_gameSysExt.m_gameLanguage]);
        m_txtMsgBoxNo.setString(answer_no[m_gameSysExt.m_gameLanguage]);

        is::setSFMLObjX_Y(m_recMsgBox, sf::Vector2f(m_view.getCenter().x, m_view.getCenter().y));
        is::setSFMLObjX_Y(m_sprMsgBox, sf::Vector2f(m_view.getCenter().x, m_view.getCenter().y));
        const float dim(6.f),
                    boxXOrigin(is::getSFMLObjOriginX(m_sprMsgBox)),
                    boxYOrigin(is::getSFMLObjOriginY(m_sprMsgBox));
        is::setSFMLObjX_Y(m_sprMsgBoxButton1,
                          is::getSFMLObjX(m_sprMsgBox) - boxXOrigin + is::getSFMLObjOriginX(m_sprMsgBoxButton1) + dim,
                          is::getSFMLObjY(m_sprMsgBox) + boxYOrigin - is::getSFMLObjHeight(m_sprMsgBoxButton1) + dim);
        is::setSFMLObjX_Y(m_sprMsgBoxButton2,
                          is::getSFMLObjX(m_sprMsgBox) + boxXOrigin - is::getSFMLObjOriginX(m_sprMsgBoxButton2) - dim,
                          is::getSFMLObjY(m_sprMsgBox) + boxYOrigin - is::getSFMLObjHeight(m_sprMsgBoxButton2) + dim);
        is::setSFMLObjX_Y(m_sprMsgBoxButton3,
                          is::getSFMLObjX(m_sprMsgBox),
                          is::getSFMLObjY(m_sprMsgBox) + boxYOrigin - is::getSFMLObjHeight(m_sprMsgBoxButton1) + dim);
        is::setSFMLObjX_Y(m_txtMsgBox,
                          is::getSFMLObjX(m_sprMsgBox)- boxXOrigin + 16.f,
                          is::getSFMLObjY(m_sprMsgBox) - boxYOrigin + 8.f);
        m_txtMsgBox.setString(msgBody);

        // adjust the text on button
        float const adjustVal(8.f);
        is::setSFMLObjX_Y(m_txtMsgBoxYes, is::getSFMLObjX(m_sprMsgBoxButton1),
                          is::getSFMLObjY(m_sprMsgBoxButton1) - adjustVal);
        is::setSFMLObjX_Y(m_txtMsgBoxNo, is::getSFMLObjX(m_sprMsgBoxButton2),
                          is::getSFMLObjY(m_sprMsgBoxButton2) - adjustVal);
        is::setSFMLObjX_Y(m_txtMsgBoxOK, is::getSFMLObjX(m_sprMsgBoxButton3),
                          is::getSFMLObjY(m_sprMsgBoxButton3) - adjustVal);
        if (m_gameSysExt.m_gameLanguage == 1) is::setSFMLObjX(m_txtMsgBoxNo, is::getSFMLObjX(m_txtMsgBoxNo) - 6.5f);

        is::setSFMLObjAlpha(m_sprMsgBoxButton1, m_msgWaitTime, 255, 255, 255);
        is::setSFMLObjAlpha(m_sprMsgBoxButton2, m_msgWaitTime, 255, 255, 255);
        is::setSFMLObjAlpha(m_sprMsgBoxButton3, m_msgWaitTime, 255, 255, 255);
        is::setSFMLObjAlpha(m_sprMsgBox, m_msgWaitTime, 255, 255, 255);
        is::setSFMLTextAlpha(m_txtMsgBoxNo, m_msgWaitTime, 255, 255, 255);
        is::setSFMLTextAlpha(m_txtMsgBoxYes, m_msgWaitTime, 0, 255, 0);
        is::setSFMLTextAlpha(m_txtMsgBoxOK, m_msgWaitTime, 0, 255, 0);
        is::setSFMLTextAlpha(m_txtMsgBox, m_msgWaitTime, 0, 0, 0);

        setView();
    }

    /// update message box components
    void updateMsgBox(float const &DELTA_TIME, sf::Color textDefaultColor = sf::Color::White, sf::Color textSelectedColor = sf::Color::Red);

    /// update time wait
    void updateTimeWait(float const &DELTA_TIME);

    /// show message box
    void drawMsgBox();

    /// allow to animate SFML text and sprite in relation to a option
    void setTextAnimation(sf::Text &txt, sf::Sprite &spr, int val);

    /// allow to animate SFML text in relation to a option
    void setTextAnimation(sf::Text &txt, int &var, int val);

    /// update view position
    void setView();

    /// abstract method to implement drawing code
    virtual void draw() = 0;

    sf::RenderWindow &m_app;
    sf::View &m_view;

    sf::RenderTexture &m_surface;
    GameSystemExtended &m_gameSysExt;

    sf::Font m_fontSystem, m_fontMsg;
    sf::Sprite m_sprButtonSelect;
    sf::Clock m_clock;
    sf::Color m_windowBgColor;
    sf::SoundBuffer m_sbWitch, m_sbCancel, m_sbSelectOption;
    sf::Sound m_sndSwitch, m_sndCancel, m_sndSelectOption;

    sf::Event m_event;

    // represent the time of vibration (ms)
    short const m_vibrateTimeDuration;

    int m_optionIndex;
    int m_waitTime, m_msgWaitTime;
    float DELTA_TIME;
    float m_viewW, m_viewH, m_viewX, m_viewY, m_sprButtonSelectScale;
    MsgAnswer m_msgAnswer;

    bool m_isRunning;
    bool m_appIsActive;
    bool m_keyBackPressed;
    bool m_showMsg, m_mbYesNo, m_msgBoxMouseInCollison;

    sf::Texture m_texMsgBox, m_sprMsgButton;
    sf::Sprite m_sprMsgBox, m_sprMsgBoxButton1, m_sprMsgBoxButton2, m_sprMsgBoxButton3;
    sf::Text m_txtMsgBox, m_txtMsgBoxYes, m_txtMsgBoxNo, m_txtMsgBoxOK;
    sf::RectangleShape m_recMsgBox, m_recCursor;

#if defined(__ANDROID__)
#if defined(ENABLE_ADMOB)
    AdmobManager *m_admobManager;
#endif // defined
#endif // defined
};

/// test collision on SFML object (Mouse on PC - Touch on mobile)
template <class T>
bool mouseCollision(GameDisplay &scene, T const &obj
                    #if defined(__ANDROID__)
                    , unsigned int finger = 0
                    #endif // defined
                    )
{
    sf::RectangleShape rec(sf::Vector2f(3.f, 3.f));
    is::centerSFMLObj(rec);

    #if defined(__ANDROID__)
    sf::Vector2i pixelPos = sf::Touch::getPosition(finger, scene.getRenderWindow());
    #else
    sf::Vector2i pixelPos = sf::Mouse::getPosition(scene.getRenderWindow());
    #endif // defined

    sf::Vector2f worldPos = getMapPixelToCoords(&scene, pixelPos);
    float dx = pointDistance(scene.getViewX(), scene.getViewY(), worldPos.x, scene.getViewY());
    float dy = pointDistance(scene.getViewX(), scene.getViewY(), scene.getViewX(), worldPos.y);

    if (worldPos.x < scene.getViewX()) dx *= -1;
    if (worldPos.y < scene.getViewY()) dy *= -1;

    rec.setPosition(scene.getViewX() + dx, scene.getViewY() + dy);
    if (obj.getGlobalBounds().intersects(rec.getGlobalBounds())) return true;
    else return false;
}

/// test collision on SFML object and get cursor object (Mouse on PC - Touch on mobile)
template <class T>
bool mouseCollision(GameDisplay &scene, T const &obj, sf::RectangleShape &rec
                    #if defined(__ANDROID__)
                    , unsigned int finger = 0
                    #endif // defined
                    )
{
    // if size is empty set default value
    if (static_cast<int>(is::getSFMLObjWidth(rec)) == 0 && static_cast<int>(is::getSFMLObjHeight(rec)) == 0) rec.setSize(sf::Vector2f(3.f, 3.f));
    is::centerSFMLObj(rec);

    #if defined(__ANDROID__)
    sf::Vector2i pixelPos = sf::Touch::getPosition(finger, scene.getRenderWindow());
    #else
    sf::Vector2i pixelPos = sf::Mouse::getPosition(scene.getRenderWindow());
    #endif // defined

    sf::Vector2f worldPos = getMapPixelToCoords(&scene, pixelPos);
    float dx = pointDistance(scene.getViewX(), scene.getViewY(), worldPos.x, scene.getViewY());
    float dy = pointDistance(scene.getViewX(), scene.getViewY(), scene.getViewX(), worldPos.y);

    if (worldPos.x < scene.getViewX()) dx *= -1;
    if (worldPos.y < scene.getViewY()) dy *= -1;

    rec.setPosition(scene.getViewX() + dx, scene.getViewY() + dy);
    if (obj.getGlobalBounds().intersects(rec.getGlobalBounds())) return true;
    else return false;
}
}

#endif // GAMEDISPLAY_H_INCLUDED
