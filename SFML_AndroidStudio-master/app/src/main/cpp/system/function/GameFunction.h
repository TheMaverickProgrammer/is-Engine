#ifndef GAME_FONCTION_H_INCLUDED
#define GAME_FONCTION_H_INCLUDED

#include <cmath>
#include <ctime>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

/// allow to browse object container (std::vector, ...)
#define WITH(_SIZE) for(unsigned int _I = 0; _I < _SIZE; ++_I)

#include "../entity/Forme.h"
#include "../function/GameSystem.h"

#if defined(__ANDROID__)
// These headers are only needed for direct NDK/JDK interaction
#include <jni.h>
#include <android/native_activity.h>
#include <android/log.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>
#endif // defined

namespace is
{
/// game execution timing variables
static float const MAX_CLOCK_TIME(0.018f);
static float const VALUE_CONVERSION(65.f);
static float const VALUE_TIME(1.538f);

#if !defined(__ANDROID__)
static float const M_PI(3.14f);
#endif // defined

/// convert number to string (Note : include "sstream" header)
template <class T>
std::string numToStr(T val)
{
    std::ostringstream s;
    s << val;
    return s.str();
}

template <class T>
std::wstring numToWStr(T val)
{
    std::wostringstream ws;
    ws << val;
    const std::wstring s(ws.str());
    return s;
}

/// draw zero behind a number
template <class T>
std::string writeZero(T val, int zeroNumber = 1)
{
    std::string str;
    for (int i(0); i < zeroNumber; i++)
        if (val < std::pow(10, (i + 1))) str += "0";
    return (str + is::numToStr(val));
}

/// return game execution time in millisecond
int getMSecond(float const &DELTA_TIME);

/// show log message
void showLog(std::string str);

/// get array size
template <size_t SIZE, class T>
inline size_t arraySize(T (&arr)[SIZE])
{
    return SIZE;
}

/// return a random value
template <typename T>
T choose(unsigned short valNumber, T x1, T x2, T x3 = 0, T x4 = 0, T x5 = 0, T x6 = 0, T x7 = 0, T x8 = 0, T x9 = 0)
{
    unsigned int randVal = rand() % valNumber;

    switch(randVal)
    {
        case 1 : return x2; break;
        case 2 : return x3; break;
        case 3 : return x4; break;
        case 4 : return x5; break;
        case 5 : return x6; break;
        case 6 : return x7; break;
        case 7 : return x8; break;
        case 8 : return x9; break;
        default : break;
    }
    return x1;
}

/// set variable limit sprite
template <typename T>
void setVarLimit(T &var, T valMin, T valMax)
{
    if (var < valMin) var = valMin;
    if (var > valMax) var = valMin;
}

/// test many values in comparison with a variable
bool isIn(unsigned short valNumber, int const var, int x1, int x2, int x3 = 0, int x4 = 0, int x5 = 0, int x6 = 0, int x7 = 0, int x8 = 0, int x9 = 0);

/// return if a is in [b,c]
bool isBetween(float a, float b, float c);

/// return if [l1,r1] intersect [l2,r2]
bool isCrossing(float l1, float r1, float l2, float r2);

/// test if the point m is on the right side of the vector ab
/// return -1 on the left, 1 on the right, o if a b c are aligned
int side(Point m, Point a,Point b);

/// return sign of x
int sign(float x);

/// return angle between two points (x1, y1) and (x2, y2)
template <typename T>
T pointDirection(float x1, float y1, float x2, float y2)
{
    if (static_cast<int>(x1) == static_cast<int>(x2))
    {
        if (y2 > y1) return (0.5f * M_PI);
        else return (1.5f * M_PI);
    }
    float result = std::atan((y2 - y1) / (x2 - x1));
    if (x2 < x1) result += M_PI;
    if (result < 0.f) result += 2.f * M_PI;
    result *= 180.f / M_PI;
    return result;
}

/// return distance between two points (x1, y1) and (x2, y2)
float pointDistance(float x1, float y1, float x2, float y2);

/// convert radian to grade
float radToDeg(float x);

/// convert grade to radian
float degToRad(float x);

/// return x component of the vector
float lengthDirX(float dir, float angle);

/// return y component of the vector
float lengthDirY(float dir, float angle);

/// allow to make scale animation
template <class T>
void scaleAnimation(float const &DELTA_TIME, float &var, T &obj, short varSign = 1, float scaleSize = 1.f)
{
    if (var > scaleSize) var -= ((0.05f * is::VALUE_CONVERSION) * DELTA_TIME);
    else var = scaleSize;
    obj.setScale(varSign * var, varSign * var);
}

/// allows to increment a variable while controlling the upper limit
/// increaseValue parameter will be multiplied later by is::VALUE_CONVERSION
template <typename T>
void increaseVar(const float &DELTA_TIME, T &var, T increaseValue, T varFinal, T varMax)
{
    if (var < varMax) var += ((increaseValue * is::VALUE_CONVERSION) * DELTA_TIME);
    else var = varFinal;
}

/// allows to decrement a variable while controlling the lower limit
/// decreasingValue parameter will be multiplied later by is::VALUE_CONVERSION
template <typename T>
void decreaseVar(const float &DELTA_TIME, T &var, T decreaseValue, T varFinal = 0, T varMin = 0)
{
    if (var > varMin) var -= ((decreaseValue * is::VALUE_CONVERSION) * DELTA_TIME);
    else var = varFinal;
}

/// return the angle of SFML object
template <class T>
float getSFMLObjAngle(T obj)
{
    return obj.getRotation();
}

/// return the x scale size of SFML object
template <class T>
float getSFMLObjXScale(T obj)
{
    return obj.getScale().x;
}

/// return the y scale size of SFML object
template <class T>
float getSFMLObjYScale(T obj)
{
    return obj.getScale().y;
}

/// return the x width of SFML object
template <class T>
float getSFMLObjWidth(T obj)
{
    return obj.getGlobalBounds().width;
}

/// return the y height of SFML object
template <class T>
float getSFMLObjHeight(T obj)
{
    return obj.getGlobalBounds().height;
}

/// return the x origin of SFML object
template <class T>
float getSFMLObjOriginX(T obj)
{
    return obj.getOrigin().x;
}

/// return the y origin of SFML object
template <class T>
float getSFMLObjOriginY(T obj)
{
    return obj.getOrigin().y;
}

/// return the x position of SFML object
template <class T>
float getSFMLObjX(T obj)
{
    return obj.getPosition().x;
}

/// return the y position of SFML object
template <class T>
float getSFMLObjY(T obj)
{
    return obj.getPosition().y;
}

/// return the x position of SFML object (pointer object)
template <class T>
float getSFMLObjX(T *obj)
{
    return obj->getPosition().x;
}

/// return the y position of SFML object (pointer object)
template <class T>
float getSFMLObjY(T *obj)
{
    return obj->getPosition().y;
}

/// set the angle of SFML object
template <class T>
void setSFMLObjAngle(T &obj, float angle)
{
    obj.setRotation(angle);
}

/// set rotation of SFML object
template <class T>
void setSFMLObjRotate(T &obj, float rotationSpeed)
{
    obj.rotate(rotationSpeed);
}

/// set the x, y scale of SFML object
template <class T>
void setSFMLObjScaleX_Y(T &obj, float x, float y)
{
    obj.setScale(x, y);
}

/// set the scale of SFML object
template <class T>
void setSFMLObjScale(T &obj, float scale)
{
    obj.setScale(scale, scale);
}

/// set origin of SFML object
template <class T>
void setSFMLObjOrigin(T &obj, float x, float y)
{
    obj.setOrigin(sf::Vector2f(x, y));
}

/// set x position of SFML object
template <class T>
void setSFMLObjX(T &obj, float x)
{
    obj.setPosition(x, obj.getPosition().y);
}

/// set y position of SFML object
template <class T>
void setSFMLObjY(T &obj, float y)
{
    obj.setPosition(obj.getPosition().x, y);
}

/// set x, y position of SFML object
template <class T>
void setSFMLObjX_Y(T &obj, sf::Vector2f position)
{
    obj.setPosition(position.x, position.y);
}

/// set x, y position of SFML object
template <class T>
void setSFMLObjX_Y(T &obj, float x, float y)
{
    obj.setPosition(x, y);
}

/// set SFML object size
template <class T>
void setSFMLObjSize(T &obj, float x, float y)
{
    obj.setSize(sf::Vector2f(x, y));
}

/// set the alpha of SFML object
template <class T>
void setSFMLObjAlpha(T &obj, unsigned int alpha)
{
    obj.setColor(sf::Color(255, 255, 255, alpha));
}

/// set the alpha and RGB distinct color of SFML object
template <class T>
void setSFMLObjAlpha(T &obj, unsigned int alpha, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
{
    obj.setColor(sf::Color(r, g, b, alpha));
}

/// set the alpha and uniform RGB color of SFML object
template <class T>
void setSFMLObjAlpha(T &obj, unsigned int alpha, sf::Uint8 rgb)
{
    obj.setColor(sf::Color(rgb, rgb, rgb, alpha));
}

/// set the color of SFML object
template <class T>
void setSFMLObjColor(T &obj, sf::Color color)
{
    obj.setColor(color);
}

/// set the sprite frame (animation)
void setFrame(sf::Sprite &sprite, float frame, int subFrame, int frameSize = 32, int recWidth = 32, int recHeight = 32);

/// set the alpha and RGB distinct color of SFML Text object
void setSFMLTextAlpha(sf::Text &obj, unsigned int alpha, sf::Uint8 r = 255, sf::Uint8 g = 255, sf::Uint8 b = 255);

/// set the color of SFML Text object
void setSFMLTextColor(sf::Text &obj, sf::Color color);

/// set the outline color of SFML object
template <class T>
void setSFMLTextOutlineColor(T &obj, sf::Color color)
{
    obj.setOutlineColor(color);
}

/// set the outline thickness and color of SFML object
template <class T>
void setSFMLTextOutlineColor(T &obj, float thickness, sf::Color color)
{
    obj.setOutlineThickness(thickness);
    obj.setOutlineColor(color);
}

/// set the alpha and RGB distinct color of SFML Rectangle object
void setSFMLRecAlpha(sf::RectangleShape &obj, unsigned int alpha, sf::Uint8 r = 255, sf::Uint8 g = 255, sf::Uint8 b = 255);

/// set the color of SFML Rectangle object
void setSFMLRecColor(sf::RectangleShape &obj, sf::Color color);

/// set Texture Rec of SFML object
template <class T>
void setSFMLObjTexRec(T &obj, int x, int y, int w = 32, int h = 32)
{
    obj.setTextureRect(sf::IntRect(x, y, w, h));
}

/// set the graphic properties of SFML object
template <class T>
void setSFMLObjProperties(T &obj, float x, float y, float angle = 0.f, int alpha = 255, float xScale = 1.f, float yScale = 1.f)
{
    is::setSFMLObjAlpha(obj, alpha);
    is::setSFMLObjAngle(obj, angle);
    is::setSFMLObjScaleX_Y(obj, xScale, yScale);
    is::setSFMLObjX_Y(obj, x, y);
}

/// get SFML Sound state (0 = Playing, 1 = Stopped, 2 = Pause)
template <class T>
bool getSFMLSndState(T &obj, int stateIndex)
{
    switch (stateIndex)
    {
        case 0:
            return (obj.getStatus() == sf::Sound::Status::Playing);
        break;
        case 1:
            return (obj.getStatus() == sf::Sound::Status::Stopped);
        break;
        case 2:
            return (obj.getStatus() == sf::Sound::Status::Paused);
        break;
    }
    return false;
}

/*
// test whether there are a collision between the two rectangle r1 and r2
bool isCollision(Rectangle r1, Rectangle r2);
bool isCollision(Rectangle r, Point p);
bool isCollision(Point p, Rectangle r);
bool isCollision(Rectangle r, Line l);
bool isCollision(Line l, Rectangle r);
bool isCollision(Line l1,Line l2);
*/

/// test collision between SFML object
template <class A, class B>
bool collisionTestSFML(A const &objA, B const &objB)
{
    return (objB.getGlobalBounds().intersects(objA.getGlobalBounds()));
}

/// test collision between two rectangles
bool collisionTest(Rectangle const &firstBox, Rectangle const &secondBox);

/// create SFML rectangle
void createRectangle(sf::RectangleShape &rec, sf::Vector2f recSize, sf::Color color, float x = 0.f, float y = 0.f, bool center = true);

/// set SFML Text style
void textStyleConfig(sf::Text &txt, bool underLined, bool boldText, bool italicText);

/// create SFML wtext with color
void createWText(sf::Font const& fnt, sf::Text &txt, std::wstring const &text, float x, float y, sf::Color color, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML text
void createText(sf::Font const& fnt, sf::Text &txt, std::string const &text, float x, float y, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML text with center parameter
void createText(sf::Font const& fnt, sf::Text &txt, std::string const &text, float x, float y, bool centerText, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML text with color and size
void createText(sf::Font const& fnt, sf::Text &txt, std::string const &text, float x, float y, sf::Color color, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML text with color, size and center
void createText(sf::Font const& fnt, sf::Text &txt, std::string const &text, float x, float y, sf::Color color, bool centerText, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML text outline with color and size
void createText(sf::Font const& fnt, sf::Text &txt, std::string const &text, float x, float y, sf::Color color, sf::Color outlineColor, int txtSize = 20, bool underLined = false, bool boldText = false, bool italicText = false);

/// create SFML sprites without IntRec
void createSprite(sf::Texture &tex, sf::Sprite &spr, sf::Vector2f position, sf::Vector2f origin, bool smooth = true);

/// create SFML sprites with IntRec
void createSprite(sf::Texture &tex, sf::Sprite &spr, sf::IntRect rec, sf::Vector2f position, sf::Vector2f origin, bool repeatTexture = false, bool smooth = true);

/// create SFML sprites advanced
void createSprite(sf::Texture &tex, sf::Sprite &spr, sf::IntRect rec, sf::Vector2f position, sf::Vector2f origin, sf::Vector2f scale, unsigned int alpha = 255, bool repeatTexture = false, bool smooth = true);

/// center SFML object
template <class T>
void centerSFMLObj(T &obj)
{
    obj.setOrigin(obj.getGlobalBounds().width / 2, obj.getGlobalBounds().height / 2);
}

/// center SFML object X
template <class T>
void centerSFMLObjX(T &obj)
{
    obj.setOrigin(obj.getGlobalBounds().width / 2, obj.getOrigin().y);
}

/// center SFML object Y
template <class T>
void centerSFMLObjY(T &obj)
{
    obj.setOrigin(obj.getOrigin().x, obj.getGlobalBounds().height / 2);
}

/// allow to use Android vibrate
int vibrate(sf::Time duration);

/// open URL in Android default navigator
void openURL(std::string urlStr);

#if defined(__ANDROID__)
/// allow to deactivate / activate Android screen lock
void setScreenLock(bool disableLock);

/// convert JNI String to std::string
static std::string jstring2string(JNIEnv *env, jstring jStr);

/// return Android terminal device
static std::string getDeviceId(JNIEnv *env, ANativeActivity *activity);
#endif // defined
}

#endif // GAME_FONCTION_H_INCLUDED
