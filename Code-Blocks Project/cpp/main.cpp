#include "core/GameEngine.h"

////////////////////////////////////////////////////////////
/// \brief application entry point
////////////////////////////////////////////////////////////
int main()
{
    GameEngine game;
    if (!game.play())
    {
        #if defined (__ANDROID__)
        std::terminate(); // close application
        #else
        return EXIT_FAILURE;
        #endif // defined
    }
}
