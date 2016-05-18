#include "regAdmin.h"
#include "ui_regAdmin.h"
#include <QMessageBox>
#include <QSqlQuery>
#include "login.h"

regAdmin::regAdmin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regAdmin)
{
    init();
    ui->setupUi(this);
}

regAdmin::~regAdmin()
{
    delete ui;
}

void regAdmin::on_btnOk_clicked()
{
    QString adminid, adminpwd, adminname ,adminpwd_check;
    QSqlQuery query;
    QString senddata;
    QString regcode = "10";
    QString delta = "|";
    QString cmd_type;
    QString tip = "两次密码输入不一致";
    QString name_base64;

    adminname = ui->adminName->text();
    adminid = ui->adminID->text();
    adminpwd = ui->adminPwd->text();
    adminpwd_check = ui->adminPwd_2->text();

    if (adminpwd_check != adminpwd) {
        QMessageBox::information(this, "info", tip);
        return;
    }

    QByteArray  tmp_in (adminname.toUtf8());
    name_base64 = QString(tmp_in.toBase64());
    senddata += name_base64;
    senddata += delta;
    senddata += adminid;
    senddata += delta;
    senddata += adminpwd;

    sendcmd(senddata, regcode);
    return;
}

void regAdmin::on_btnQuit_clicked()
{
    this->close();
}

void regAdmin::init()
{

   tcpSocket = new QTcpSocket(this);

   new_tcp_connect();


   connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

   connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),

           this,SLOT(displayError(QAbstractSocket::SocketError)));

}

void regAdmin::revData()
{
    QString err = "用户名或者密码错误";
    QString err_unreg = "该用户未注册";
    QString datas = tcpSocket->readAll();

    QString head = datas.mid(0, 2);
    QString tail = datas.right(2);
    QString type = datas.mid(2, 1);
    QString result = datas.mid(3, 2);
    QString use_judge = datas.mid(4, 1);

    qDebug() << "Not system packet";
    if(head != "EA" || tail != "EB"){
        qDebug() << "Not system packet";

        return ;
    }

    if (type == "1") {
        if (result == "01") {
            QMessageBox::information(this, "info", "注册成功");
            login *lo1 = new login;
            this->close();
            lo1->show();
        } else if (result == "00") {
            QMessageBox::information(this, "info", "注册失败");
        } else {
            QMessageBox::information(this, "info", datas);
        }
    }
}

void regAdmin::new_tcp_connect()
{

   tcpSocket->abort();

   tcpSocket->connectToHost("192.168.0.108", 1234);

}

void regAdmin::displayError(QAbstractSocket::SocketError)

{
    QString err = "服务器连接失败";
    qDebug()<<tcpSocket->errorString();
    QMessageBox::information(this, "show", err);
    tcpSocket->close();

}

int regAdmin::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);
    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}
