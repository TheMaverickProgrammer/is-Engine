#include "GameLevel.h"

void GameLevel::levelSpecialAction(float const &DELTA_TIME)
{
    if (CURRENT_LEVEL == LevelId::LEVEL_1)
    {
        if (m_gameDialog.getDialogIndex() == GameDialog::DIALOG_NONE)
        {
            m_gameDialog.setDialog(GameDialog::DIALOG_PLAYER_MOVE);
        }
    }

    if (CURRENT_LEVEL == LevelId::LEVEL_3)
    {
        if (m_gameDialog.getDialogIndex() == GameDialog::DIALOG_NONE)
        {
            m_gameDialog.setDialog(GameDialog::DIALOG_PLAYER_ATTACK);
        }
    }
}
