#ifndef MAINREAMER_H
#define MAINREAMER_H

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#ifndef ANGLE_RANGE
#define ANGLE_RANGE 3601
#endif

#ifndef METEO
#define METEO 5
#endif

#include<QGLWidget>
#include<QColorDialog>
#include<QVariant>
#include<QTime>
#include<QBasicTimer>
#include<QTimerEvent>
#include<QDebug>
#include"helper.h"

#ifndef ArraySize
template<typename T,size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];
#define ArraySize(array)(sizeof(ArraySizeHelper(array)))
#endif

class MainReamer : public QGLWidget
{
    Q_OBJECT
public:
    explicit MainReamer(QWidget *parent = 0);
    ~MainReamer();
    template<typename OptionType>void SetSettings(const QString group,const QString name,OptionType option);
    template<typename OptionType>void SetSettings(const QString name,OptionType option);
    template<typename T>T CalcScaleValue(const T value)const;
    template<typename T>T CalcScaleValue(const T value,const unsigned int scale) const;
    void GenerationRay();
    void GenerationRay(qint16 angle);
    void GenerationTrash();
    virtual void GenerationRange();
    void GenerationAzimuth();
    void GenerationLocalItems();
    void GenerationActiveNoiseTrash();
    void GenerationActiveAnswerTrash();
    void GenerationActiveInSyncTrash();
    void GenerationTargetPaths();
    void GenerationMeteo();
    void ChangeFPS(qreal fps);
    void CleanDataBuffer();
    void ChangeTargetsState();
    QColor SelectColor(const QString,const QString);
    virtual bool IsActive() const;
    bool clockwise,
         show;
    qint16 targets_pos;

signals:

public slots:
protected:
    void timerEvent(QTimerEvent *event);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    virtual void ContinueSearch();
    void DrawStation() const;
    void LocatorArea() const;
    void DrawTrash() const;
    void DrawRange() const;
    void DrawAzimuth() const;
    void DrawLocalItems() const;
    void DrawActiveNoiseTrash() const;
    void DrawActiveAnswerTrash();
    void DrawActiveInSyncTrash();
    void DrawMeteo() const;
    void DrawTargets();
    qreal GetRandomCoord(quint8,const bool rsign=false) const;
    qint16 GetRandomSign() const;
    quint16 radians_size;
    virtual qreal CalcAlpha(qreal angle) const;
    qreal fps;
    Points radians[ANGLE_RANGE];
    struct Coordinates
    {
        QVector<PointsPath>trash,local_items,meteo;
        QVector<LineEntity>active_noise_trash;
    }Cache;
    QVector<LineEntity>range;
    QVector<Points*>::const_iterator ray_position;
    QVector<Points*>circle,ray;
    QBasicTimer timer;
    QMap<QString,QMap<QString,QVariant> >settings;
    bool not_clean,targets_df;
    void CreateEllipseTrashArea(QVector<PointsPath>&storage,qreal offset_x,qreal offset_y,qreal intensity,bool ellipse,bool clear);
    virtual void CreateEllipseTrashArea(QVector<PointsPath>&storage,qreal begin,qreal end,qreal offset_x,qreal offset_y,qreal intensity,bool ellipse,bool clear);


private:
    void DrawEllipseTrashArea(QVector<PointsPath>storage, quint16 size) const;
    QColorDialog* Color;
    QVector<LineEntity>azimuth;
    QMap<QString,QColor>color;


protected slots:

};

template<typename OptionType>void MainReamer::SetSettings(const QString group,const QString name,OptionType option)
{
    settings[group][name]=QVariant::fromValue(option);
    if(name=="show")
    {
        if(group=="system")
            show=static_cast<bool>(option);
    }
    else if(group=="system")
    {
        if(name=="scale")
        {
            GenerationTrash();
            GenerationRange();
            GenerationLocalItems();
//            if(Cache.meteo.isEmpty())
//                GenerationMeteo();
//            if(Cache.active_noise_trash.isEmpty())
//                GenerationActiveNoiseTrash();
        }
        else if(name=="range")
            GenerationRange();
        else if(name=="azimuth")
            GenerationAzimuth();
        else if(name=="focus")
        {
//            GenerationTrash();
            GenerationRange();
            GenerationAzimuth();
        }
        else if(name=="mode")
        {
            if(option==0)
            {
                SetSettings("trash","show",false);
                SetSettings("local_items","show",false);
                SetSettings("meteo","show",false);
                SetSettings("active_noise_trash","show",false);
                SetSettings("active_answer_trash","show",false);
                SetSettings("active_insync_trash","show",false);
            }
            else if(option==2)
            {
                SetSettings("trash","show",false);
                SetSettings("local_items","show",false);
                SetSettings("meteo","show",false);
                SetSettings("active_noise_trash","show",false);
            }
        }
    }
    else if(group=="trash")
    {
        if(name=="intensity" || name=="begin" || name=="end")
            GenerationTrash();
    }
    else if (group=="active_noise_trash")
    {
//        if(name=="azimuth" || name=="intensity")
//            GenerationActiveNoiseTrash();
    }
    if(group!="common")
        updateGL();
}

template<typename OptionType>void MainReamer::SetSettings(const QString name,OptionType option)
{
    SetSettings("common",name,option);
}

template<typename T>T MainReamer::CalcScaleValue(const T value) const
{
    return Helper::CalcScaleValue(value,settings["system"]["scale"].toUInt());
}

#endif // MAINREAMER_H
