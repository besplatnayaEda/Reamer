#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "reamerabs.h"
#include "ui_reamerabs.h"
#include <QDebug>
#include <QtCore>

ReamerABS::ReamerABS(QWidget *parent) : QWidget(parent),ui(new Ui::ReamerABS)
{
    fps=static_cast<quint8>(0);//
//    fr=static_cast<quint8>(1);

    port->setBaudRate();

    ui->setupUi(this);
    ui->RenderReamer->SetSettings("system","clws",true);
    ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(360.0f));
    ui->RenderReamer->SetSettings("system","range",static_cast<quint16>(1));
    ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(8));
    ui->RenderReamer->SetSettings("system","azimuth",static_cast<quint16>(0));
    ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(0)/100);
    ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(0)/100);
    ui->RenderReamer->SetSettings("system","focus",static_cast<qreal>(0)/100);
    ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
    ui->RenderReamer->ChangeFPS(fps>0 ? 1000/fps : 0);
    ui->RenderReamer->SetSettings("local_items","show",true);
    ui->RenderReamer->SetSettings("trash","show",false);
    ui->RenderReamer->SetSettings("trash","intensity",static_cast<quint8>(50));
    ui->RenderReamer->SetSettings("trash","begin",0.f);
}

ReamerABS::~ReamerABS()
{
    delete ui;
    delete port;
}

void ReamerABS::keyPressEvent(QKeyEvent *ke)
{
    switch (ke->key())
    {
        case Qt::Key_Escape:
            QApplication::exit();
            break;
        case Qt::Key_S:
            {
            if(ui->RenderReamer->IsActive())
            {
                fps=0;
                ui->RenderReamer->ChangeFPS(fps>0 ? 1000/fps : 0);
                ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(0)/100);
                ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(0)/100);
                ui->RenderReamer->SetSettings("system","focus",static_cast<qreal>(0)/100);
                ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
            }
            else
            {
                fps=static_cast<quint8>(60);
                ui->RenderReamer->ChangeFPS(fps>0 ? 1000/fps : 0);
                ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(100)/100);
                ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(100)/100);
                ui->RenderReamer->SetSettings("system","focus",static_cast<qreal>(100)/100);
                ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
            }
            break;
            }
        case Qt::Key_1:
            ui->RenderReamer->SetSettings("system","azimuth",static_cast<quint16>(0));
            break;
        case Qt::Key_2:
            ui->RenderReamer->SetSettings("system","azimuth",static_cast<quint16>(1));
            break;
        case Qt::Key_3:
            ui->RenderReamer->SetSettings("system","azimuth",static_cast<quint16>(2));
            break;
        case Qt::Key_4:
            ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(90.0f));
            ui->RenderReamer->SetSettings("trash","end",90.f);
            break;
        case Qt::Key_5:
            ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(180.0f));
            ui->RenderReamer->SetSettings("trash","end",180.f);
            break;
        case Qt::Key_6:
            ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(360.0f));
            ui->RenderReamer->SetSettings("trash","end",360.f);
            break;
        case Qt::Key_7:
            ui->RenderReamer->SetSettings("system","range",static_cast<quint16>(0));
            break;
        case Qt::Key_8:
            ui->RenderReamer->SetSettings("system","range",static_cast<quint16>(1));
            break;
        case Qt::Key_9:
            ui->RenderReamer->SetSettings("system","range",static_cast<quint16>(2));
            break;
        case Qt::Key_T:
            ui->RenderReamer->SetSettings("trash","show",true);
            break;
        case Qt::Key_Y:
            ui->RenderReamer->SetSettings("trash","show",false);
            break;
        case Qt::Key_Z:
            ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(8));
//            fr=static_cast<quint8>(1);
            break;
        case Qt::Key_X:
            ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(16));
//            fr=static_cast<quint8>(2);
            break;
        case Qt::Key_C:
            ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(24));
//            fr=static_cast<quint8>(3);
            break;
        case Qt::Key_V:
            ui->RenderReamer->SetSettings("system","clws",true);
            break;
        case Qt::Key_B:
            ui->RenderReamer->SetSettings("system","clws",false);
            break;

    }


}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        break;
    default:
        break;
    }
}
