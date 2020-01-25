#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "../system/display/GameDisplay.h"
#include "../system/function/GameKeyData.h"

#include "../gameplay/gamelevel/Player.h"
#include "../gameplay/gamelevel/Bloc.h"
#include "../gameplay/gamelevel/HUD.h"
#include "../gameplay/gamelevel/Bonus.h"
#include "../gameplay/gamelevel/Enemy.h"
#include "../gameplay/gamelevel/ElectroShock.h"
#include "../gameplay/gamelevel/FinishObject.h"
#include "../gameplay/gamelevel/LevelTile.h"
#include "../gameplay/gamelevel/Limiter.h"
#include "../gameplay/widgets/CancelButton.h"
#include "../gameplay/widgets/PauseOption.h"
#include "../gameplay/widgets/GameDialog.h"

#include "../level/Level.h"
#include "../language/GameLanguage.h"

/// uncomment to use Box 2D
// #include "../ext_lib/Box2D/Box2D.h"

class GameLevel : public is::GameDisplay
{
public:
    GameLevel(sf::RenderWindow &window, sf::View &swooshView, sf::RenderTexture &surface, DisplayOption &m_launchOption, GameSystemExtended &gameSysExt);
    ~GameLevel();

    void step();
    void draw();

    bool loadResources();

private:
    void gamePlay(const float &DELTA_TIME);
    void levelSpecialAction(float const &DELTA_TIME);
    void stopSounds(bool val);
    float getPlayerOtherObjPtDir(is::MainObject const *obj);
    void playerHurt(bool canContinue = true);
    void backgroundUpdate(const float &DELTA_TIME);
    void joystickController(const float &DELTA_TIME);
    void pauseGame();
    void pauseObjUpdate(const float &DELTA_TIME, bool useMoveAnimation);
    bool clearObject(float const &x) const;
    bool playerIsVulnerable() const;
    bool isInPlayerRay(is::MainObject *obj) const;
    void stopLevelMusic(float const &DELTA_TIME);
    void updateObjBonusList(float const &DELTA_TIME);
    void updateObjEnemyList(float const &DELTA_TIME);
    void updateObjView(float const &DELTA_TIME);
    void updateObjFinish(float const &DELTA_TIME);
    void updateObjGameplayController(float const &DELTA_TIME);

    std::vector<Bloc*> m_blocList;
    std::vector<Limiter*> m_limiterList;
    std::vector<Bonus*> m_bonusList;
    std::vector<Enemy*> m_enemyList;
    std::vector<LevelTile*> m_levelBackTile;

    sf::Texture m_texPad, m_texToolsPad;
    sf::Texture m_texPlayer, m_texFinishObject, m_texEnemy, m_texBonus;
    sf::Texture m_texIcoMenuBtn;
    sf::Texture m_texDialog, m_texJoystick;

    sf::Texture m_texLevelBg, m_texTile;

    sf::Sprite m_sprLevelBg;
    sf::Sprite m_sprPad1, m_sprPad2, m_sprPad3;

    sf::RectangleShape m_recPauseBG;

    sf::Font m_fontLevel;

    sf::Text m_txtContinue, m_txtQuit, m_txtRestart;
    #if !defined(__ANDROID__)
    sf::Text m_txtDebugInfo;
    #endif

    sf::SoundBuffer sdEnemyDestroy, m_sbHaveHealth, m_sbHurt, m_sbAttack, m_sbLose;
    sf::SoundBuffer m_sbHaveBonus, m_sbHaveLife, m_sbJump;
    sf::SoundBuffer m_sbPlayerWin;

    sf::Sound m_sndEnemyDestroy, m_sndHaveHealth, m_sndHurt, m_sndAttack, m_sndLose;
    sf::Sound m_sndHaveBonus, m_sndHaveLife, m_sndJump;
    sf::Sound m_sndPlayerWin;

    sf::Music m_mscLevel;

    // permanents objects
    Player m_player;
    HUD m_gameHud;
    FinishObject m_finishObject;
    GameTime m_gameTime;
    CancelButton m_cancelBt;
    GameKeyData m_gameKey;
    GameDialog m_gameDialog;

    // temporal objects
    PauseOption *m_pauseOption;
    ElectroShock *m_electroShock;

    int const CURRENT_LEVEL;
    unsigned int m_levelWidth, m_levelHeight, m_playerHealth;
    int m_mscLevelVolume;
    int m_playerInvicibilityTime, m_restartTime, m_validationKeyPressedTime;
    float m_timeCancelCollision, m_pauseObjMove;
    bool m_isOnMovingBloc;
    bool m_isPlaying, m_levelStart, m_levelEnd, m_levelCompleted;
    bool m_timeUp, m_countLevelTime;
};

#endif // GAMELEVEL_H
