#ifndef PORT_H
#define PORT_H

#include <QWidget>


namespace Ui { class Port;}

class QTimer;
class QextSerialPort;


class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(QObject *parent = 0);
    ~Port();
    QextSerialPort *port_1, *port_2;
private:
    void sortArray();
    void updateSettings();
    QTimer *timer;


private slots:
    void onReadyRead();


protected:
    QByteArray data_unSort_1,data_unSort_2;
    quint8 fps,i,j, spd,b1,b2,b3,b4,b5,b6, data_port_1[48],data_port_2[48];
    quint8 STOP,ROT2,ROT4,ROT6,FINE, data_old1[48],data_old2[48];


};

#endif // PORT_H
