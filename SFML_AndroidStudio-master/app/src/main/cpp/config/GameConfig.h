#ifndef GAMEFILESPATH_H_INCLUDED
#define GAMEFILESPATH_H_INCLUDED

#include <string>

namespace is
{
namespace GameVersion
{
    static const std::wstring MAJOR = L"2";
    static const std::wstring MINOR = L"0";
    std::wstring getGameVersion();
}

static std::wstring const GAME_NAME(L"is::Engine DEMO");
static std::wstring const GAME_AUTHOR(L"DAOUDA NOUHOUN Ismael");

static std::string const GUI_DIR("data/image/gui/");
static std::string const FONT_DIR("data/font/");
static std::string const SPRITES_DIR("data/image/sprites/");
static std::string const TILES_DIR("data/image/tiles/");
static std::string const SFX_DIR = "data/sound/sfx/";
static std::string const MUSIC_DIR = "data/sound/music/";

#if defined(__ANDROID__)
// game package name
static std::string const PACKAGE_NAME("com.isdaouda.isenginedemo");

// files paths
static std::string const DATA_FILE(        "/data/data/" + PACKAGE_NAME + "/game_data.bin");
static std::string const CONFIG_FILE(      "/data/data/" + PACKAGE_NAME + "/game_config.dat");
static std::string const GAME_PAD_FILE(      "/data/data/" + PACKAGE_NAME + "/game_pad_config.dat");
#else
static std::string const DATA_FILE(        "save/game_data.bin");
static std::string const CONFIG_FILE(      "save/game_config.dat");
static std::string const GAME_PAD_FILE(    "save/game_pad_config.dat");
#endif // defined
}

#endif // GAMEFILESPATH_H_INCLUDED
