#include "GameConfig.h"

namespace is
{
namespace GameVersion
{
std::wstring getGameVersion()
{
    return MAJOR + L"." + MINOR;
}
}
}
