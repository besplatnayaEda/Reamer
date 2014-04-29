#ifndef REAMERABS_H
#define REAMERABS_H

#include <QWidget>
#include <QtGui>
#include <QPointer>
#include <QObject>
#include <QDialog>


namespace Ui { class ReamerABS;}

class QTimer;
class QextSerialPort;
class QextSerialEnumerator;

class ReamerABS : public QWidget
{
    Q_OBJECT

public:
    explicit ReamerABS(QWidget *parent = 0);
    ~ReamerABS();

private:
    Ui::ReamerABS *ui;
    QTimer *timer;
    QextSerialPort *port;
    QextSerialEnumerator *enumerator;

private slots:
    void onReadyRead();

protected:
    void keyPressEvent(QKeyEvent* ke);

    quint8 fps,b1,b2,b3,b4,b5,b6,b7,b8,b9,reg1[7],reg2[7],reg3[7];

    QString pname,pread;
};
#endif // REAMERABS_H
