#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>
#include "regAdmin.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();


private slots:
    void on_btnQuit_clicked();
    void on_btnOk_clicked();
    void on_btnReg_clicked();
    void revData(); //接收来自服务端的数据
    void displayError(QAbstractSocket::SocketError);
private:
    int sendcmd(QString data, QString type);
    enum {
        ADMIN = 0,
        TEACHER = 1,
        STUDENT = 2,
    };
    QString usrn, pwd;
    QTcpSocket *tcpSocket;
    int user_type;

signals:
    void  sendDifDiaData(QString);
//    regAdmin ＊regadmin;
protected:

    void init();
    void new_tcp_connect();

    qint32 usertype;
private:
    Ui::login *ui;

};

#endif // LOGIN_H
