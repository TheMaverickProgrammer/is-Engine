#ifndef GAMEHELP_H_INCLUDED
#define GAMEHELP_H_INCLUDED

#include <cstring>
#include "../../system/entity/MainObject.h"
#include "../../language/GameLanguage.h"

class GameDialog : public is::MainObject
{
public:
    enum DialogIndex
    {
        DIALOG_NONE = -1,
        DIALOG_PLAYER_MOVE,
        DIALOG_PLAYER_ATTACK,
    };

    GameDialog();
    void loadResources(sf::Texture &tex, sf::Font &fnt, GameSystemExtended &gameSysExt);
    void step(const float &DELTA_TIME, is::GameDisplay *scene, sf::Sound &sndSwitch, sf::Sound &sndCancel);
    void setDialog(DialogIndex dialogIndex);
    void setMouseInCollison(bool val);
    void draw(sf::RenderTexture &surface);

    DialogIndex getDialogIndex() const
    {
        return m_dialogIndex;
    }

    bool getMouseInCollison() const
    {
        return m_mouseInCollison;
    }

    bool showDialog() const
    {
        return m_showDialog;
    }

private:
    sf::Text m_txtDialog, m_txtSkip;
    std::wstring m_strDialog;
    sf::Sprite m_sprNext, m_sprSkip;

    bool m_showDialog, m_mouseInCollison, m_dialogEnd, m_newLine;
    int m_msgIndex, m_msgIndexMax, m_size;
    float m_blindTime;
    DialogIndex m_dialogIndex;
};

#endif // GAMEHELP_H_INCLUDED
