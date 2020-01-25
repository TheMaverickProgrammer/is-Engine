#include "GameLevel.h"

bool GameLevel::loadResources()
{
    if (!GameDisplay::loadParentResources()) return false;

    ////////////////////////////////////////////////////
    #if !defined(__ANDROID__)
    is::createText(m_fontSystem, m_txtDebugInfo, "", 0.f, 0.f, 20);
    #endif
    ////////////////////////////////////////////////////

    is::createText(m_fontSystem, m_txtContinue, lang::pad_continue_game[m_gameSysExt.m_gameLanguage], 0.f, 0.f, true);
    is::createText(m_fontSystem, m_txtQuit, lang::pad_quit_game[m_gameSysExt.m_gameLanguage], 0.f, 0.f, true);
    is::createText(m_fontSystem, m_txtRestart, lang::pad_restart_game[m_gameSysExt.m_gameLanguage], 0.f, 0.f, true);

    if (!m_fontLevel.loadFromFile(is::FONT_DIR + "space_ranger_halftone_italic_qz_30.otf")) return false;

    if (!m_texToolsPad.loadFromFile(is::GUI_DIR + "tools_pad.png"))      return false;
    if (!m_texJoystick.loadFromFile(is::GUI_DIR + "game_pad.png"))       return false;
    if (!m_texIcoMenuBtn.loadFromFile(is::GUI_DIR + "ico_button.png"))   return false;
    m_cancelBt.loadResources(m_texToolsPad);
    m_gameKey.loadResources(m_texJoystick);

    setView();

    // load buffers
    if (!sdEnemyDestroy.loadFromFile(is::SFX_DIR + "enemy_destroy.wav"))            return false;
    if (!m_sbHaveHealth.loadFromFile(is::SFX_DIR + "have_health.ogg"))              return false;
    if (!m_sbHurt.loadFromFile(is::SFX_DIR + "hurt.ogg"))                           return false;
    if (!m_sbAttack.loadFromFile(is::SFX_DIR + "attack.wav"))                       return false;
    if (!m_sbLose.loadFromFile(is::SFX_DIR + "lose.ogg"))                           return false;
    if (!m_sbHaveBonus.loadFromFile(is::SFX_DIR + "have_bonus.ogg"))                return false;
    if (!m_sbHaveLife.loadFromFile(is::SFX_DIR + "have_life.wav"))                  return false;
    if (!m_sbJump.loadFromFile(is::SFX_DIR + "jump.wav"))                           return false;
    if (!m_sbPlayerWin.loadFromFile(is::SFX_DIR + "finish.ogg"))                    return false;

    // sound
    m_sndEnemyDestroy.setBuffer(sdEnemyDestroy);
    m_sndHaveHealth.setBuffer(m_sbHaveHealth);
    m_sndAttack.setBuffer(m_sbAttack);
    m_sndHurt.setBuffer(m_sbHurt);
    m_sndLose.setBuffer(m_sbLose);
    m_sndHaveBonus.setBuffer(m_sbHaveBonus);
    m_sndHaveLife.setBuffer(m_sbHaveLife);
    m_sndJump.setBuffer(m_sbJump);
    m_sndPlayerWin.setBuffer(m_sbPlayerWin);

    // GUI resources
    if (!m_texPad.loadFromFile(is::GUI_DIR + "option_pad.png"))               return false;
    if (!m_texDialog.loadFromFile(is::GUI_DIR + "dialog_box.png"))            return false;

    is::createSprite(m_texPad, m_sprPad1, sf::IntRect(0, 0, 160, 32), sf::Vector2f(0.f, 0.f), sf::Vector2f(80.f, 16.f));
    is::createSprite(m_texPad, m_sprPad2, sf::IntRect(0, 0, 160, 32), sf::Vector2f(0.f, 0.f), sf::Vector2f(80.f, 16.f));
    is::createSprite(m_texPad, m_sprPad3, sf::IntRect(0, 0, 160, 32), sf::Vector2f(0.f, 0.f) , sf::Vector2f(80.f, 16.f));
    is::createSprite(m_texPad, m_sprButtonSelect, sf::IntRect(160, 0, 160, 32), sf::Vector2f(0.f, 0.f) , sf::Vector2f(80.f, 16.f));

    // tiles
    if (!m_texTile.loadFromFile(is::TILES_DIR + "tileset.png")) return false;

    // sprites
    if (!m_texPlayer.loadFromFile(is::SPRITES_DIR + "player.png"))                    return false;
    if (!m_texBonus.loadFromFile(is::SPRITES_DIR + "bonus_icon.png"))                 return false;
    if (!m_texEnemy.loadFromFile(is::SPRITES_DIR + "enemy.png"))                      return false;
    if (!m_texFinishObject.loadFromFile(is::SPRITES_DIR + "finish_flag.png"))         return false;

    // background
    if (!m_texLevelBg.loadFromFile(is::TILES_DIR + "level_bg.png")) return false;

    // chose the corresponding map array
    short const *mapIndex;
    switch (CURRENT_LEVEL)
    {
        case LEVEL_1  : mapIndex = LEVEL_1_MAP; break;
        case LEVEL_2  : mapIndex = LEVEL_2_MAP; break;
        case LEVEL_3  : mapIndex = LEVEL_3_MAP; break;
    }

    // allow to browse the map array
    int dataIndex(0);

    // variable to position the objects in level
    float i(0.f), j(0.f);

    // allow to build tile map
    std::vector<short> backTileValue;

    // this variables allow to store tile id
    // signification of 999 = empty tile
    short backTileNumber(999);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      TILE MAP INTERPRETER
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // read values in array (1003 = is end of tile map)
    do
    {
        if (mapIndex[dataIndex] >= 400 && mapIndex[dataIndex] < 700)
        {
            backTileNumber = (mapIndex[dataIndex] - 400);
        }

        else if (mapIndex[dataIndex] == 0) // bloc mask
            m_blocList.push_back(new Bloc(Bloc::BlocType::BLOC_NORMAL, 32.f * j, 32.f * i, 0));

        else if (mapIndex[dataIndex] == 1) // bloc transparent (bloc traversable)
            m_blocList.push_back(new Bloc(Bloc::BlocType::BLOC_TRANSPARENT, 32.f * j, 32.f * i, 0));

        else if (mapIndex[dataIndex] == 2) // bloc horizontal moving
        {
            m_blocList.push_back(new Bloc(Bloc::BlocType::BLOC_MOVE_HORIZ, 32.f * j, 32.f * i, &m_texTile));
            m_blocList[m_blocList.size() - 1]->setMoveHorizontal(true);
        }

        else if (mapIndex[dataIndex] == 3) // bloc vertical moving
        {
            m_blocList.push_back(new Bloc(Bloc::BlocType::BLOC_MOVE_VERTI, 32.f * j, 32.f * i, &m_texTile));
            m_blocList[m_blocList.size() - 1]->setMoveVertical(true);
        }

        else if (mapIndex[dataIndex] == 4) // limiter
            m_limiterList.push_back(new Limiter(32.f * j, 32.f * i));

        else if (mapIndex[dataIndex] == 5)
            m_enemyList.push_back(new Enemy(m_texEnemy, Enemy::EnemyType::ENEMY_SPIKE_BALL, 32.f * j, 32.f * i));

        else if (mapIndex[dataIndex] == 6)
            m_enemyList.push_back(new Enemy(m_texEnemy, Enemy::EnemyType::ENEMY_BALL, 32.f * j, 32.f * i, 1.f));

        else if (mapIndex[dataIndex] == 7) // Bonus
            m_bonusList.push_back(new Bonus(m_texBonus, Bonus::BonusType::BONUS_NORMAL, 32.f * j, 32.f * i));

        else if (mapIndex[dataIndex] == 8) // Health
            m_bonusList.push_back(new Bonus(m_texBonus, Bonus::BonusType::BONUS_HEALTH, 32.f * j, 32.f * i));

        else if (mapIndex[dataIndex] == 9) // Life
            m_bonusList.push_back(new Bonus(m_texBonus, Bonus::BonusType::BONUS_1_UP, 32.f * j, 32.f * i));

        else if (mapIndex[dataIndex] == 10) // player
        {
            // load player resources
            m_player.loadResources(m_texPlayer, m_sbJump);
            m_player.setX(32.f * j);
            m_player.setY((32.f * i) - 16.f);
            m_player.setStartPosition(m_player.getX(), m_player.getY());
        }

        else if (mapIndex[dataIndex] == 11) // finish level
        {
            m_finishObject.setX(32.f * j);
            m_finishObject.setY(32.f * i);
            m_finishObject.loadResources(m_texFinishObject);
            m_finishObject.setStartPosition(m_finishObject.getX(), m_finishObject.getY());
        }

        else if (mapIndex[dataIndex] == 1000) // empty tile (empty space)
        {
            // empty tile
            backTileNumber = 999;
        }

        else if (mapIndex[dataIndex] == 1001) // move the cursor
        {
            // filling the tile number array
            backTileValue.push_back(backTileNumber);

            // empty tile
            backTileNumber = 999;
            j++;
        }

        else if (mapIndex[dataIndex] == 1002) // back to the line of cursor
        {
            if (m_levelWidth == 0) m_levelWidth = static_cast<unsigned int>(j); // define the level width
            j = 0.f;
            i++;
            backTileNumber = 999; // empty tile
            m_levelHeight = static_cast<unsigned int>(i); // define the level height
        }

        else // error
        {
            is::showLog("\nUnknown value  : " + is::numToStr(mapIndex[dataIndex]) + "\n");
        }

        // allow to now the line of cursor
        dataIndex++;
    }
    while (mapIndex[dataIndex] != 1003);

    m_gameTime.setValue(2, 59, 59);

    // sort object array
    is::sortObjArray(m_bonusList);
    is::sortObjArray(m_enemyList);
    is::sortObjArray(m_blocList);

    // level information
    is::showLog("\nLevel             : " + is::numToStr(CURRENT_LEVEL + 1) +
                "\nInstance Number   : " + is::numToStr(is::MainObject::instanceNumber) +
                "\nLevel Width       : " + is::numToStr(m_levelWidth) +
                "\nLevel Height      : " + is::numToStr(m_levelHeight) +
                "\nLevel Time        : " + is::numToStr(m_gameTime.getMinute()) + "min : " + is::numToStr(m_gameTime.getSecond()) + "s" +
                "\n");


//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      BUILD THE BACKGROUND OF LEVEL
//////////////////////////////////////////////////////////////////////////////////////////////////////
    is::createSprite(m_texLevelBg, m_sprLevelBg, sf::IntRect(0, 0, (m_levelWidth * 32), 640), sf::Vector2f(m_viewX * 0.25f - 80.f, m_viewY - 240.f),
                     sf::Vector2f(0.f, 0.f), true);

    // create the tile map with vertex array
    unsigned int valX(0), valY(0);
    int const bgSize(35); // size of vertex array
    bool stop(false);

    while (!stop)
    {
        m_levelBackTile.push_back(new LevelTile(m_texTile));
        m_levelBackTile[m_levelBackTile.size() - 1]->load(sf::Vector2u(32, 32), backTileValue, m_levelWidth, m_levelHeight, valX, valY, bgSize, stop);
    }
    WITH (m_levelBackTile.size())
    {
        if (!m_levelBackTile[_I]->hasTile())
        {
            delete m_levelBackTile[_I];
            m_levelBackTile[_I] = 0;
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      CREATION OF THE LEVEL
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // set view position in relation to player
    m_viewX = m_player.getX();
    m_viewY = m_player.getY();

    if (m_viewX < m_viewW / 2) m_viewX = m_viewW / 2;
    if (m_viewX > static_cast<float>(m_levelWidth) * 32.f - m_viewW / 2)
        m_viewX = static_cast<float>(m_levelWidth) * 32.f - m_viewW / 2;

    if (m_viewY < m_viewH / 2) m_viewY = m_viewH / 2;
    if (m_viewY > static_cast<float>(m_levelHeight) * 32.f - m_viewH / 2)
        m_viewY = static_cast<float>(m_levelHeight) * 32.f - m_viewH / 2;

    m_pauseOption = new PauseOption(m_texIcoMenuBtn);

    // set pause screen object position
    is::setSFMLObjX_Y(m_sprPad1, m_viewX, m_viewY - 32.f);
    is::setSFMLObjX_Y(m_sprPad2, m_viewX, m_viewY + 32.f);
    is::setSFMLObjX_Y(m_sprPad3, m_viewX - (70.f - m_pauseObjMove), m_viewY);
    is::setSFMLObjX_Y(m_sprButtonSelect, is::getSFMLObjX(m_sprPad1), is::getSFMLObjY(m_sprPad1));
    is::setSFMLObjX_Y(m_txtContinue, is::getSFMLObjX(m_sprPad1), is::getSFMLObjY(m_sprPad1));
    is::setSFMLObjX_Y(m_txtQuit, is::getSFMLObjX(m_sprPad3), is::getSFMLObjY(m_sprPad3));
    is::setSFMLObjX_Y(m_recPauseBG, m_viewX, m_viewY);

    // load HUD resources
    m_gameHud.setPosition(m_viewX, m_viewY);
    m_gameHud.loadResources(m_fontSystem);

    // load Dialog Box resources
    m_gameDialog.loadResources(m_texDialog, m_fontSystem, m_gameSysExt);

    // set view param
    setView();

    // load level music
    m_mscLevel.openFromFile(is::MUSIC_DIR + "world_1_music.ogg");
    m_mscLevel.setLoop(true);
    m_mscLevel.play();

    // don't play music if this option is off
    if (!m_gameSysExt.m_enableMusic) m_mscLevel.pause();

    // initialize background
    backgroundUpdate(is::MAX_CLOCK_TIME);

    return true;
}
