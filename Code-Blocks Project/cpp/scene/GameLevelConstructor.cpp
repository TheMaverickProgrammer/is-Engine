#include "GameLevel.h"

GameLevel::GameLevel(sf::RenderWindow &window, sf::View &view, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt):
    GameDisplay(window, view, surface, m_launchOption, gameSysExt, sf::Color::White),
    m_gameHud(m_playerHealth, *this),
    m_finishObject(m_gameSysExt),
    m_gameKey(this),
    CURRENT_LEVEL(gameSysExt.m_currentLevel),
    m_levelWidth(0),
    m_levelHeight(0),
    m_playerHealth(4),
    m_mscLevelVolume(100),
    m_playerInvicibilityTime(0),
    m_restartTime(0),
    m_validationKeyPressedTime(0),
    m_timeCancelCollision(0.f),
    m_isOnMovingBloc(false),
    m_isPlaying(true),
    m_levelStart(true),
    m_levelEnd(false),
    m_levelCompleted(false),
    m_timeUp(false),
    m_countLevelTime(true)
{
    // pause menu rectangle
    is::createRectangle(m_recPauseBG, sf::Vector2f(m_viewW + 10, m_viewH + 10.f), sf::Color(0, 0, 0, 100));

    // redefine view parameter
    m_viewX = (m_viewW / 2.f);
    m_viewY = (m_viewH / 2.f);

    srand(time(NULL));
    m_pauseOption = 0;
    m_electroShock = 0;
}


GameLevel::~GameLevel()
{
    // destroy object to release memory
    WITH(m_blocList.size())
    {
        is::instanceDestroy(m_blocList[_I]);
    }
    WITH(m_limiterList.size())
    {
        is::instanceDestroy(m_limiterList[_I]);
    }
    WITH(m_bonusList.size())
    {
        is::instanceDestroy(m_bonusList[_I]);
    }
    WITH(m_enemyList.size())
    {
        is::instanceDestroy(m_enemyList[_I]);
    }
    WITH(m_levelBackTile.size())
    {
        is::instanceDestroy(m_levelBackTile[_I]);
    }
    is::instanceDestroy(m_pauseOption);
    is::instanceDestroy(m_electroShock);
}
