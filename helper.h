#ifndef HELPER_H
#define HELPER_H
#include<QWidget>
#include<qmath.h>

#ifndef GetRadianValue
#define GetRadianValue(radian) M_PI*radian/180
#endif

struct Daddy
{
    qreal x,angle;
};

struct Points:public Daddy
{
    qreal y;
};
struct PointsPath:public Points
{
    qreal r;
};
struct LineEntity
{
    qreal width;
    Points *Coordinates;
};

struct Targets
{
    Daddy Coordinates[5];
    qreal speed;
    quint8 landing;
};

class Helper
{
    public:
        template<typename T>static T CalcScaleValue(const T value,const unsigned int scale);
};

template<typename T>T Helper::CalcScaleValue(const T value,const unsigned int scale)
{
    return value/scale;
//    return 10u;
}
#endif // HELPER_H
