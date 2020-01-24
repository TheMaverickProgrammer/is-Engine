#ifndef HEALTH_H_INCLUDED
#define HEALTH_H_INCLUDED

class Health
{
public:
    Health(int healt):
        m_health(healt)
    {

    }

    virtual void setHealth(int val)
    {
        m_health = val;
        if (m_health < 0) m_health = 0;
    }

    virtual int getHealth() const
    {
        return m_health;
    }

protected:
    int m_health;
};

#endif // HEALTH_H_INCLUDED
