#ifndef REAMERABS_H
#define REAMERABS_H

#include <QWidget>
#include <QtGui>
#include <QPointer>
#include <QObject>
#include <QDialog>
#include "port.h"


namespace Ui { class ReamerABS;}

class QTimer;
//class QextSerialPort;

class ReamerABS : public QWidget
{
    Q_OBJECT

public:
    explicit ReamerABS(QWidget *parent = 0);
    ~ReamerABS();

private:
    void setDefaultSettings();
//    void sortArray();
//    void updateSettings();
    Ui::ReamerABS *ui;
    QTimer *timer;
//    QextSerialPort *port_1, *port_2;


private slots:
//    void onReadyRead();

protected:
    void keyPressEvent(QKeyEvent* ke);
//    QByteArray data_unSort_1,data_unSort_2;
    quint8 fps,i,j, spd,b1,b2,b3,b4,b5,b6, data_port_1[48],data_port_2[48];
//    quint8 STOP,ROT2,ROT4,ROT6,FINE, data_old1[48],data_old2[48];

};
#endif // REAMERABS_H
