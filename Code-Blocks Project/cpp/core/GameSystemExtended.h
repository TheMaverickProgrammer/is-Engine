#ifndef GAMESYSTEMEXTENDED_H_INCLUDED
#define GAMESYSTEMEXTENDED_H_INCLUDED

#include "../system/function/GameFunction.h"
#include "../config/GameConfig.h"

enum LevelId
{
    LEVEL_1,
    LEVEL_2,
    LEVEL_3
};

class GameSystemExtended : public is::GameSystem
{
public:
    GameSystemExtended();

    void initSystemData();
    void initProgress();
    void initData(bool clearCurrentLevel = true);
    void saveData(std::string const &fileName);
    void loadData(std::string const &fileName);
    void saveConfig(std::string const &fileName);
    void loadConfig(std::string const &fileName);
    void savePadConfig(std::string const &fileName);
    void loadPadConfig(std::string const &fileName);

    int   m_gameProgression;
    int   m_gameLanguage;
    int   m_currentLevel;
    int   m_currentLives;
    int   m_currentBonus;
    int   m_currentScore;
    int   m_levelNumber;
    int   m_levelTime;
    bool  m_permutePadAB;
    int const LEVEL_NUMBER;
    float m_padDirXPos, m_padDirYPos, m_padAtionXPos, m_padActionYPos;
    float m_defaultPadDirXPos, m_defaultPadDirYPos, m_defaultPadAtionXPos, m_defaultPadActionYPos;
    int m_padAlpha;
    bool  m_firstLaunch;
};

#endif // GAMESYSTEMEXTENDED_H_INCLUDED
