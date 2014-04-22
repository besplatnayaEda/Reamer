#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "reamerabs.h"
#include "ui_reamerabs.h"
#include <QDebug>
#include <QtCore>

ReamerABS::ReamerABS(QWidget *parent) : QWidget(parent),ui(new Ui::ReamerABS)
{

    fps=static_cast<quint8>(0);
//    fr=static_cast<quint8>(1);
/*
    port->setBaudRate(BAUD4800);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_2);
    port->setQueryMode(QextSerialPort::QueryMode(2));
//QextSerialPort(QueryMode mode = EventDriven, QObject *parent = 0);
*/
    foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
        pname=info.portName;

    timer = new QTimer(this);
    timer->setInterval(40);
    //! [1]
    PortSettings settings = {BAUD4800, DATA_8, PAR_NONE, STOP_2, FLOW_OFF, 10};
    port = new QextSerialPort("COM2", settings, QextSerialPort::Polling);
    //! [1]

    enumerator = new QextSerialEnumerator(this);
    enumerator->setUpNotifications();

    port->setPortName("COM2");
    port->open(QIODevice::ReadWrite);
    pread=QString::fromLatin1(port->readAll());
    QTextStream Qcout(stdout);
    Qcout <<QString::fromLatin1(port->readAll());
    printf("deb");
    Qcout << "46456";
    switch (pread.toInt()) {
    case 31:
        ui->RenderReamer->SetSettings("system","clws",true);
        break;
    case 32:
        ui->RenderReamer->SetSettings("system","clws",false);
        break;
    }

//    port->readAll();

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
                fps=static_cast<quint8>(30);
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
        case Qt::Key_R:
//            port->write("test");
        pread=QString::fromLatin1(port->read(1));
        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(pread.toInt()));
        QTextStream Qcout(stdout);
        Qcout <<pread.toInt();
        break;

    }


}

void ReamerABS::changeEvent(QEvent *e)
{
//    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
