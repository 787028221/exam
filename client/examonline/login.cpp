#include "login.h"
#include "ui_login.h"
#include "regStudent.h"
#include "regteacher.h"
#include "regadmin.h"
#include "exam.h"
#include "tcpsendmsg.h"
#include "studentpersonal.h"
#include "teacherpersonal.h"
#include "adminpersonal.h"
#include <qmessagebox.h>
#include <QtSql/QSqlQuery>
#include <QtNetwork>
#include "tcpsendmsg.h"
#include "exam.h"

QString userid;
QString global_passwd;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    init();
    ui->setupUi(this);
}

login::~login()
{
    tcpSocket->close();
    delete ui;
}

void login::on_btnReg_clicked()
{
    QString usrn,pwd,type;

    usrn = ui->leName->text();
    pwd = ui->lePwd->text();
    type = ui->cbxType->currentText();

    if(type==tr("管理员")) {
        //regAdmin *ex = new regAdmin;
        //ex->show();
        QMessageBox::information(this, "info", "请联系管理员邀请注册");
        //this->close();
    } else if (type==tr("教师")) {
        regTeacher *regTea = new regTeacher;
        regTea->show();
        this->close();
    } else if (type==tr("学生")){
        regStudent *regstu = new regStudent;
        regstu->show();
        this->close();
    }
}

void login::on_btnOk_clicked()
{

    bool flag = false;
    QString type;
    QSqlQuery query;
    QString senddata;
    QString logincode = "01";
    QString delta = "|";
    QString cmd_type;

    usrn = ui->leName->text();
    pwd = ui->lePwd->text();
    type = ui->cbxType->currentText();
    userid = usrn;
    global_passwd = pwd;
    if(usrn.isEmpty() || pwd.isEmpty()){
        flag = true;
        goto TAR_OUT;
    }
    if(type==tr("管理员")) {
        user_type = ADMIN;
        cmd_type = "00";
    } else if (type==tr("教师")) {
        user_type = TEACHER;
        cmd_type = "01";
    } else {
        user_type = STUDENT;
        cmd_type = "02";
    }

    senddata.append(usrn);
    senddata.append(delta);
    senddata.append(pwd);

    sendcmd(senddata, cmd_type);

TAR_OUT:
    if(flag)
        QMessageBox::warning(this,tr("Warning"),tr("账户和密码不能为空!"));

}

void login::on_btnQuit_clicked()
{
    this->close();

}

void login::init()
{

   tcpSocket = new QTcpSocket(this);

   new_tcp_connect();


   connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

   connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),

           this,SLOT(displayError(QAbstractSocket::SocketError)));

}



void login::revData()
{
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

    if (type == "0") {
        if (result == "21") {
            studentpersonal *stu = new studentpersonal;
            stu->stuid = usrn;
            stu->passwd = pwd;
            stu->show();
            this->close();
        } else if (result == "11") {
            teacherpersonal *teach = new teacherpersonal;
            teach->show();
            this->close();
        } else if (result == "01") {
            adminpersonal *admin = new adminpersonal;
            admin->show();
            this->close();
        }  else if (use_judge == "2") {
            QMessageBox::information(this, "info", "该用户未注册");
            if (user_type == ADMIN) {
//                regAdmin *rega = new regAdmin;
//                rega->show();
//                this->close();
            } else if (user_type == TEACHER) {
                regTeacher *regt = new regTeacher;
                regt->show();
                this->close();
            } else {
                regStudent *regs = new regStudent;
                regs->show();
                this->close();
            }
        } else {
            ui->tiplabel->setText(err);
            qDebug() << "login err!";
        }

    } else if (type == "1") {
        if (result == "11") {
//            QMessageBox::information(this, "info", datas);
        }

    }
    emit sendDifDiaData(usrn);
}

void login::new_tcp_connect()
{

   tcpSocket->abort();

   tcpSocket->connectToHost("192.168.0.108", 1234);
   //tcpSocket->

}



void login::displayError(QAbstractSocket::SocketError)

{
    QString err = "服务器连接失败";
    qDebug()<<tcpSocket->errorString();
    ui->tiplabel->setText(err);
    QMessageBox::information(this, "show", err);
    tcpSocket->close();

}

int login::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);
    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}
