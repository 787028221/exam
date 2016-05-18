#ifndef REGTEACHER_H
#define REGTEACHER_H

#include <QWidget>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>

namespace Ui {
    class regTeacher;
}

class regTeacher : public QWidget
{
    Q_OBJECT

public:
    explicit regTeacher(QWidget *parent = 0);
    ~regTeacher();

private:
    Ui::regTeacher *ui;
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

#endif // REGTEACHER_H
