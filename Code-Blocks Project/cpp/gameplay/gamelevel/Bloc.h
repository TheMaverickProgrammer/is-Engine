#ifndef BLOC_H_INCLUDED
#define BLOC_H_INCLUDED

#include "../../core/GameSystemExtended.h"
#include "../../system/entity/MainObject.h"

class Bloc : public is::MainObject
{
public:
    /// types of blocs
    enum BlocType
    {
        BLOC_NORMAL,
        BLOC_TRANSPARENT,
        BLOC_MOVE_HORIZ,
        BLOC_MOVE_VERTI
    };

    Bloc(BlocType type, float x, float y, sf::Texture *tex);
    ~Bloc();

    void step(float const& DELTA_TIME, bool isInView = false);
    void setMoveHorizontal(bool val);
    void setMoveVertical(bool val);
    void setChangeDir(bool val);
    void setCanNotMoveBloc();
    void draw(sf::RenderTexture &surface);

    bool getChangeDir() const;
    bool getMoveHorizontal() const;
    bool getMoveVertical() const;
    bool hasTexture() const;
    BlocType getType() const;

private:
    float m_sprOrigin;
    BlocType m_type;
    bool m_textureExiste, m_changeDir, m_moveHorizontal, m_moveVertical;
};

#endif // BLOC_H_INCLUDED
