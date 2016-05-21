#ifndef TEACHERPERSONAL_H
#define TEACHERPERSONAL_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>
#include<QMap>

namespace Ui {
    class teacherpersonal;
}

class teacherpersonal : public QDialog
{
    Q_OBJECT

public:
    explicit teacherpersonal(QWidget *parent = 0);
    ~teacherpersonal();

private:
    Ui::teacherpersonal *ui;
    QTcpSocket *tcpSocket;
private:
    enum {
        SINGLE_CHOICE = 0,
        MULTI_CHOICE,
        BLANK_FILL,
        JUDGE_QUES,
    };
    void new_tcp_connect();
    void init();
    int sendcmd(QString data, QString type);
    QMap <QString, QString> majormap;
    bool isJudge;
    int questionType;
    int dbQuestionNum;
    int examLine;
    void set_show_local(QString questionId, QString quesType, QString question, QString answer);
    int questionNum;
private slots:
    void revData();
    void displayError(QAbstractSocket::SocketError);
    void on_btnQuit_clicked();
    void on_btnOk_clicked();
    void openexam(int row, int cloume);
    void on_btnAdd_clicked();
    void choice(int index);
    void on_chbA_clicked();
    void on_chbB_clicked();
    void majorShow(int index);
    void on_btnGenrate_clicked();
    void on_btnDel_clicked();
    void on_btnSendPaper_clicked();
};


#endif // TEACHERPERSONAL_H
