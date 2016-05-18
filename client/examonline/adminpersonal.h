#ifndef ADMINPERSONAL_H
#define ADMINPERSONAL_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>

namespace Ui {
    class adminpersonal;
}

class adminpersonal : public QDialog
{
    Q_OBJECT

public:
    explicit adminpersonal(QWidget *parent = 0);
    ~adminpersonal();

private:
    Ui::adminpersonal *ui;
    QTcpSocket *tcpSocket;

private:
    void new_tcp_connect();
    void init();
    void init_tableWidget();
    int ItemTeacherCnt;
    int ItemStuCnt;
    int ItemExamCnt;
    int sendcmd(QString data, QString type);
//    void btnReTea();

private slots:
    void revData();
    void displayError(QAbstractSocket::SocketError);

    void on_btnsubmit_clicked();
    void on_btnAddTea_clicked();
    void on_btnAddStu_clicked();
    void on_btnReTea_clicked();
    void on_btnReStu_clicked();
    void on_btnDelTea_clicked();
    void on_btnDelStu_clicked();
    void on_btnRefreshStu_clicked();
    void on_btnCancel_clicked();
    void fill_paper_name(int row, int cloume);

};
#endif // ADMINPERSONAL_H
