#include "studentpersonal.h"
#include "ui_studentpersonal.h"
#include "login.h"
#include "exam.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>
#include <QTableWidget>

extern QString userid;
extern QString global_passwd;
bool isExam;
QString paperid;
QString major;
QString examTime;

studentpersonal::studentpersonal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::studentpersonal)
{

    ex = NULL;
    ui->setupUi(this);
    init();
    init_tableWidget();
    ui->tableWidget_examing->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget_examing->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget_examing->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget_examing,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(openexam(int,int)));
}


studentpersonal::~studentpersonal()
{
    delete ui;
}


void studentpersonal::init()
{
    QString type = "22";

    tcpSocket = new QTcpSocket(this);

    new_tcp_connect();

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

    sendcmd(userid, type);
}

void studentpersonal::init_tableWidget()
{
    QStringList headers;

    headers << tr( "试卷ID" ) << tr( "科目" ) <<  tr( "满分" ) << tr( "考试时间" ) ;
    ui->tableWidget_examing->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_examing->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_examing->setColumnCount(4);
    ui->tableWidget_examing->setHorizontalHeaderLabels(headers);
    ui->tableWidget_examing->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    headers.clear();
    headers << tr( "试卷ID" ) << tr( "科目" ) <<  tr( "成绩" ) << tr( "满分" ) ;
    ui->tableWidget_examed->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_examed->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_examed->setColumnCount(4);
    ui->tableWidget_examed->setHorizontalHeaderLabels(headers);
    ui->tableWidget_examed->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}


void studentpersonal::revData()
{
    QString err = "用户名或者密码错误";
    QString err_unreg = "该用户未注册";
    QString datas = tcpSocket->readAll();
    QString head = datas.mid(0, 2);
    QString tail = datas.right(2);
    QString result = datas.mid(3, 2);
    int list_size;
    int i;

    qDebug() << "Not system packet";
    if(head != "EA" || tail != "EB"){
        qDebug() << "Not system packet";

        return ;
    }

    qDebug() << datas;
    QStringList pktlist = datas.split("EB");

    for (i=0; i<pktlist.size(); i++) {
        QString pkt = pktlist[i];
        QString type = pkt.mid(2, 1);
        QString result = pkt.mid(3, 2);
        QString userdata = pkt.mid(5, -1);
        int pktcnt = pkt.mid(5, 1).toInt();
        if (type == "2") {
            if (result == "21") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                if (list_size < 2) {
                    return;
                }

                QString name = strlist.at(0);
                QString id = strlist.at(1);
                ui->stuid_2->setText(id);
                ui->stuname->setText(name);
                sendcmd(userid, tr("42"));
            } else if (result == "20") {
                QMessageBox::information(this, "info", "个人信息提取失败");
            } else {
                QMessageBox::information(this, "info", datas);
            }
        } else if (type == "3") {
            if (result == "21") {
                QMessageBox::information(this, "info", "修改密码成功");
                ui->leNewPwd->clear();
                ui->leNewPwd_chk->clear();
            } else if (result == "20") {
                QMessageBox::information(this, "info", "修改密码失败");
            } else {
                QMessageBox::information(this, "info", datas);
            }
        } else if (type == "4") {
            if (result == "21") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1, *item2, *item3;

                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;
                item2 = new QTableWidgetItem;
                item3 = new QTableWidgetItem;

                QString paperid = strlist.at(1);
                QString major = strlist.at(2);
                QString fullscore = strlist.at(3);
                QString time = strlist.at(4);

                item0->setText(paperid);
                item1->setText(major);
                item2->setText(fullscore);
                item3->setText(time);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                ui->tableWidget_examing->setRowCount(rowNum);
                ui->tableWidget_examing->setItem(pktcnt, 0, item0);

                ui->tableWidget_examing->setItem(pktcnt, 1, item1);
                ui->tableWidget_examing->setItem(pktcnt, 2, item2);
                ui->tableWidget_examing->setItem(pktcnt, 3, item3);
                qDebug() << pktcnt <<userdata;
            } else if (result == "22") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1, *item2, *item3;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;
                item2 = new QTableWidgetItem;
                item3 = new QTableWidgetItem;

                QString paperid = strlist.at(1);
                QString major = strlist.at(2);
                QString score = strlist.at(3);
                QString fullscore = strlist.at(4);

                item0->setText(paperid);
                item1->setText(major);
                item2->setText(score);
                item3->setText(fullscore);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                ui->tableWidget_examed->setRowCount(rowNum);
                ui->tableWidget_examed->setItem(pktcnt, 0, item0);

                ui->tableWidget_examed->setItem(pktcnt, 1, item1);
                ui->tableWidget_examed->setItem(pktcnt, 2, item2);
                ui->tableWidget_examed->setItem(pktcnt, 3, item3);
                qDebug() << pktcnt <<userdata;
            } else if (result == "03") {

                qDebug() << pktcnt <<userdata;
            }
        }
    }
}

void studentpersonal::on_btnOk_clicked()
{
    bool flag = false;
    QSqlQuery query;
    QString old_pwd;
    QString new_pwd;
    QString new_pwd_chk;
    QString senddata;
    QString logincode = "32";
    QString delta = "|";
    QString cmd_type;

    old_pwd = ui->leOldPwd->text();
    new_pwd = ui->leNewPwd->text();
    new_pwd_chk = ui->leNewPwd_chk->text();
    if(old_pwd.isEmpty() || new_pwd.isEmpty() || new_pwd_chk.isEmpty()){
        flag = true;
        goto TAR_OUT;
    }
    if (new_pwd != new_pwd_chk) {
        QMessageBox::warning(this,tr("Warning"),tr("两次密码输入不一致!"));
        return;
    }
    if (old_pwd != global_passwd) {
        QMessageBox::warning(this,tr("Warning"),tr("原密码输入错误!"));
        return;
    }

    senddata.append(userid);
    senddata.append(delta);
    senddata.append(new_pwd);

    sendcmd(senddata, logincode);

TAR_OUT:
    if(flag) {
        QMessageBox::warning(this,tr("Warning"),tr("密码不能为空!"));
    }

}

void studentpersonal::on_btnQuit_clicked()
{
    this->close();
}

void studentpersonal::new_tcp_connect()
{
   tcpSocket->abort();

   tcpSocket->connectToHost("192.168.0.108", 1234);

}

void studentpersonal::displayError(QAbstractSocket::SocketError)

{
    QString err = "服务器连接失败";
    qDebug()<<tcpSocket->errorString();
    QMessageBox::information(this, "show", err);
    tcpSocket->close();
}

int studentpersonal::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);
    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}

void studentpersonal::openexam(int row, int cloume)
{
    if (!isExam) {
        QString senddata;
        QString cmd = "67";
        QString delta = "|";

        paperid = ui->tableWidget_examing->item(row, 0)->text();
        major = ui->tableWidget_examing->item(row, 1)->text();
        examTime = ui->tableWidget_examing->item(row, 3)->text();

        senddata = paperid + delta + userid;
        ex = new exam;
        ex->show();
        ex->init(paperid);
        this->close();
        sendcmd(senddata, cmd);
        qDebug() << paperid << major << examTime;
    }
}

