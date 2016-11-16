#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setData(int numb, int diskr, QVector<double> *vect) {
    numberOfGraphics = numb;
    samplingFrequency = diskr;
    vectors = vect;
    QVector<double> aX;
    for(int i = 0; i < vect[0].size(); i++) {
        aX.push_back(i * (1.0 / diskr));
    }

    QColor subTickColor(237, 213, 208, 100);
    QColor tickColor(220, 179, 172, 100);
    QPen subTickPen(subTickColor);
    QPen tickPen(tickColor);
    tickPen.setWidth(2);
    ui->setupUi(this);
    customPlot = new QCustomPlot(); // Инициализируем графическое полотно
    ui->gridLayout->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения

    customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    //customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);   // Подпись координат по Оси X в качестве Даты и Времени
    //customPlot->xAxis->setDateTimeFormat("hh:mm");  // Устанавливаем формат даты и времени

    // Настраиваем шрифт по осям координат
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // Автоматическое масштабирование тиков по Оси X
    customPlot->xAxis->setAutoTickStep(true);
    customPlot->yAxis->setAutoTickStep(true);
    customPlot->xAxis->setAutoTickCount(customPlot->width()/(customPlot->physicalDpiX() * 10 / 25.4));
    customPlot->xAxis->setAutoSubTicks(false);
    customPlot->xAxis->setSubTickCount(9);
    customPlot->xAxis->setTickLength(customPlot->height()*2, 5);
    customPlot->xAxis->setSubTickLength(customPlot->height(), 5);
    customPlot->xAxis->setSubTickPen(subTickPen);
    customPlot->xAxis->setTickPen(tickPen);
    //customPlot->xAxis->setAutoTickLabels(false);

    /* Делаем видимыми оси X и Y по верхней и правой границам графика,
     * но отключаем на них тики и подписи координат
     * */
//    customPlot->xAxis2->setVisible(true);
//    customPlot->yAxis2->setVisible(true);
//    customPlot->xAxis2->setTicks(false);
//    customPlot->yAxis2->setTicks(false);
//    customPlot->xAxis2->setTickLabels(false);
//    customPlot->yAxis2->setTickLabels(false);

    customPlot->yAxis->setTickLabelColor(QColor(Qt::red)); // Красный цвет подписей тиков по Оси Y
    customPlot->legend->setVisible(true);   //Включаем Легенду графика
    // Устанавливаем Легенду в левый верхний угол графика
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // Инициализируем график и привязываем его к Осям
    graphic = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(graphic);  // Устанавливаем график на полотно
    graphic->setName("Доход, Р");       // Устанавливаем
    graphic->setPen(QPen(QColor(Qt::red))); // Устанавливаем цвет графика
    //graphic->setAntialiased(false);         // Отключаем сглаживание, по умолчанию включено
    //graphic->setLineStyle(QCPGraph::lsImpulse); // График в виде импульсных тиков

    /* Подключаем сигнал от Оси X об изменении видимого диапазона координат
     * к СЛОТу для переустановки формата времени оси.
     * */
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this, SLOT(slotRangeChanged(QCPRange)));

    // Будем строить график с сегодняшнего дни и текущей секунды в будущее
    double now = QDateTime::currentDateTime().toTime_t();
    // Объявляем вектора времени и доходов
    QVector <double> time(400), income(400);

    srand(15); // Инициализируем генератор псевдослучайных чисел

    // Заполняем график значениями
    for (int i=0; i<400; ++i)
      {
        time[i] = now + 3600*i;
        income[i] = qFabs(income[i]) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
      }

    graphic->setData(aX, vectors[0]); // Устанавливаем данные
    customPlot->rescaleAxes();      // Масштабируем график по данным
    customPlot->replot();           // Отрисовываем график
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotRangeChanged(const QCPRange &newRange)
{
    /* Если область видимости графика меньше одного дня,
     * то отображаем часы и минуты по Оси X,
     * в противном случае отображаем дату "День Месяц Год"
     * */
    //customPlot->xAxis->setDateTimeFormat((newRange.size() <= 86400)? "hh:mm" : "dd MMM yy");
}
