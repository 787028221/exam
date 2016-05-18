#include "exam.h"
#include "ui_exam.h"
#include <qmessagebox.h>
#include <QtSql/QSqlQuery>
#include <QtNetwork>
#include <QMessageBox>
#include <QMap>

extern bool isExam;
extern QString paperid;
extern QString major;
extern QString examTime;
extern QString userid;
question *nq;

//QListIterator<question> iterator(list);


exam::exam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exam)
{
    ui->setupUi(this);
    isExam = true;
    qDebug() << "open exam layout";
    setAttribute(Qt::WA_DeleteOnClose);
    curQuestion = 0;
    getScore = 0;
}

exam::~exam()
{
    delete ui;
    isExam = false;
    qDebug() << "Close exam layout";
}

void exam::init(QString pId)
{
    ui->leExamName->setText(major);
    ui->leExamTime->setText(examTime);
    ui->leLeftTime->setText(examTime);
    questionMessage();
    tcpInit(pId);
}

void exam::deinit()
{
    this->close();
}

void exam::questionMessage()
{
    QString msg0 = "您即将进行关于";
    QString msg1 = "的考试，考试时间为";
    QString msg2 = "分钟";
    QString msg;

    msg += msg0;
    msg += major;
    msg += msg1;
    msg += examTime;
    msg += msg2;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                    msg,
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        startRecord();
        //getPaper();
        qDebug() << "YES";
    } else if (reply == QMessageBox::No) {
        qDebug()<< "NO";
        this->close();
    }

}

void exam::startRecord()
{
    this->recordTimer = new QTimer;
    connect(recordTimer,SIGNAL(timeout()),this,SLOT(recordTiming()));
    int time = examTime.toInt();
    hour = time / 60;
    minute = time % 60;
    second = 0;
    recordTimer->start(1000);
}

void exam::recordTiming()
{
    if (--second <= 0) {
        second = 59;
        if (--minute <= 0) {
            minute = 59;
            hour--;
        }
    }

    QString str,tmp;
    tmp.setNum(hour);
    if(hour <10) {
        str+='0';
    }
    str += tmp;
    str += ':';

    tmp.setNum(minute);
    if(minute<10)
    {
        str+='0';
    }
    str += tmp;
    str+=':';

    tmp.setNum(second);
    if(second<10)
    {
        str+='0';
    }
    str += tmp;
    ui->leLeftTime->show();
    ui->leLeftTime->setText(str);
}

void exam::tcpInit(QString pId)
{
    QString senddata;
    QString code = "62";

    tcpSocket = new QTcpSocket(this);

    new_tcp_connect();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

    senddata = userid + tr("|") + pId;
    qDebug() << senddata;
    sendcmd(senddata, code);
}

void exam::revData()
{
    int i;
    QString err = "用户名或者密码错误";
    QString err_unreg = "该用户未注册";
    QString datas = tcpSocket->readAll();

    QString head = datas.mid(0, 2);
    QString tail = datas.right(2);
    QString type = datas.mid(2, 1);
    QString result = datas.mid(3, 2);
    QString use_judge = datas.mid(4, 1);

    if(head != "EA" || tail != "EB"){
        qDebug() << "Not system packet";

        return ;
    }
    QStringList pktlist = datas.split("EB");

    for (i=0; i<pktlist.size(); i++) {
        QString pkt = pktlist[i];
        QString type = pkt.mid(2, 1);
        QString result = pkt.mid(3, 2);
        QString userdata = pkt.mid(5, -1);
        int pktcnt = pkt.mid(5, 1).toInt();
        if (type == "6") {
            if (result == "21") {
                QStringList strlist = userdata.split("|");

                question *q = new question;
                q->type = strlist.at(1);
                q->question = strlist.at(2);
                q->answer = strlist.at(3);
                q->questionId  = i;
                q->score = 20; /*TEAM*/
                q->isAns = false;
                questionmap.insert(pktcnt, q);
                qDebug() << pktcnt << q->type << q->question <<  q->answer << q->questionId;
            } else if (result == "22") {


            } else if (result == "23") {
                qDebug() << "startRecord()" << curQuestion;
                QMap<int, question *>::iterator it = questionmap.find(curQuestion);

                qDebug() << it.key();
                if (it != questionmap.end()) {
                    nq = it.value();
                    qDebug() << nq->question;
                    ui->teQuestion->setText(nq->question);
                }
            } else if (result == "20") {
                QMessageBox::information(this, "info", "个人信息提取失败");
            }
        } else if (type == "3") {

        } else if (type == "4") {

        }
    }

}

void exam::new_tcp_connect()
{
   tcpSocket->abort();
   tcpSocket->connectToHost("192.168.0.108", 1234);
}

int exam::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);
    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}

void exam::on_btnNext_clicked()
{

    QMap<int, question *>::iterator it = questionmap.find(curQuestion);

    qDebug() << "map size: " <<questionmap.size();
    if (it != questionmap.end()) {
        curQuestion++;
        nq = it.value();
        ui->teQuestion->setText(nq->question);
        ui->leQuestionID->clear();
        ui->leChkAns->clear();
        qDebug() << "Question id: " << curQuestion;
        ui->leQuestionID->setText(QString::number(curQuestion + 1, 10));
    }


}


void exam::on_btnPrevious_clicked()
{
    QMap<int, question *>::iterator it = questionmap.find(curQuestion);

    if (it == questionmap.end()) {
        nq = it.value();
        ui->teQuestion->setText(nq->question);
        ui->leQuestionID->clear();
        ui->leChkAns->clear();
        qDebug() << "Question id: " << curQuestion;
        ui->leQuestionID->setText(QString::number(curQuestion, 10));
    }
    if (it != questionmap.end()) {
        nq = it.value();
        ui->teQuestion->setText(nq->question);
        ui->leQuestionID->clear();
        ui->leChkAns->clear();
        qDebug() << "Question id: " << curQuestion;
        ui->leQuestionID->setText(QString::number(curQuestion + 1, 10));
        curQuestion--;
        if (curQuestion <= 0) {
            curQuestion = 0;
            return;
        }
    }
}

void exam::on_btnSubmit_clicked()
{
    QString msg0 = "您是否要交卷？现在离考试结束还有";
    QString msg1 = "分钟";
    QString msg;

    msg += msg0;
    msg += ui->leLeftTime->text();
    msg += msg1;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                    msg,
                                    QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        //getPaper();
        qDebug() << "YES";
    } else if (reply == QMessageBox::No) {
        qDebug()<< "NO";
        //this->close();
    }
}


void exam::on_btnchkAn_clicked()
{
    QString senddata;
    QString myAns;
    QString cmd = "72";
    QString delta = "|";

    if (!nq->isAns) {
        myAns = ui->leMyAns->text();
        QByteArray  tmp_in (nq->question.toUtf8());
        QString ques_base64 = QString(tmp_in.toBase64());
//        QString ans_base64 =
        QByteArray  tmp_in2 (major.toUtf8());
        QString major_base64 = QString(tmp_in2.toBase64());

        if (myAns == nq->answer) {
            senddata = paperid + delta + major_base64 + delta + \
                    userid + delta + ques_base64 + delta + \
                    nq->answer + delta + myAns;
            getScore += nq->score;
            sendcmd(senddata, cmd);
        }
        nq->isAns = true;
    }
    ui->leChkAns->setText(nq->answer);

//    sendcmd();
}

void exam::getPaper()
{
    QString senddata;
    QString delta = "|";
    QString cmd = "62";

    senddata = userid + delta + paperid;
    //sendcmd(senddata, cmd);
}
