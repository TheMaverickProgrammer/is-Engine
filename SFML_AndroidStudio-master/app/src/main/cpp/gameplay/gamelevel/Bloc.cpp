#include "Bloc.h"

Bloc::Bloc(BlocType type, float x, float y, sf::Texture *tex):
    MainObject(x, y),
    m_sprOrigin(16.f),
    m_type(type),
    m_textureExiste((tex == 0) ? false : true),
    m_changeDir(false),
    m_moveHorizontal(false),
    m_moveVertical(false)
{
    m_isActive = true;
    m_time = 20.f;

    // define collision mask
    m_w = 32;
    m_h = 32;

    // load texture if it exists
    switch (type)
    {
        case Bloc::BlocType::BLOC_MOVE_HORIZ:
            is::createSprite(*tex, m_sprParent, sf::IntRect(0, 64, 64, 32), sf::Vector2f(m_x, m_y),
                             sf::Vector2f(0.f, 0.f), false, false);
            m_w = 64;
            m_speed = -1.f;
            m_sprOrigin = 0.f;
        break;

        case Bloc::BlocType::BLOC_MOVE_VERTI:
            is::createSprite(*tex, m_sprParent, sf::IntRect(64, 64, 64, 32), sf::Vector2f(m_x, m_y),
                             sf::Vector2f(0.f, 0.f), false, false);
            m_w = 64;
            m_speed = -1.f;
            m_sprOrigin = 0.f;
        break;
    }

    updateCollisionMask();
}

Bloc::~Bloc()
{
}

void Bloc::step(float const& DELTA_TIME, bool isInView)
{
    if (m_isActive)
    {
        if (m_type == Bloc::BlocType::BLOC_MOVE_HORIZ)
        {
            if (m_x > m_xStart && m_changeDir)
            {
                m_changeDir = false;
                m_speed = -m_speed;
            }
            m_x += ((m_speed * is::VALUE_CONVERSION) * DELTA_TIME);
        }

        if (m_type == Bloc::BlocType::BLOC_MOVE_VERTI)
        {
            if (m_y > m_yStart && m_changeDir)
            {
                m_changeDir = false;
                m_speed = -m_speed;
            }
            m_y += ((m_speed * is::VALUE_CONVERSION) * DELTA_TIME);
        }

        if (is::isIn(2, m_type, BLOC_MOVE_VERTI, BLOC_MOVE_HORIZ))
        {
            is::setSFMLObjX_Y(m_sprParent, m_x + m_sprOrigin, (m_y + m_sprOrigin));
        }

        updateCollisionMask();
    }
}

void Bloc::setMoveHorizontal(bool val)
{
    m_moveHorizontal = val;
}

void Bloc::setMoveVertical(bool val)
{
    m_moveVertical = val;
}

void Bloc::setChangeDir(bool val)
{
    m_changeDir = val;
}

bool Bloc::getChangeDir() const
{
    return m_changeDir;
}

bool Bloc::getMoveHorizontal() const
{
    return m_moveHorizontal;
}

bool Bloc::getMoveVertical() const
{
    return m_moveVertical;
}

bool Bloc::hasTexture() const
{
    return m_textureExiste;
}

Bloc::BlocType Bloc::getType() const
{
    return m_type;
}

void Bloc::draw(sf::RenderTexture &surface)
{
    surface.draw(m_sprParent);
}
