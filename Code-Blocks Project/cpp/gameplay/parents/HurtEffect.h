#ifndef HURTEFFECT_H_INCLUDED
#define HURTEFFECT_H_INCLUDED

#include "../../system/entity/MainObject.h"

class HurtEffect
{
public:
    HurtEffect() : m_isHurtTime(0.f)
    {
    }

    void hurtStep(float const &D_TIME, sf::Sprite &m_sprParent)
    {
        if (m_isHurtTime > 0.f)
        {
            m_isHurtTime -= D_TIME;
            is::setSFMLObjAlpha(m_sprParent, is::choose(5, 25, 200, 100, 25, 150));
        }
        else
        {
            m_isHurtTime = 0.f;
            is::setSFMLObjAlpha(m_sprParent, 255);
        }
    }

    void setIsHurt()
    {
        m_isHurtTime = 100.f;
    }

    bool getIsHurt() const
    {
        return (m_isHurtTime > 0.f);
    }

protected:
    float m_isHurtTime;
};

#endif // HURTEFFECT_H_INCLUDED
