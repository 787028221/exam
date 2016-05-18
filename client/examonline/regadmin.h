#ifndef REGADMIN_H
#define REGADMIN_H

#include<QtNetwork>
#include<QtNetwork/QTcpSocket>
#include <QWidget>
#include "login.h"

namespace Ui {
    class regAdmin;
}

class regAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit regAdmin(QWidget *parent = 0);
    ~regAdmin();

private:
    Ui::regAdmin *ui;
    QTcpSocket *tcpSocket;

private:
    void new_tcp_connect();
    void init();
private slots:
    void on_btnQuit_clicked();
    void on_btnOk_clicked();
    void revData();
    void displayError(QAbstractSocket::SocketError);
    int sendcmd(QString data, QString type);

};


#endif // REGADMIN_H
