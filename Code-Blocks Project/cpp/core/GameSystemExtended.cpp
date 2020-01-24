#include "GameSystemExtended.h"

GameSystemExtended::GameSystemExtended() :
    GameSystem(),
    LEVEL_NUMBER(3)
{
    m_defaultPadDirXPos = -250.f;
    m_defaultPadDirYPos = 170.f;
    m_defaultPadAtionXPos = 245.f;
    m_defaultPadActionYPos = 200.f;
    m_padDirXPos = m_defaultPadDirXPos;
    m_padDirYPos = m_defaultPadDirYPos;
    m_padAtionXPos = m_defaultPadAtionXPos;
    m_padActionYPos = m_defaultPadActionYPos;
    m_permutePadAB = false;
    m_padAlpha = 255;
}

void GameSystemExtended::initProgress()
{
    m_gameProgression = 0; // LEVEL 1
    m_currentLives = 3;
    m_currentBonus = 0;
    m_currentLevel = 0;
    m_currentScore = 0;
}

void GameSystemExtended::initSystemData()
{
    // global variable
    m_gameLanguage = 0;
    m_firstLaunch = true;
    m_levelNumber = LEVEL_3; // number of level
}

void GameSystemExtended::initData(bool clearCurrentLevel)
{
    if (clearCurrentLevel) m_currentLevel = 0; // LEVEL 1
    m_currentBonus = 0;
    m_currentScore = 0;
}

void GameSystemExtended::saveData(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "wb");

    if (file != NULL)
    {
        fwrite(&m_gameProgression, sizeof(int), 1, file);
        fwrite(&m_currentLives, sizeof(int), 1, file);
        fwrite(&m_currentBonus, sizeof(int), 1, file);
        fclose(file);
    }
}

void GameSystemExtended::loadData(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "rb");

    if (file != NULL)
    {
        fread(&m_gameProgression, sizeof(int), 1, file);
        fread(&m_currentLives, sizeof(int), 1, file);
        fread(&m_currentBonus, sizeof(int), 1, file);
        fclose(file);
    }
}

void GameSystemExtended::saveConfig(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "wb");

    if (file != NULL)
    {
        fwrite(&m_enableSound, sizeof(bool), 1, file);
        fwrite(&m_enableMusic, sizeof(bool), 1, file);
        fwrite(&m_enableVibrate, sizeof(bool), 1, file);
        fwrite(&m_gameLanguage, sizeof(int), 1, file);
        fwrite(&m_firstLaunch, sizeof(bool), 1, file);
        fclose(file);
    }
}

void GameSystemExtended::loadConfig(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "rb");

    if (file != NULL)
    {
        fread(&m_enableSound, sizeof(bool), 1, file);
        fread(&m_enableMusic, sizeof(bool), 1, file);
        fread(&m_enableVibrate, sizeof(bool), 1, file);
        fread(&m_gameLanguage, sizeof(int), 1, file);
        fread(&m_firstLaunch, sizeof(bool), 1, file);
        fclose(file);
    }
}

void GameSystemExtended::savePadConfig(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "wb");

    if (file != NULL)
    {
        fwrite(&m_padDirXPos, sizeof(float), 1, file);
        fwrite(&m_padDirYPos, sizeof(float), 1, file);
        fwrite(&m_padAtionXPos, sizeof(float), 1, file);
        fwrite(&m_padActionYPos, sizeof(float), 1, file);
        fwrite(&m_padAlpha, sizeof(int), 1, file);
        fwrite(&m_permutePadAB, sizeof(bool), 1, file);
        fclose(file);
    }
}

void GameSystemExtended::loadPadConfig(std::string const &fileName)
{
    FILE *file = NULL;
    file = fopen(fileName.c_str(), "rb");

    if (file != NULL)
    {
        fread(&m_padDirXPos, sizeof(float), 1, file);
        fread(&m_padDirYPos, sizeof(float), 1, file);
        fread(&m_padAtionXPos, sizeof(float), 1, file);
        fread(&m_padActionYPos, sizeof(float), 1, file);
        fread(&m_padAlpha, sizeof(int), 1, file);
        fread(&m_permutePadAB, sizeof(bool), 1, file);
        fclose(file);
    }
}
