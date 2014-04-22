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

protected:
    void keyPressEvent(QKeyEvent* ke);
    void changeEvent(QEvent *e);
    qint8 fps;//,fr;
    QString pname,pread;
};

#endif // REAMERABS_H
