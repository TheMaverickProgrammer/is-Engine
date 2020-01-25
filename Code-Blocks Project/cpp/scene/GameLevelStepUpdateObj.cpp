#include "GameLevel.h"

void GameLevel::updateObjBonusList(float const &DELTA_TIME)
{
    WITH(m_bonusList.size())
    {
        if (is::instanceExist(m_bonusList[_I]))
        {
            if (!isInPlayerRay(m_bonusList[_I])) continue;

            if (m_bonusList[_I]->inViewRec(*this, true))
            {
                if (m_player.getIsActive())
                {
                     if (is::pointDistance(m_player.getSpriteX(), m_player.getSpriteY(),
                                       m_bonusList[_I]->getX() + 16.f, m_bonusList[_I]->getY()+ 16.f) < 34.f && !m_bonusList[_I]->getTouchPlayer())
                    {
                        switch (m_bonusList[_I]->getType())
                        {
                            case Bonus::BonusType::BONUS_NORMAL:
                                m_gameSysExt.m_currentBonus++;
                                m_gameSysExt.m_currentScore += m_bonusList[_I]->getScorePoint();

                                // add life if bonus > 99
                                if (m_gameSysExt.m_currentBonus > 99)
                                {
                                    m_gameSysExt.playSound(m_sndHaveLife);
                                    m_gameSysExt.useVibrate(60);
                                    m_gameSysExt.m_currentLives++;
                                    m_gameSysExt.m_currentBonus = 0;
                                }
                                m_gameSysExt.playSound(m_sndHaveBonus);
                            break;

                            case Bonus::BonusType::BONUS_HEALTH:
                                if (m_playerHealth< 4) m_playerHealth++;
                                else m_gameSysExt.m_currentScore += m_bonusList[_I]->getScorePoint() * 5;
                                m_gameSysExt.playSound(m_sndHaveHealth);
                            break;

                            case Bonus::BonusType::BONUS_1_UP:
                                m_gameSysExt.m_currentLives++;
                                m_gameSysExt.playSound(m_sndHaveLife);
                            break;
                        }
                        m_bonusList[_I]->setTouchPlayer(true);
                    }
                }

                m_bonusList[_I]->step(DELTA_TIME);

                // destruction
                if (m_bonusList[_I]->isDestroyed())
                {
                    delete m_bonusList[_I];
                    m_bonusList[_I] = 0;
                }
            }
        }
    }
}

void GameLevel::updateObjEnemyList(float const &DELTA_TIME)
{
    auto lowB = std::lower_bound(m_enemyList.begin(), m_enemyList.end(), m_player);
    auto upB  = std::upper_bound(m_enemyList.begin(), m_enemyList.end(), m_player);

    for (auto it = lowB; it != upB; ++it)
    {
        if (is::instanceExist((*it)))
        {
            // collision with player
            if (m_player.placeMetting(0, 0, (*it)))
            {
                if (playerIsVulnerable()) playerHurt();
            }

            // collision with player electro shock
            if (is::instanceExist(m_electroShock))
            {
                if (m_electroShock->distantToObject((*it), false) < 32.f && (*it)->getIsDestructible())
                {
                    m_gameSysExt.playSound(m_sndEnemyDestroy);
                    (*it)->setDestroyed();
                    m_electroShock->setDestroyed();
                }
            }

            (*it)->step(DELTA_TIME);

            // destruction
            if ((*it)->isDestroyed())
            {
                if ((*it)->getScorePoint() > 0) m_gameSysExt.m_currentScore += (*it)->getScorePoint();
                delete (*it);
                (*it) = 0;
            }
        }
    }
}

void GameLevel::updateObjView(float const &DELTA_TIME)
{
    // move the view if player is active
    if (!m_player.getIsKO())
    {
        if (m_player.getX() > m_viewW / 2.f && m_player.getX() < static_cast<float>(m_levelWidth * 32) - m_viewW / 2.f)
            m_viewX = m_player.getX();

        if (m_player.getY() > static_cast<float>(m_levelHeight * 32) - m_viewH / 2.f)
            m_viewY = static_cast<float>(m_levelHeight * 32) - m_viewH / 2.f;
        else if (m_player.getY() > m_viewH / 2.f && m_player.getY() < static_cast<float>(m_levelHeight * 32) - m_viewH / 2.f)
            m_viewY = m_player.getY();
        else if (m_player.getY() < m_viewH / 2.f) m_viewY = m_viewH / 2.f;
    }

    // update view
    setView();
}

void GameLevel::updateObjFinish(float const &DELTA_TIME)
{
    if (m_finishObject.getStep() == 0)
    {
        if (m_finishObject.placeMetting(0, 0, &m_player))
        {
            m_gameSysExt.playSound(m_sndPlayerWin);
            m_gameKey.m_disableAllKey = true;
            m_finishObject.addStep();
        }
    }
}

void GameLevel::updateObjGameplayController(float const &DELTA_TIME)
{
    m_gameHud.step(DELTA_TIME, m_gameTime, m_player, m_levelCompleted);

    pauseObjUpdate(DELTA_TIME, false);

    m_cancelBt.setPosition(m_viewX + 280.f, m_viewY - 200.f);
    m_cancelBt.step(DELTA_TIME);
}
