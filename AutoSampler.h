#ifndef AUTOSAMPLER_H
#define AUTOSAMPLER_H

#include <QMainWindow>
#include "glwidget.h"
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QPixmap>
namespace Ui {
class AutoSampler;
}

class AutoSampler : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoSampler(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);

    ~AutoSampler();

    QGridLayout *glayout;
    GLWidget glWidget;
    QWidget *widget;

signals:
    void sigKey(QString key);

public slots:
    void slotInputDisplayLed(uint portData);
    void slotOutputDisplayLed(uint portData);
    void slotFullScreen();
    void slotQuitFullScreen();

private:
    Ui::AutoSampler *ui;
};

#endif // AUTOSAMPLER_H
