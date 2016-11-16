#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void setData(int numb, int diskr, QVector<double> *vect);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;    // Объявляем графическое полотно
    QCPGraph *graphic;          // Объявляем график
    int numberOfGraphics;
    QVector<double> *vectors;
    int samplingFrequency;

private slots:
    void slotRangeChanged (const QCPRange &newRange);
};

#endif // MAINWINDOW_H
