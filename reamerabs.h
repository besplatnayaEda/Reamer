#ifndef REAMERABS_H
#define REAMERABS_H

#include <QWidget>
#include <QtGui>


namespace Ui
{
class ReamerABS;
}

class ReamerABS : public QWidget
{
    Q_OBJECT

public:
    explicit ReamerABS(QWidget *parent = 0);
    ~ReamerABS();

private:
    Ui::ReamerABS *ui;

protected:
    void keyPressEvent(QKeyEvent* ke);
    qint8 fps;
};

#endif // REAMERABS_H
