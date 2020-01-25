#ifndef MAINOBJECT_H_INCLUDED
#define MAINOBJECT_H_INCLUDED

#include "../entity/Forme.h"
#include "../function/GameFunction.h"
#include "../display/GameDisplay.h"

namespace is
{
////////////////////////////////////////////////////////////
/// \brief Main Class for all physiques objects
////////////////////////////////////////////////////////////
class MainObject
{
public:
    MainObject();
    MainObject(float x, float y);
    virtual ~MainObject();

    /// return the instance number
    static int instanceNumber;

    /// check if the object is in view surface
    virtual bool inViewRec(GameDisplay const &app, bool useTexRec = true);

    /// set x initial position
    virtual void setXStart(float x);

    /// set y initial position
    virtual void setYStart(float y);

    /// set x preview position
    virtual void setXPrevious(float x);

    /// set y preview position
    virtual void setYPrevious(float y);

    /// set x, y initial position
    virtual void setStartPosition(float x, float y);

    /// set x position
    virtual void setX(float x);

    /// set y position
    virtual void setY(float y);

    /// move object on x axis
    virtual void moveX(float x, float const& DELTA_TIME);

    /// move object on y axis
    virtual void moveY(float y, float const& DELTA_TIME);

    /// set x, y position
    virtual void setPosition(float x, float y);

    /// set x, y scale of main sprite
    virtual void setSpriteScale(float x, float y);

    /// set speed
    virtual void setSpeed(float val);

    /// set horizontal speed
    virtual void setHsp(float val);

    /// set vertical speed
    virtual void setVsp(float val);

    /// set angular move
    virtual void setAngularMove(float const &DELTA_TIME, float speed, float angle);

    /// set the x scale
    virtual void setImageXscale(float val);

    /// set the y scale
    virtual void setImageYscale(float val);

    /// set the scale
    virtual void setImageScale(float val);

    /// set the angle
    virtual void setImageAngle(float val);

    /// set the x offset
    virtual void setXOffset(float val);

    /// set the y offset
    virtual void setYOffset(float val);

    /// set the x, y offset in relation to parent sprite origin
    virtual void setXYOffset();

    /// set image scale
    virtual void setImageScale(float x, float y);

    /// set time
    void setTime(float x);

    /// set alpha
    virtual void setImageAlpha(int val);

    /// set image index
    virtual void setImageIndex(int val);

    /// set mask w
    virtual void setMaskW(int val);

    /// set mask h
    virtual void setMaskH(int val);

    /// set active
    virtual void setIsActive(bool val);

    /// update the collision mask
    virtual void updateCollisionMask();

    /// update the collision mask with independent x, y point
    virtual void updateCollisionMask(int x, int y);

    /// update the collision mask by centering it in relation to the position of the object
    virtual void centerCollisionMask(int x, int y);

    /// update object main sprite
    virtual void updateSprite();

    /// update object main sprite with external parameter
    virtual void updateSprite(float x, float y, float angle = 0.f, int alpha = 255, float xScale = 1.f, float yScale = 1.f);

    /// draw the main sprite of object
    virtual void draw(sf::RenderTexture &surface) = 0;

    /// return the rectangle mask
    virtual is::Rectangle getMask() const;

    /// return the x position
    virtual float getX() const;

    /// return the y position
    virtual float getY() const;

    /// return the x initial position
    virtual float getXStart() const;

    /// return the y initial position
    virtual float getYStart() const;

    /// return the x previous position
    virtual float getXPrevious() const;

    /// return the y previous position
    virtual float getYPrevious() const;

    /// return the distance between this object and point (x, y)
    virtual float distantToPoint(float x, float y) const;

    /// return the distance between this object and another
    virtual float distantToObject(MainObject const *other, bool useSpritePosition) const;

    /// return the angle between this object and point (x, y)
    virtual float pointDirection(float x, float y) const;

    /// return the angle between this object and another
    virtual float pointDirection(MainObject const *other) const;

    /// return the angle between this object main sprite and point (x, y)
    virtual float pointDirectionSprite(float x, float y) const;

    /// return the angle between this object main sprite and another object
    virtual float pointDirectionSprite(MainObject const *other) const;

    /// return the speed
    virtual float getSpeed() const;

    /// return the horizontal speed
    virtual float getHsp() const;

    /// return the vertical speed
    virtual float getVsp() const;

    /// return frame
    virtual float getFrame() const;

    /// return frame start
    virtual float getFrameStart() const;

    /// return frame end
    virtual float getFrameEnd() const;

    /// return the x scale
    virtual float getImageXscale() const;

    /// return the y scale
    virtual float getImageYscale() const;

    /// return the x scale
    float getImageScale() const;

    /// return the angle
    virtual float getImageAngle() const;

    /// return x offset
    virtual float getXOffset() const;

    /// return y offset
    virtual float getYOffset() const;

    /// return object timing variable
    virtual float getTime() const;

    /// return the ID of object (instance number)
    virtual int getInstanceId() const;

    /// return the width of collision mask
    virtual int getMaskWidth() const;

    /// return the height of collision mask
    virtual int getMaskHeight() const;

    /// return the active value
    virtual bool getIsActive() const;

    /// return image alpha
    virtual int getImageAlpha() const;

    /// return image index
    virtual int getImageIndex() const;

    /// return the width of main sprite
    virtual int getSpriteWidth() const;

    /// return the height of main sprite
    virtual int getSpriteHeight() const;

    /// return x of main sprite
    virtual float getSpriteX() const;

    /// return y of main sprite
    virtual float getSpriteY() const;

    /// return the x center of main sprite
    virtual int getSpriteCenterX() const;

    /// return the y center of main sprite
    virtual int getSpriteCenterY() const;

    /// test collision in comparison with another
    virtual bool placeMetting(int x, int y, MainObject const *other);

    /// return the sprite of object
    virtual sf::Sprite& getSprite();

protected:
    /// set frame limit
    virtual void setFrame(float frameStart, float frameEnd = -1.f);

    float m_x, m_y, m_xStart, m_yStart, m_xPrevious, m_yPrevious;
    float m_speed, m_hsp, m_vsp;
    float m_frameStart, m_frameEnd, m_frame, m_imageXscale, m_imageYscale, m_imageScale, m_imageAngle;
    float m_xOffset, m_yOffset;
    float m_time;
    int m_w, m_h, m_instanceId, m_imageAlpha, m_imageIndex;
    bool m_isActive;
    is::Rectangle m_aabb;
    sf::Sprite m_sprParent;
};

/// functor for compare the x position of objects
class CompareX
{
public:
    bool operator()(MainObject const *a, MainObject const *b) const;
};
bool operator<(const MainObject *a, const MainObject &b);
bool operator<(const MainObject &b, const MainObject *a);

/// check if instance exists
template<class T>
bool instanceExist(T const *obj)
{
    return (obj != 0);
}

/// check if instance is destroyed
template<class T>
void instanceDestroy(T *obj)
{
    if (is::instanceExist(obj))
    {
        delete obj;
        obj = 0;
    }
}

/// sort object array
template<class T>
void sortObjArray(std::vector<T> &v)
{
    std::sort(v.begin(), v.end(), is::CompareX());
}
}

#endif // MAINOBJECT_H_INCLUDED
