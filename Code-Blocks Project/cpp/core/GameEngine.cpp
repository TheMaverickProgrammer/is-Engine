#include "GameEngine.h"

GameEngine::GameEngine():
    m_launchOption(is::GameDisplay::DisplayOption::INTRO)
    {}

GameEngine::~GameEngine()
{
    #if defined(__ANDROID__)
    /// uncomment to active lock screen on Android
    // is::setScreenLock(false);
    #endif // defined
}

bool GameEngine::initEngine()
{
    m_gameSysExt.initSystemData();
    m_gameSysExt.initProgress();
    m_window.create(sf::VideoMode(640, 480), is::GAME_NAME, sf::Style::Close);

    #if !defined(__ANDROID__)
    // load application icon
    sf::Image iconTex;
    if (!iconTex.loadFromFile(is::GUI_DIR + "icon.png")) return false;
    m_window.setIcon(32, 32, iconTex.getPixelsPtr());

    // create saving directory
    if (!m_gameSysExt.fileExist(is::CONFIG_FILE))
    {
        #if !defined(LINUX_VERSION)
        mkdir("save");
        #else
        mkdir("save", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #endif
        m_gameSysExt.saveConfig(is::CONFIG_FILE);
    }
    #else
        /// uncomment to disable lock screen on Android
        // is::setScreenLock(true);
    #endif // defined

    m_window.setFramerateLimit(60);
    return true;
}

bool GameEngine::play()
{
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      GAME INTILISATION
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!initEngine()) return false;

    #if defined(__ANDROID__)
    #if defined(ENABLE_ADMOB)
    ANativeActivity* activity = sf::getNativeActivity();
    JNIEnv* env = activity->env;
    JavaVM* vm = activity->vm;
    vm->AttachCurrentThread(&env, NULL);

    AdmobManager *admobManager = 0;
    admobManager = new AdmobManager(m_app, m_gameSysExt, activity, env, vm);
    admobManager->checkAdObjInit();
    #endif // definded
    #endif // defined

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         GAME STARTUP
//////////////////////////////////////////////////////////////////////////////////////////////////////
    m_gameSysExt.m_currentLevel = 0;
    ActivityController app(m_window);
    app.push<GameActivity>(m_launchOption, m_gameSysExt);

    // run the program as long as the window is open
    float elapsed = 0.0f;
    sf::Clock clock;

    srand((unsigned int)time(0));

    while (m_window.isOpen())
    {
        clock.restart();
        m_window.clear();
        app.update(elapsed);
        app.draw();
        m_window.display();
        elapsed = static_cast<float>(clock.getElapsedTime().asSeconds());
    }

    #if defined(__ANDROID__)
    #if defined(ENABLE_ADMOB)
     if (is::instanceExist(admobManager))
     {
         delete admobManager;
         admobManager = 0;
     }
    #endif
    #endif // defined
    return true;
}
