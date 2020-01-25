#ifndef GAMEACTIVITY_H_INCLUDED
#define GAMEACTIVITY_H_INCLUDED

#include "SwooshFiles.h"
#include "../scene/GameIntro.h"
#include "../scene/GameMenu.h"
#include "../scene/GameLevel.h"
#include "../scene/GameOver.h"
#include "../scene/GameEndScreen.h"

using namespace swoosh::intent;

class GameActivity : public Activity
{
private:
    is::GameDisplay *m_gameScene;
    bool m_changeActivity;

public:
    GameActivity(ActivityController& controller, is::GameDisplay::DisplayOption &launchOption,
                        GameSystemExtended &gameSysExt) : Activity(&controller), m_changeActivity(false)
    {
        m_gameScene = 0;
        switch (launchOption)
        {
        case is::GameDisplay::DisplayOption::INTRO:
            m_gameScene = new GameIntro(controller.getWindow(), getView(), *(this->controller->getSurface()), launchOption, gameSysExt);
        break;
        case is::GameDisplay::DisplayOption::MAIN_MENU:
            m_gameScene = new GameMenu(controller.getWindow(), getView(), *(this->controller->getSurface()), launchOption, gameSysExt);
        break;
        case is::GameDisplay::DisplayOption::GAME_LEVEL:
            m_gameScene = new GameLevel(controller.getWindow(), getView(), *(this->controller->getSurface()), launchOption, gameSysExt);
        break;
        case is::GameDisplay::DisplayOption::GAME_OVER:
            m_gameScene = new GameOver(controller.getWindow(), getView(), *(this->controller->getSurface()), launchOption, gameSysExt);
        break;
        case is::GameDisplay::DisplayOption::GAME_END_SCREEN:
            m_gameScene = new GameEndScreen(controller.getWindow(), getView(), *(this->controller->getSurface()), launchOption, gameSysExt);
        break;
        default:
			is::showLog("Error : Scene not found !");
			std::terminate();
		break;
        }
        if (!m_gameScene->loadResources())
		{
			is::showLog("Error in loadResources function !");
			std::terminate();
		}
        this->setBGColor(m_gameScene->getBgColor());
    }

    virtual void onStart()
    {
        std::cout << "GameActivity OnStart called" << std::endl;
    }

    virtual void onUpdate(double elapsed)
    {
        if (m_gameScene->isRunning()) m_gameScene->step();
        else
        {
            if (!m_changeActivity)
            {
                switch (m_gameScene->m_launchOption)
                {
                    case is::GameDisplay::RESTART_LEVEL : // restart level (when player loses)
                        // restart level if life > 1
                        if (m_gameScene->getGameSystem().m_currentLives > 1)
                        {
                            m_gameScene->getGameSystem().m_currentLives--;
                            m_gameScene->getGameSystem().m_currentScore = 0;
                            m_gameScene->m_launchOption = is::GameDisplay::DisplayOption::GAME_LEVEL;
                            m_gameScene->getGameSystem().saveData(is::DATA_FILE);
                            getController().push<segue<RadialCCW, sec<1>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                                  m_gameScene->getGameSystem());
                        }
                        else // else GAME OVER
                        {
                            // reinitialize all variables excepting the current level and the game progress
                            m_gameScene->getGameSystem().initData(false);
                            m_gameScene->getGameSystem().m_currentBonus = 0;
                            m_gameScene->getGameSystem().m_currentLives = 3;
                            m_gameScene->m_launchOption = is::GameDisplay::GAME_OVER;
                            getController().push<segue<VerticalSlice, sec<2>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                                     m_gameScene->getGameSystem());
                        }
                    break;

                    case is::GameDisplay::NEXT_LEVEL : // go to the next level
                       // reinitialize level global variable
                        m_gameScene->getGameSystem().m_levelTime = 0;
                        m_gameScene->getGameSystem().m_currentScore = 0;
                        m_gameScene->m_launchOption = is::GameDisplay::DisplayOption::GAME_LEVEL;

                        // go to game end screen when player finish all levels
                        if (m_gameScene->getGameSystem().m_currentLevel > m_gameScene->getGameSystem().m_levelNumber)
                        {
                            m_gameScene->m_launchOption = is::GameDisplay::GAME_END_SCREEN;
                            getController().push<segue<VerticalSlice, sec<2>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                                 m_gameScene->getGameSystem());
                        }
                        else getController().push<segue<RadialCCW, sec<1>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                                   m_gameScene->getGameSystem());
                    break;

                    case is::GameDisplay::DisplayOption::GAME_OPTION_RESTART: // restart level (when player uses restart option)
                        m_gameScene->m_launchOption = is::GameDisplay::DisplayOption::GAME_LEVEL;

                        // reinitialize all variables excepting the current level and the game progress
                        m_gameScene->getGameSystem().initData(false);
                        getController().push<segue<RadialCCW, sec<1>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                              m_gameScene->getGameSystem());
                    break;

                    case is::GameDisplay::DisplayOption::MAIN_MENU:
                        getController().push<segue<VerticalSlice, sec<2>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                                 m_gameScene->getGameSystem());
                    break;

                    case is::GameDisplay::DisplayOption::GAME_LEVEL:
                        getController().push<segue<VerticalSlice, sec<2>>::to<GameActivity>>(m_gameScene->m_launchOption,
                                                                                              m_gameScene->getGameSystem());
                    break;
                }
                m_changeActivity = true;
            }
        }
    }

    virtual void onLeave()
    {
        std::cout << "GameActivity OnLeave called" << std::endl;
    }

    virtual void onExit()
    {
        delete m_gameScene;
        m_gameScene = 0;
        std::cout << "GameActivity OnExit called" << std::endl;
    }

    virtual void onEnter()
    {
        std::cout << "GameActivity OnEnter called" << std::endl;
    }

    virtual void onResume()
    {
        std::cout << "GameActivity OnResume called" << std::endl;
    }

    virtual void onDraw(sf::RenderTexture& surface)
    {
        sf::RenderWindow& window = getController().getWindow();
        m_gameScene->drawScreen();
    }

    virtual void onEnd() {}

    ~GameActivity() {}
};

#endif // GAMEACTIVITY_H_INCLUDED
