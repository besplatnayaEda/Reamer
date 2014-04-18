#include "mainreamer.h"

MainReamer::MainReamer(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    clockwise=true; //По часовой стрелке
    not_clean=true;
    show=true;
    targets_df=true;
    Color=new QColorDialog(this);
    qsrand(QTime(0u,0u,0u).secsTo(QTime::currentTime()));

    //Переведём все используемые градусы в радианы
    for(quint16 i=0u;i<361/*ANGLE_RANGE*/;i++)
    {
        radians[i].angle=GetRadianValue(i);
        radians[i].x=qFastCos(radians[i].angle);
        radians[i].y=qFastSin(radians[i].angle);
    }
    radians_size=ArraySize(radians);
    circle.clear();
    for(Points*i=radians,*end=radians+radians_size;i<end;circle.append(i),i+=3u); //Получаем координаты для отрисовки фона индикатора
    GenerationRay();
    ray_position=ray.begin(); //Устанавливаем стартовую позицию луча
    ChangeFPS(0);
}

void MainReamer::timerEvent(QTimerEvent *event)
{
    if(timer.timerId()==event->timerId())
        ContinueSearch();
    QWidget::timerEvent(event);
}

MainReamer::~MainReamer()
{
    if(IsActive())
    killTimer(timer.timerId());
}

void MainReamer::initializeGL()
{
//    qglClearColor(palette().background().color()); //Фон OpenGl-виджета
    qglClearColor(Qt::black);
    glMatrixMode(GL_PROJECTION); //Устанавливаем матрицу
    glShadeModel(GL_SMOOTH);//GL_FLAT
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
//    glEnable(GL_DEPTH_TEST);
}

void MainReamer::resizeGL(int width, int height)
{
    glEnable(GL_MULTISAMPLE);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

//    glViewport(static_cast<GLint>(0u/*width/2-height/2*/),static_cast<GLint>(0u),static_cast<GLint>(768u),static_cast<GLint>(768u));
    if(width>height)
        glViewport(static_cast<GLint>(width/2-height/2),static_cast<GLint>(0u),static_cast<GLint>(height),static_cast<GLint>(height));
    else
        glViewport(static_cast<GLint>(0u),static_cast<GLint>(height/2-width/2),static_cast<GLint>(width),static_cast<GLint>(width));
    glOrtho(0, 0, 0, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void MainReamer::paintGL() // none
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glLoadIdentity(); // загружаем матрицу
    glPushMatrix();
    glLineWidth(2.0f*1u*settings["system"]["focus"].toDouble());
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    LocatorArea();
//    DrawStation();
    glColor4f(static_cast<GLfloat>(.0),static_cast<GLfloat>(1.0),static_cast<GLfloat>(.0),settings["system"]["brightness"].toFloat());//перерисовка линии
    glRotatef(90.0f,.0f,.0f,1.0);
    glBegin(GL_LINES);
        glVertex2d(static_cast<GLdouble>(.0f),static_cast<GLdouble>(.0f));
        glVertex2d((*ray_position)->x,(*ray_position)->y);
    glEnd();
    if(settings["trash"]["show"].toBool() && !Cache.trash.isEmpty())
//        DrawTrash();
    if(!range.isEmpty())
        DrawRange();
    if(!azimuth.isEmpty())
        DrawAzimuth();
    if(settings["local_items"]["show"].toBool() && !Cache.local_items.isEmpty())
//        DrawLocalItems();
    if(settings["meteo"]["show"].toBool() && !Cache.meteo.isEmpty())
//        DrawMeteo();
    if(settings["active_noise_trash"]["show"].toBool() && !Cache.active_noise_trash.isEmpty())
//        DrawActiveNoiseTrash();
//    if(!TargetsSettings::targets.isEmpty())
//        DrawTargets();

    glPopMatrix();
}

void MainReamer::ChangeFPS(qreal fps)
{
    if(fps<=.0f && IsActive())
        timer.stop();
    if(fps>.0f)
    {
        if(IsActive())
            timer.stop();
        timer.start(fps,this);
    }
}

bool MainReamer::IsActive() const
{
    return timer.isActive();
}

void MainReamer::CleanDataBuffer()
{

}

QColor MainReamer::SelectColor(const QString option,const QString title="")
{
    if(!title.isEmpty())
        Color->setWindowTitle("title");
    return color[option]=Color->getColor();
}

void MainReamer::GenerationRay()
{
    GenerationRay(radians_size);
}

void MainReamer::GenerationRay(qint16 angle)
{
    qint16 fr=1;
//    fr=settings["system"]["freq"].toInt();
    ray.clear();
    Points*i=radians,*end=radians+angle;
    while(i<end)ray.append(clockwise ? end=end-fr : i=i+fr);
//    updateGL();
}

qreal MainReamer::CalcAlpha(qreal angle) const
{
    qreal alpha;
    if(settings["system"]["show"].toBool())
        alpha=1.0f;
    else
    {
        alpha=(clockwise ? -1 : 1)*((*ray_position)->angle-angle)-.01;
        if(not_clean && alpha<.0f)
            alpha+=2u*M_PI;
    }
    return alpha;
}

void MainReamer::GenerationRange()
{
    qreal r=.0f,delta,distance;
    quint8 j=0u,d=0u;
    range.clear();

    distance=CalcScaleValue(1.0f);
    switch(settings["system"]["range"].toUInt())
    {
        case 1:
            delta=distance*10u;
            j=5u;
            break;
        case 0:
            return;
        default:
            delta=distance*50u;
            j=1u;
    }

    LineEntity cache;
    quint16 c;
    while(r<=1u)
    {
        cache.width=d%j==0u ? 3.5f : 1.0f;
        cache.Coordinates=new Points[radians_size];
        c=0u;
        for(Points *i=radians,*end=radians+radians_size;i<end;i++,c++)
        {
            cache.Coordinates[c].angle=i->angle;
            cache.Coordinates[c].x=r*i->x;
            cache.Coordinates[c].y=r*i->y;
        }
        range.append(cache);
        r+=delta;
        d++;
    }
}

void MainReamer::DrawRange() const
{
    qreal alpha;
    for(QVector<LineEntity>::const_iterator it=range.begin();it<range.end();it++)
    {
        glLineWidth(it->width);
        glBegin(GL_LINE_STRIP);
        for(Points *i=it->Coordinates,*end=it->Coordinates+radians_size;i<end;i++)
        {
            alpha=CalcAlpha(i->angle);
            if(alpha>.0f)
            {
                alpha=alpha<settings["system"]["lightning"].toDouble() ? 1.0f : settings["system"]["lightning"].toDouble()/alpha;
                glColor4f(static_cast<GLfloat>(.0),static_cast<GLfloat>(1.0),static_cast<GLfloat>(.0),alpha*settings["system"]["brightness"].toDouble());
                glVertex2d(i->x,i->y);
            }
        }
        glEnd();
    }
}

void MainReamer::GenerationAzimuth()
{
    quint16 delta;
    azimuth.clear();
    switch(settings["system"]["azimuth"].toUInt())
    {
        case 1:
            delta=5u;
            break;
        case 0:
            delta=360u;
            break;
        default:
            delta=10u;
    }
    LineEntity cache;
    for(Points *i=radians,*k=radians+radians_size;i<k;i+=delta)
    {
        cache.width=(i-radians)%30u>0u ? 1.0f : 3.5f;
        cache.Coordinates=new Points[1];
        cache.Coordinates->angle=i->angle;
        cache.Coordinates->x=i->x;
        cache.Coordinates->y=i->y;
        azimuth.append(cache);
    }
}

void MainReamer::DrawAzimuth() const
{
    qreal alpha;
    for(QVector<LineEntity>::const_iterator it=azimuth.begin();it<azimuth.end();it++)
    {
        alpha=CalcAlpha(it->Coordinates->angle);
        if(alpha>.0f)
        {
            glLineWidth(it->width*settings["system"]["focus"].toDouble());
            alpha=alpha<settings["system"]["lightning"].toDouble() ? 1.0f : settings["system"]["lightning"].toDouble()/alpha;
            glBegin(GL_LINES);
            glColor4f(static_cast<GLfloat>(.0),static_cast<GLfloat>(1.0),static_cast<GLfloat>(.0),alpha*settings["system"]["brightness"].toDouble());
            glVertex2d(.0f,.0f);
            glVertex2f(it->Coordinates->x,it->Coordinates->y);
            glEnd();
        }
    }
}

void MainReamer::ContinueSearch()
{
    updateGL();
    if(ray_position==ray.end()-1u)
    {
        if(!not_clean)
            not_clean=true;
        ray_position=ray.begin();
        targets_df=true;
    }
    ray_position++;
}

void MainReamer::CreateEllipseTrashArea(QVector<PointsPath>&storage,qreal offset_x,qreal offset_y,qreal intensity,bool ellipse,bool clear=true)
{
    return CreateEllipseTrashArea(storage,.0f,GetRandomCoord(4u)*10u,offset_x,offset_y,intensity,ellipse,clear);
}

void MainReamer::CreateEllipseTrashArea(QVector<PointsPath>&storage,qreal begin,qreal end,qreal offset_x,qreal offset_y,qreal intensity=3.0f,bool ellipse=false,bool clear=true)
{
    qreal rand;
    begin=CalcScaleValue(begin),
    end=CalcScaleValue(end);
    if(clear)
        storage.clear();
    PointsPath cache;
    for(Points*i=radians,*k=radians+radians_size;i<k;i++)
    {
        for(quint16 l=0u,t=fmod(qrand(),intensity);l<t;l++)
        {
            if(ellipse)
            {
                rand=begin+fmod(GetRandomCoord(4u),end-begin);
                cache.x=i->x*rand+CalcScaleValue(offset_x)+GetRandomSign();//*CalcScaleValue(offset_x*rand);
                rand=begin+fmod(GetRandomCoord(4u),end-begin);
                cache.y=i->y*rand+CalcScaleValue(offset_y)+GetRandomSign();//*CalcScaleValue(offset_y*rand);
            }
            else
            {
                rand=begin+fmod(GetRandomCoord(4u),end-begin);
                cache.x=i->x*rand+CalcScaleValue(offset_x);
                cache.y=i->y*rand+CalcScaleValue(offset_y);
            }
            cache.r=qSqrt(qPow(cache.x,2u)+qPow(cache.y,2u));
            if(offset_x>.0f || offset_y>.0f)
                if(cache.x>0)
                    cache.angle=qAtan2(cache.y,cache.x);
                else if(cache.x==0)
                    cache.angle=M_PI/2;
                else
                    cache.angle=qAtan2(cache.y,cache.x);
            else
                cache.angle=i->angle;
            storage.append(cache);
        }
    }
}

qreal MainReamer::GetRandomCoord(quint8 accuracy,const bool rsign) const
{
    //Фикс странного бага, наблюдающегося под виндой
    if(accuracy>4u)
        accuracy=4u;

    qreal a=(qrand()%quint32(qPow(10u,accuracy)+1u))/qPow(10u,accuracy);
    if(rsign)
        return a*GetRandomSign();
    return a;
}

qint8 MainReamer::GetRandomSign() const
{
    if(rand()%2u)
        return 1u;
    return-1;
}

void MainReamer::LocatorArea() const
{
    color["locator"].isValid() ? qglColor(color["locator"]) : qglColor(QColor(255,153,0,255));
    glBegin(GL_TRIANGLE_FAN);
        for(QVector<Points*>::const_iterator it=circle.begin();it<circle.end();it++)
            glVertex2d((*it)->x,(*it)->y);
    glEnd();
}

void MainReamer::DrawEllipseTrashArea(QVector<PointsPath>storage,quint8 size=8u) const
{
    glPointSize(size*settings["system"]["focus"].toDouble());
    glEnable(GL_ALPHA_TEST);
    qreal alpha;
    for(QVector<PointsPath>::const_iterator it=storage.begin();it<storage.end();it++)
    {
        alpha=CalcAlpha(it->angle);
        if(alpha>.0f)
        {
            alpha=alpha<settings["system"]["lightning"].toDouble() ? 1.0f : settings["system"]["lightning"].toDouble()/alpha;
            alpha*=settings["system"]["brightness"].toDouble()+it->r-settings["system"]["varu"].toDouble();
            glBegin(GL_POINTS);
            glColor4f(static_cast<GLfloat>(.0),static_cast<GLfloat>(1.0),static_cast<GLfloat>(.0),alpha*settings["system"]["brightness"].toDouble());
            glVertex2f(it->x,it->y);
            glEnd();
        }
    }
}

void MainReamer::GenerationTrash()
{
    CreateEllipseTrashArea(Cache.trash,settings["trash"]["begin"].toDouble(),settings["trash"]["end"].toDouble(),.0f,.0f,settings["trash"]["intensity"].toUInt() ?: 1u);
}

void MainReamer::GenerationLocalItems()
{
    CreateEllipseTrashArea(Cache.local_items,.0f,15.0f,.0f,.0f);
}

void MainReamer::GenerationMeteo()
{
    CreateEllipseTrashArea(Cache.meteo,20.0f,20.0f,3u,true);
    CreateEllipseTrashArea(Cache.meteo,-20.0f,20.0f,3u,true,false);
    CreateEllipseTrashArea(Cache.meteo,-30.0f,-30.0f,3u,true,false);
    CreateEllipseTrashArea(Cache.meteo,-50.0f,-10.0f,3u,true,false);
}

void MainReamer::GenerationActiveNoiseTrash()
{
    quint8 density=17;
    qint16 angle;
    Cache.active_noise_trash.clear();

    LineEntity cache;
    switch(settings["active_noise_trash"]["intensity"].toUInt())
    {
        case 0:
            angle=settings["active_noise_trash"]["azimuth"].toUInt();
            for(Points*i=radians+radians_size-angle,*k=radians+radians_size-angle+20;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                Cache.active_noise_trash.append(cache);
            }
            if(angle<20)
                for(Points*i=radians,*k=radians+20-angle;i<k;i++)
                {
                    cache.Coordinates=new Points[1];
                    cache.Coordinates->angle=i->angle;
                    cache.Coordinates->x=i->x;
                    cache.Coordinates->y=i->y;
                    cache.width=GetRandomCoord(4)*density;
                    Cache.active_noise_trash.append(cache);
                }
            break;
        case 1:
            angle=settings["active_noise_trash"]["azimuth"].toUInt();
            for(Points*i=radians+radians_size-angle,*k=radians+radians_size-angle+30;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                Cache.active_noise_trash.append(cache);
            }
            angle=settings["active_noise_trash"]["azimuth"].toUInt()+100;
            for(Points*i=radians+radians_size-angle,*k=radians+radians_size-angle+20;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                cache.width-=4*density/5;
                Cache.active_noise_trash.append(cache);
            }
            angle=settings["active_noise_trash"]["azimuth"].toUInt()+200;
            for(Points*i=radians+radians_size-angle,*k=radians+radians_size-angle+20;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                cache.width-=4*density/5;
                Cache.active_noise_trash.append(cache);
            }
            angle=settings["active_noise_trash"]["azimuth"].toUInt()+300;
            for(Points*i=radians+radians_size-angle,*k=radians+radians_size-angle+20;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                cache.width-=4*density/5;
                Cache.active_noise_trash.append(cache);
            }
            break;
        case 2:
            for(Points*i=radians+settings["active_noise_trash"]["azimuth"].toUInt()+260,*k=radians+settings["active_noise_trash"]["azimuth"].toUInt()+280;i<k;i++)
            {
                cache.Coordinates=new Points[1];
                cache.Coordinates->angle=i->angle;
                cache.Coordinates->x=i->x;
                cache.Coordinates->y=i->y;
                cache.width=GetRandomCoord(4)*density;
                Cache.active_noise_trash.append(cache);
            }
            break;
    }
}
