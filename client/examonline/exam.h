#ifndef EXAM_H
#define EXAM_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>
#include <QMap>
namespace Ui {
    class exam;
}

class question {
public:
    QString question;
    QString type;
    QString answer;
    int questionId;
    int score;
    bool isAns;
};

class exam : public QDialog
{
    Q_OBJECT

public:
    explicit exam(QWidget *parent = 0);
    ~exam();
    void init(QString pI);



private slots:
    void recordTiming();
    void on_btnNext_clicked();
    void on_btnchkAn_clicked();
    void on_btnPrevious_clicked();
    void on_btnSubmit_clicked();
    void revData();

private:
    int sendcmd(QString data, QString type);
    QTcpSocket *tcpSocket;
    int user_type;
    int flag;
    int hour;
    int minute;
    int second;
    void questionMessage();
    void deinit();
    QTimer *recordTimer;
    void startRecord();
    void tcpInit(QString pId);
    void new_tcp_connect();
    int getScore;
    void getPaper();
    QMap<int, question*> questionmap;
private:
    Ui::exam *ui;
    int isopen;
    QString paperid;
    int curQuestion;

};



#endif // EXAM_H
