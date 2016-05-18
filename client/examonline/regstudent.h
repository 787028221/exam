#ifndef REGSTUDENT_H
#define REGSTUDENT_H

#include <QWidget>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>

namespace Ui {
    class regStudent;
}

class regStudent : public QWidget
{
    Q_OBJECT

public:
    explicit regStudent(QWidget *parent = 0);
    ~regStudent();

private:
    Ui::regStudent *ui;
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

#endif // REGSTUDENT_H
