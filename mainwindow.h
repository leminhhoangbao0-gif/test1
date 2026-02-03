#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QMainWindow>
#include <QGraphicsScene>
#include <Qvector>

#include "memorymanager.h"
#include "firstfit.h"
#include "worstfit.h"
#include "bestfit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btnLoad_clicked();
    void on_btnRun_clicked();
    void on_btnStress_clicked();
    void on_btnExport_clicked();
private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    QVector<process> processes;
    memorymanager manager;

    void showTable();
    void drawMemory();
};
#endif // MAINWINDOW_H
