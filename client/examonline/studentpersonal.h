#ifndef STUDENTPERSONAL_H
#define STUDENTPERSONAL_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>
#include "exam.h"

namespace Ui {
    class studentpersonal;
}

class studentpersonal : public QDialog
{
    Q_OBJECT

public:
    explicit studentpersonal(QWidget *parent = 0);
    ~studentpersonal();
    QString stuid;
    QString passwd;

private:
    Ui::studentpersonal *ui;
    QTcpSocket *tcpSocket;

private:
    void new_tcp_connect();
    void init();
    void init_tableWidget();
    void questionMessage();
    exam *ex;

private slots:
    void revData();
    void displayError(QAbstractSocket::SocketError);
    int sendcmd(QString data, QString type);
//    void recDifDiaData(QString data);
    void on_btnQuit_clicked();
    void on_btnOk_clicked();
    void openexam(int row, int cloume);
};
#endif // STUDENTPERSONAL_H
