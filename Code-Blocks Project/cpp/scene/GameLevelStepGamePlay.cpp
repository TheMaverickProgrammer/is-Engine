#include "GameLevel.h"

void GameLevel::gamePlay(const float &DELTA_TIME)
{
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      VIEW
//////////////////////////////////////////////////////////////////////////////////////////////////////
    updateObjView(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CONTROLLER
//////////////////////////////////////////////////////////////////////////////////////////////////////
    joystickController(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SPECIAL STEP
//////////////////////////////////////////////////////////////////////////////////////////////////////
    levelSpecialAction(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      LEVEL BLOC
//////////////////////////////////////////////////////////////////////////////////////////////////////

    short const yPrecision(6);
    short const xPrecision(10);
    bool rightCollision(true);
    bool cancelRightCollision(false);
    bool canFollowBloc(true);

    if (m_timeCancelCollision > 0.f) m_timeCancelCollision -= is::getMSecond(DELTA_TIME);
    else m_timeCancelCollision = 0.f;

    // this variables allow to store bloc
    Bloc *blocGroundId(0);
    Bloc *blocId(0);

    // bloc loop
    WITH(m_blocList.size())
    {
        if (is::instanceExist(m_blocList[_I]))
        {
            if (!isInPlayerRay(m_blocList[_I])) continue;

            if (m_blocList[_I]->getIsActive())
            {
                // moving blocs
                if (m_blocList[_I]->getType() == Bloc::BlocType::BLOC_MOVE_HORIZ ||
                    m_blocList[_I]->getType() == Bloc::BlocType::BLOC_MOVE_VERTI)
                {
                    for (auto &limiter : m_limiterList)
                    {
                        if (m_blocList[_I]->placeMetting(0 , 0, limiter))
                        {
                            if (!m_blocList[_I]->getChangeDir())
                            {
                                m_blocList[_I]->setSpeed(1.f);
                                m_blocList[_I]->setChangeDir(true);
                                break;
                            }
                        }
                    }
                }

                // check collision at bottom
                if (static_cast<int>(m_timeCancelCollision) == 0)
                {
                    if (m_player.getY() + static_cast<float>(m_player.getMaskHeight()) < m_blocList[_I]->getY() + (static_cast<float>(yPrecision) + 4.f) &&
                        m_player.getY() + static_cast<float>(m_player.getMaskHeight()) > m_blocList[_I]->getY() - 2.f)
                    {
                        if (m_player.placeMetting(0, yPrecision + 10, m_blocList[_I]) && m_player.getVspAcc() > -0.99f)
                        {
                            blocGroundId = m_blocList[_I]; // store the bloc to use it after
                        }
                    }
                }

                // execute this instruction only for this bloc
                if (m_blocList[_I]->getType() == Bloc::BlocType::BLOC_NORMAL)
                {
                    // check collision at right
                    if (m_player.placeMetting(xPrecision, 0, m_blocList[_I]))
                    {
                        if (m_player.getHsp() > 0.f)
                        {
                            blocId = m_blocList[_I];
                            rightCollision = true;
                        }
                        if (is::instanceExist(blocGroundId))
                        {
                            if (blocGroundId->getMoveHorizontal() && std::abs(blocGroundId->getSpeed()) > 0.f) canFollowBloc = false;
                        }
                    }
                    else if (m_player.placeMetting(-xPrecision, 0, m_blocList[_I])) // check collision at left
                    {
                        if (m_player.getHsp() < 0.f)
                        {
                            blocId = m_blocList[_I];
                            rightCollision = false;
                        }
                        if (is::instanceExist(blocGroundId))
                        {
                            if (blocGroundId->getMoveHorizontal() && std::abs(blocGroundId->getSpeed()) > 0.f) canFollowBloc = false;
                        }
                    }
                    else if (m_player.placeMetting(0, -yPrecision, m_blocList[_I]) && m_player.getVspAcc() < 0.f) // check top collision
                    {
                        m_player.setY(m_blocList[_I]->getY() + 5.f + static_cast<float>(m_blocList[_I]->getMaskHeight()));
                        m_player.setIsJumping(false);
                    }
                }

                // update blocks
                m_blocList[_I]->step(DELTA_TIME, m_blocList[_I]->inViewRec(*this, true));
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // reduce player invincibility time
    if (m_playerInvicibilityTime > 0) m_playerInvicibilityTime -= static_cast<int>(is::VALUE_CONVERSION * DELTA_TIME);
    else m_playerInvicibilityTime = 0;
    ///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      ENEMY
//////////////////////////////////////////////////////////////////////////////////////////////////////
    updateObjEnemyList(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      FINISH FLAG
//////////////////////////////////////////////////////////////////////////////////////////////////////
    updateObjFinish(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      LEVEL CHRONOMETER
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_countLevelTime)
    {
        if (m_gameTime.getTimeValue() != 0)
        {
            // level global time
            m_gameSysExt.m_levelTime += is::getMSecond(DELTA_TIME);
            m_gameTime.step(DELTA_TIME, is::VALUE_CONVERSION, is::VALUE_TIME);
        }
        else m_timeUp = true;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      PLAYER IS ACTIVE
//////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_player.getIsActive())
    {
        // check if it has collision
        if (is::instanceExist(blocGroundId) && static_cast<int>(m_timeCancelCollision) == 0) m_player.setOnGround(true);
        else m_player.setOnGround(false);

        // cancel horizontal collision if this condition is true
        if (cancelRightCollision) blocId = 0;

        // is on ground
        if (m_player.getOnGround())
        {
            // if player touch the ground after jumping
            if (m_player.getVspAcc() > 0.f)
            {
                m_player.setVspAcc(0.f);
            }

            // if player is on horizontal moving bloc
            if (canFollowBloc)
            {
                if (blocGroundId->getMoveHorizontal())
                {
                    m_player.setX(m_player.getX() + (blocGroundId->getSpeed() * is::VALUE_CONVERSION) * DELTA_TIME);
                }
                if (blocGroundId->getMoveVertical())
                {
                    m_player.setY(m_player.getY() + (blocGroundId->getSpeed() * is::VALUE_CONVERSION) * DELTA_TIME);
                }
            }

            // if player is on vertical moving bloc
            if (is::instanceExist(blocGroundId))
            {
                m_player.setY(blocGroundId->getY() - static_cast<float>(m_player.getMaskHeight()));
            }
        }

        // horizontal collision with blocs
        if (blocId != 0)
        {
            if (!rightCollision) // put player to the right of the block
            {
                m_player.setX(blocId->getX() + static_cast<float>(blocId->getMaskWidth()));
                m_player.setHsp(0.f);
            }
            else  // put player to the left of the block
            {
                m_player.setX(blocId->getX() - static_cast<float>(m_player.getMaskWidth()));
                m_player.setHsp(0.f);
            }
        }

        // if the player outs at the bottom of the level then he loses
        if (m_player.getY() > (static_cast<float>(m_levelHeight * 32) + 8.f))
        {
            playerHurt(false);
        }

        // player will lose if the chronometer reaches 00min : 00s : 00
        if (m_timeUp) playerHurt(false);

        // avoid the player to out at the left of the level
        if (m_player.getX() < 5.f && m_player.getHsp() < 0.f)
        {
            m_player.setX(m_player.getXPrevious());
            m_player.setHsp(0.f);
        }

        // avoid the player to out at the right of the level
        if (m_player.getX() > static_cast<float>(m_levelWidth * 32) - 32.f)
        {
            m_player.setHsp(0.f);
            m_player.setX(static_cast<float>(m_levelWidth * 32) - 32.f);
        }

        // avoid the player to out at the top of the level
        if (m_player.getY() < -64.f)
        {
            m_player.setVspAcc(0.f);
            m_player.setY(0.f);
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      IF PLAYER IS NOT ACTIVE
//////////////////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        if (m_playerHealth == 0 && m_player.getY() < (static_cast<float>(m_levelHeight * 32) + 24.f))
        {
            m_player.moveDown((2.153f * is::VALUE_CONVERSION) * DELTA_TIME);
        }
        if (m_restartTime > 0) m_restartTime += static_cast<int>(is::VALUE_CONVERSION * DELTA_TIME);
        if (m_restartTime > 30) m_player.setIsKO(true);
    }

    bool createElectroShock(false);

    // update player
    m_player.step(m_gameSysExt ,DELTA_TIME, m_gameKey, m_playerHealth, (blocId == 0), createElectroShock);

    if (createElectroShock && !is::instanceExist(m_electroShock))
    {
        m_gameSysExt.playSound(m_sndAttack);
        m_electroShock = new ElectroShock(m_player.getSpriteX(), m_player.getSpriteY());

        // orientation
        float const _SPEED(4.f);
        if (m_gameKey.m_keyRightPressed)     m_electroShock->setHsp(_SPEED);
        else if (m_gameKey.m_keyLeftPressed) m_electroShock->setHsp(-_SPEED);
        else if (m_gameKey.m_keyDownPressed) m_electroShock->setVsp(_SPEED);
        else if (m_gameKey.m_keyUpPressed)   m_electroShock->setVsp(-_SPEED);
    }

    // update electro shock object
    if (is::instanceExist(m_electroShock))
    {
        m_electroShock->step(DELTA_TIME);

        // destruction
        if (m_electroShock->isDestroyed())
        {
            delete m_electroShock;
            m_electroShock = 0;
        }
    }


//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      GAME PLAY CONTROLLER
//////////////////////////////////////////////////////////////////////////////////////////////////////
    updateObjGameplayController(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      BACKGROUND
//////////////////////////////////////////////////////////////////////////////////////////////////////
    backgroundUpdate(DELTA_TIME);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      BONUS
//////////////////////////////////////////////////////////////////////////////////////////////////////
    updateObjBonusList(DELTA_TIME);
}
