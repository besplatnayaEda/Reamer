#include "port.h"
#include "reamerabs.h"
#include "ui_reamerabs.h"
#include <QDebug>
#include <QtCore>
#include <QThread>

Port::Port(QObject *parent) : QObject(parent)
{
    PortSettings settings = {BAUD19200, DATA_8, PAR_NONE, STOP_2, FLOW_OFF, 10};
    port_1 = new QextSerialPort("COM4", settings, QextSerialPort::EventDriven);
    port_1->open(QIODevice::ReadWrite);
    port_2 = new QextSerialPort("COM5", settings, QextSerialPort::EventDriven);
    port_2->open(QIODevice::ReadWrite);
    connect(port_1, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(port_2, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

}


Port::~Port()
{
    qDebug("By in Thread!");
}

void Port::sortArray()
{
    quint8 j, m, k;

    for ( j = 0; j < 52; j++ )
    {
        if( ( data_unSort_1[0] == 254 && data_unSort_1[1] == 254 ) != 1 )
        {
            //
            if ( j-1 < 0 )
                k = 51;
            else
                k = j-1;
            //
            if ( ( data_unSort_1[k] == 254 && data_unSort_1[j] == 254 ) == 1 )
            {
                if ( j+1 == 52)
                    m = 0;
                else
                    m = j+1;
            }
            //
        }
        else
            m = 2;
    }
    b5 = m;
    //
    for ( j = 0; j < 48; j++ )
    {
        if ( j+m < 52)
            data_old1[j] = data_unSort_1[j+m];
        else
            data_old1[j] = data_unSort_1[j+m-52];
    }
    //
    //
    for ( j = 0; j < 52; j++ )
    {
        if( ( data_unSort_2[0] == 254 && data_unSort_2[1] == 254 ) != 1)
        {
            //
            if ( j-1 < 0 )
                k = 51;
            else
                k = j-1;
            //
            if ( ( data_unSort_2[k] == 254 && data_unSort_2[j] == 254 ) == 1 )
            {
                if ( j+1 == 52)
                    m = 0;
                else
                    m = j+1;
            }
            //
        }
        else
            m = 2;
    }
    b6 = m;
    //
    for ( j = 0; j < 48; j++ )
    {
        if ( j+m < 52)
            data_old2[j] = data_unSort_2[j+m];
        else
            data_old2[j] = data_unSort_2[j+m-52];
    }

}
/*
void Port::updateSettings()
{
    if(STOP==1) //stop
    {
        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(0));
        ui->RenderReamer->SetSettings("system","clws",true);
    }
    if(ROT2==1) //2
    {
        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(8));
        ui->RenderReamer->SetSettings("system","clws",true);
    }
    if(ROT4==1) //4
    {
        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(16));
        ui->RenderReamer->SetSettings("system","clws",true);
    }
    if(ROT6==1) //6
    {
        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(24));
        ui->RenderReamer->SetSettings("system","clws",true);
    }
    if(FINE==1) //plavn
    {
        if((spd-67)>0)
            ui->RenderReamer->SetSettings("system","clws",true);
        else
            ui->RenderReamer->SetSettings("system","clws",false);

        ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(abs(ceil(spd-67))));

    }*/
/*
    if(data_port_1[10]==1)    //vkl
    {
        fps=0;
        ui->RenderReamer->ChangeFPS(fps>0 ? 1000/fps : 0);
        ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(data_port_1[41])/100);
        ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(50)/100);
        ui->RenderReamer->SetSettings("system","focus",static_cast<qreal>(0)/100);
        ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
        ui->RenderReamer->GenerationMeteo();
        ui->RenderReamer->GenerationTrash();
    }
    else


    if(data_port_1[6]==1)   // vikl
    {
        fps=static_cast<quint8>(30);
        ui->RenderReamer->ChangeFPS(fps>0 ? 1000/fps : 0);
        ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(data_port_1[41])/100);
        ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(50)/100);
        ui->RenderReamer->SetSettings("system","focus",static_cast<qreal>(0)/100);
        ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
        ui->RenderReamer->GenerationMeteo();
        ui->RenderReamer->GenerationTrash();
    }

*/

//    if(data_port_1[41]>0) //yark
//        ui->RenderReamer->SetSettings("system","brightness",static_cast<qreal>(data_port_1[41])/100);
//    if(data_port_1[42]>0) //usil
//        ui->RenderReamer->SetSettings("system","lightning",static_cast<qreal>(data_port_1[42])/100);

/*
    if(data_port_1[24]==1 && data_port_1[25]==0)    //90
    {
        ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(90.0f));
        ui->RenderReamer->SetSettings("trash","end",90.f);
        ui->RenderReamer->GenerationMeteo();
        ui->RenderReamer->GenerationTrash();

    }
    if(data_port_1[24]==0 && data_port_1[25]==0)    //180
    {
        ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(180.0f));
        ui->RenderReamer->SetSettings("trash","end",180.f);
        ui->RenderReamer->GenerationMeteo();
        ui->RenderReamer->GenerationTrash();

    }
    if(data_port_1[24]==0 && data_port_1[25]==1)    //360
    {
        ui->RenderReamer->SetSettings("system","scale",static_cast<quint16>(360.0f));
        ui->RenderReamer->SetSettings("trash","end",360.f);
        ui->RenderReamer->GenerationMeteo();
        ui->RenderReamer->GenerationTrash();

    }

*/
/*
    if(data_port_1[44]>0)   //usil
        ui->RenderReamer->SetSettings("trash","intensity",static_cast<quint8>(data_port_1[44]));
    else
        ui->RenderReamer->SetSettings("trash","intensity",static_cast<quint8>(0));

    if(data_port_1[47]>0)   //varu
        ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(data_port_1[47])/100);
    else
        ui->RenderReamer->SetSettings("system","varu",static_cast<qreal>(1)/100);
*/
//    return;
//}


void Port::onReadyRead()
{
    QTextStream Qcout(stdout);

    int numBytes_p1, numBytes_p2;

    numBytes_p1=port_1->bytesAvailable();
    if(numBytes_p1 > 52)
        numBytes_p1 = 52;

    data_unSort_1=port_1->read(numBytes_p1);

    numBytes_p2=port_2->bytesAvailable();
    if(numBytes_p2 > 52)
        numBytes_p2 = 52;

    data_unSort_2=port_2->read(numBytes_p2);

    Qcout << "\n=========================\n" << numBytes_p1 << "|| dat1:";
    for(i=0;i<numBytes_p1;i++)
    {
        b1=data_unSort_1[i];
        Qcout << b1 << " ";
    }
    Qcout  << "\n"  << numBytes_p2 << "|| dat2:";
    for(i=0;i<numBytes_p2;i++)
    {
        b2=data_unSort_2[i];
        Qcout << b2 << " ";
    }
    Qcout << "\n=========================";

    if ( ( (numBytes_p1 == 52) || (numBytes_p2 == 52) ) == 1 )
        sortArray();

    Qcout << "\n_________________________" << "\n\n sort1:";
    for ( i = 0; i < 48; i++ )
    {
        b3=data_old1[i];
        Qcout << b3 << " ";
    }
    Qcout << "|" << b5 << "\n sort2:";
    for ( i = 0; i < 48; i++ )
    {
        b4=data_old2[i];
        Qcout << b4 << " ";
    }
    Qcout << "|" << b6 <<"\n_________________________";

//    ui->RenderReamer->SetSettings("system","freq",static_cast<quint8>(0));
/*
    for( i = 0; i < 48; i++)
    {
        if ( !(data_old1[i] != data_port_1[i]) )
        {
            for ( j = 0; j < 48; j++)
            {
            data_port_1[j] = data_old1[j];
            }
            STOP=data_port_1[0];
            ROT2=data_port_1[1];
            ROT4=data_port_1[2];
            ROT6=data_port_1[3];
            FINE=data_port_1[4];
            spd=data_port_1[40];
//            updateSettings();
            i = 48;
            break;
        }

    }
*/

//    updateSettings();

}
