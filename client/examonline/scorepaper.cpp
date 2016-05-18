#include "scorepaper.h"
#include "ui_scorepaper.h"
#include <QMessageBox>
#include <QSqlQuery>

scorepaper::scorepaper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scorepaper)
{
    ui->setupUi(this);
}

scorepaper::~scorepaper()
{
    delete ui;
}

