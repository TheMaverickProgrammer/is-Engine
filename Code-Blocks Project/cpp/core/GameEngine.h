#ifndef GAMEENGINE_H_INCLUDED
#define GAMEENGINE_H_INCLUDED

#include "../ext_lib/Swoosh/Swoosh/ActivityController.h"
#include "../activity/GameActivity.h"

////////////////////////////////////////////////////////////
// PC version code
#if !defined(__ANDROID__)
#if !defined(LINUX_VERSION)
#include <direct.h>
#else
#include <sys/stat.h>
#endif // defined
#else
#if defined(ENABLE_ADMOB)
#include "../system/android/AdmobManager.h"
#endif
#endif // defined
////////////////////////////////////////////////////////////

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    bool initEngine();
    bool play();

private:
    sf::RenderWindow m_window;
    GameSystemExtended m_gameSysExt;
    is::GameDisplay::DisplayOption m_launchOption;
};

#endif // GAMEENGINE_H_INCLUDED
