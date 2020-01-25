#ifndef SCOREPOINT_H_INCLUDED
#define SCOREPOINT_H_INCLUDED

class ScorePoint
{
public:
    ScorePoint(int point = 0) :
        m_scorePoint(point)
    {

    }

    virtual void setScorePoint(int point)
    {
        m_scorePoint = point;
    }

    virtual int getScorePoint() const
    {
        return m_scorePoint;
    }

protected:
    int m_scorePoint;
};

#endif // SCOREPOINT_H_INCLUDED
