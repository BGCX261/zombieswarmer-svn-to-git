#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_

#include "cocos2d.h"

using namespace cocos2d;

class HealthBar : public CCNode
{
public:
    HealthBar(int maxVal = 10, int value = 10);
    ~HealthBar(void);

    void draw(void);

    CCSize size()                                       { return m_size; }
    void setSize(CCSize sz)                             { m_size = sz; }
    void setMaxValue(int val)                           { m_maxVal = val; }
    int value()                                         { return m_value; }
    void setValue(int value)                            { m_value = value; }

private:

    void ccFillPoly(const CCPoint *vertices, int numOfVertices, bool closePolygon);

    CCSize              m_size;
    int                 m_maxVal;
    int                 m_value;
};

#endif//_HEALTHBAR_H_