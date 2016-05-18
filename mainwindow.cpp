#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::Title(QWidget *parent) : QWidget(parent)
{
    this->setFixedHeight(30);
   // icon_label = new Qlabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}
