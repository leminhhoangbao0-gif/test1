#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QGraphicsTextItem>
#include <QElapsedTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    scene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,400,600);

    showTable();
    drawMemory();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshBlockTable()
{
    ui->tableWidget->clear();

    ui->tableWidget->setRowCount(userBlocks.size());
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"Base","Size","Free"});

    for (int i = 0; i < userBlocks.size(); ++i) {
        ui->tableWidget->setItem(i,0,
                                 new QTableWidgetItem(QString::number(userBlocks[i].base)));

        ui->tableWidget->setItem(i,1,
                                 new QTableWidgetItem(QString::number(userBlocks[i].size)));

        ui->tableWidget->setItem(i,2,
                                 new QTableWidgetItem(userBlocks[i].free ? "Yes" : "No"));
    }
}
void MainWindow::on_btnAddBlock_clicked()
{
    block b;
    b.base = ui->editBase->text().toInt();
    b.size = ui->editSize->text().toInt();
    b.free = true;

    userBlocks.push_back(b);

    refreshBlockTable();
}
void MainWindow::on_btnSaveBlocks_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"Save Blocks","","CSV Files (*.csv)");
    if(path.isEmpty()) return;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out << "Base,Size\n";

    for(auto &b : userBlocks)
        out << b.base << "," << b.size << "\n";
}
void MainWindow::on_btnLoadBlocks_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Load Blocks","","CSV Files (*.csv)");
    if(path.isEmpty()) return;

    QFile file(path);
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    in.readLine();

    userBlocks.clear();

    while(!in.atEnd()){
        auto parts = in.readLine().split(",");
        if(parts.size()<2) continue;

        block b;
        b.base = parts[0].toInt();
        b.size = parts[1].toInt();
        b.free = true;

        userBlocks.push_back(b);
    }

    manager.setmemory({userBlocks.begin(), userBlocks.end()});
    showTable();
    drawMemory();
}

void MainWindow::on_btnLoad_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Open CSV", "", "CSV Files (*.csv)");

    if (path.isEmpty()) return;

    processes.clear();

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream in(&file);
    in.readLine();

    while (!in.atEnd()) {
        auto parts = in.readLine().split(",");
        if (parts.size() < 2) continue;

        process p;
        p.pid = parts[0].toStdString();
        p.size = parts[1].toInt();
        processes.push_back(p);
    }

    QMessageBox::information(this, "Loaded", "Loaded " + QString::number(processes.size()) + " processes");
}

void MainWindow::on_btnRun_clicked(){
    if (processes.isEmpty()) return;

    manager.reset();

    Algorithmn* algo = nullptr;

    if (ui->comboAlgo->currentIndex() == 0)
        algo = new firstfit();
    else if (ui->comboAlgo->currentIndex() == 1)
        algo = new bestfit();
    else
        algo = new worstfit();

    for (auto& p : processes)
        manager.allocate(p, *algo);

    delete algo;

    showTable();
    drawMemory();
    qDebug() << "Run clicked";
    qDebug() << "Processes:" << processes.size();
    qDebug() << "Memory blocks:" << manager.memory().size();

}
void MainWindow::showTable()
{
    auto mem = manager.memory();

    ui->tableWidget->setRowCount(mem.size());
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"Base","Size","Free"});

    for (int i = 0; i < mem.size(); ++i) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(mem[i].base)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(mem[i].size)));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(mem[i].free?"Yes":"No"));
    }
}
void MainWindow::drawMemory()
{
    scene->clear();

    auto mem = manager.memory();
    if (mem.empty()) {
        qDebug() << "No memory to draw";
        return;
    }

    int y = 0;
    for (auto& b : mem) {
        int h = b.size / 2;
        if (h < 15) h = 15;

        scene->addRect(0, y, 150, h);

        auto *text = scene->addText(QString::number(b.size));
        text->setPos(160, y);

        y += h + 8;
    }

    scene->update();
}

void MainWindow::on_btnExport_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"Save CSV","","CSV Files (*.csv)");
    if (path.isEmpty()) return;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out << "Base,Size,Free\n";

    for (auto& b : manager.memory()) {
        out << b.base << "," << b.size << "," << b.free << "\n";
    }
}
void MainWindow::on_btnStress_clicked()
{
    QVector<process> big;

    for (int i = 0; i < 10000; ++i) {
        process p;
        p.pid = "P" + std::to_string(i);
        p.size = rand()%200 + 1;
        big.push_back(p);
    }

    firstfit algo;

    QElapsedTimer timer;
    timer.start();

    manager.reset();
    for (auto& p : big)
        manager.allocate(p, algo);

    qint64 ms = timer.elapsed();

    ui->label_time->setText("Time: " + QString::number(ms) + " ms");
}



