#include "adminpersonal.h"
#include "ui_adminpersonal.h"
#include <QTableWidget>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>
#include "regteacher.h"
#include "regstudent.h"

extern QString userid;
extern QString global_passwd;

adminpersonal::adminpersonal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminpersonal)
{
    ui->setupUi(this);
    init();
    init_tableWidget();
    ItemTeacherCnt = 0;
    ItemStuCnt = 0;
    ItemExamCnt = 0;
}


adminpersonal::~adminpersonal()
{
    delete ui;
}


void adminpersonal::init_tableWidget()
{
    ui->tabWidget_teacher->setColumnCount(3);
    QStringList headers;
    headers << tr( "姓名" ) << tr( "教师编号" ) <<  tr( "是否选择" );
    ui->tabWidget_teacher->setHorizontalHeaderLabels(headers);
    ui->tabWidget_teacher->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    headers.clear();
    headers << tr( "姓名" ) << tr( "学号" ) <<  tr( "是否选择" );
    ui->tabWidget_stu->setColumnCount(3);
    ui->tabWidget_stu->setHorizontalHeaderLabels(headers);
    ui->tabWidget_stu->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    headers.clear();
    headers << tr( "试卷ID" ) << tr( "专业" )  << tr( "选项" ) ;
    ui->tableWidget_exam->setColumnCount(3);
    ui->tableWidget_exam->setHorizontalHeaderLabels(headers);
    ui->tableWidget_exam->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}


void adminpersonal::init()
{
    QString type = "40";

    tcpSocket = new QTcpSocket(this);

    new_tcp_connect();

    ui->tabWidget_teacher->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_exam->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableWidget_exam->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget_exam->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));
    connect(ui->tableWidget_exam, SIGNAL(cellClicked(int, int)), this, SLOT(fill_paper_name(int, int)));

    sendcmd(userid, type);
}

void adminpersonal::revData()
{
    QString err = "用户名或者密码错误";
    QString err_unreg = "该用户未注册";
    QString datas = tcpSocket->readAll();
    int list_size;
    int i;

    QString head = datas.mid(0, 2);
    QString tail = datas.right(2);
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

        if (type == "4") {
            if (result == "01") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1, *item2, *item3;
                QTableWidgetItem *checkBox;
                checkBox = new QTableWidgetItem;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;
                item2 = new QTableWidgetItem;
                item3 = new QTableWidgetItem;

                QString name = strlist.at(1);
                QString id = strlist.at(2);
                checkBox->setCheckState(Qt::Unchecked);

                item0->setText(name);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                ui->tabWidget_teacher->setRowCount(rowNum);
                ui->tabWidget_teacher->setItem(pktcnt, 0, item0);
                item1->setText(id);
                ui->tabWidget_teacher->setItem(pktcnt, 1, item1);
                ui->tabWidget_teacher->setItem(pktcnt, 2, checkBox);
                qDebug() << pktcnt <<userdata;
                ItemTeacherCnt++;
            } else if (result == "02") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1;
                QTableWidgetItem *checkBox;
                checkBox = new QTableWidgetItem;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;

                QString name = strlist.at(1);
                QString id = strlist.at(2);
                checkBox->setCheckState(Qt::Unchecked);

                item0->setText(name);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                ui->tabWidget_stu->setRowCount(rowNum);
                ui->tabWidget_stu->setItem(pktcnt, 0, item0);
                item1->setText(id);
                ui->tabWidget_stu->setItem(pktcnt, 1, item1);
                ui->tabWidget_stu->setItem(pktcnt, 2, checkBox);
                ItemStuCnt++;
                qDebug() << pktcnt <<userdata;
            } else if (result == "03") {
                QStringList strlist = userdata.split("|");
                QTableWidgetItem *item0, *item1, *item2, *item3;
                QTableWidgetItem *checkBox;
                checkBox = new QTableWidgetItem;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;

                QString paperid = strlist.at(1);
                QString major = strlist.at(2);
                checkBox->setCheckState(Qt::Unchecked);

                item0->setText(paperid);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                qDebug() << "row :" << rowNum;
                ui->tableWidget_exam->setRowCount(rowNum);

                item1->setText(major);
                ui->tableWidget_exam->setItem(pktcnt, 0, item0);
                ui->tableWidget_exam->setItem(pktcnt, 1, item1);
                ui->tableWidget_exam->setItem(pktcnt, 2, checkBox);
                qDebug() << pktcnt <<userdata;
                ItemExamCnt++;
            }
        } else if (type == "5") {
            if (result == "01") {
                QMessageBox::warning(this,tr("Warning"),tr("更新成功"));
            } else if (result == "00"){
                QMessageBox::warning(this,tr("Warning"),tr("更新失败"));
            }
        }
    }
}


void adminpersonal::new_tcp_connect()
{

   tcpSocket->abort();

   tcpSocket->connectToHost("192.168.0.108", 1234);

}

void adminpersonal::displayError(QAbstractSocket::SocketError)

{
    QString err = "服务器连接失败";
    qDebug()<<tcpSocket->errorString();
    this->close();
    tcpSocket->close();

}

int adminpersonal::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);
    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}

void adminpersonal::on_btnReTea_clicked()
{

    QString type = "40";
    sendcmd(userid, type);
    ItemTeacherCnt = 0;
    ItemExamCnt = 0;
    qDebug() << "Refresh .....";
}

void adminpersonal::on_btnCancel_clicked()
{
    bool flag = false;
    QString type;
    int i, j;
    QSqlQuery query;

    QString logincode = "53";
    QString delta = "|";
    QString major, paperid, studentid;
    QTableWidgetItem *itemExam;
    QTableWidgetItem *itemExamchk;

    QCheckBox *chk;

    qDebug() << ItemExamCnt <<"Examing";
    for (i=0; i<ItemExamCnt; i++) {
        QString stuid; //= ui->tableWidget_exam->item(i, 0)->text();
        itemExam = ui->tableWidget_exam->item(i, 1);
        itemExamchk = ui->tableWidget_exam->item(i, 2);
        paperid = ui->tableWidget_exam->item(i, 0)->text();
        major = itemExam->text();
        if (itemExam == NULL || itemExamchk == NULL) {
            continue;
        }
        if (itemExamchk->checkState() != Qt::Checked) {
            qDebug() << i << "student is not select";
            continue;
        }
        //stuid = itemExam->text();
        QString msg0 = "是否要删除";
        QString msg1 = "考试";
        QString msg;

        msg += msg0;
        msg += major;
        msg += msg1;
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                        msg,
                                        QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            //startRecord();
            //getPaper();
            qDebug() << "YES";
        } else if (reply == QMessageBox::No) {
            qDebug()<< "NO";
            continue;
           // this->close();
        }
        ui->tableWidget_exam->removeRow(i);

        qDebug() << "Del exam " << i << paperid;
        ItemExamCnt--;
        qDebug() << paperid;
        sendcmd(paperid, logincode);
    }
}

void adminpersonal::fill_paper_name(int row, int cloume)
{
    QTableWidgetItem * item1 = new QTableWidgetItem;
    item1 = ui->tableWidget_exam->item(row, 1);
    ui->currentPaper->clear();
    ui->currentPaper->setText(item1->text());
    qDebug()<<item1->text();
}

void adminpersonal::on_btnAddTea_clicked()
{
    regTeacher *t = new regTeacher;
    t->show();
}

void adminpersonal::on_btnAddStu_clicked()
{
    regStudent *s = new regStudent;
    s->show();
}

void adminpersonal::on_btnReStu_clicked()
{
    QString type = "40";
    sendcmd(userid, type);
    ItemStuCnt = 0;
    ItemExamCnt = 0;
    qDebug() << "Refresh .....";
}

void adminpersonal::on_btnDelStu_clicked()
{
    bool flag = false;
    QString type;
    int i, j;
    QSqlQuery query;

    QString logincode = "55";
    QString delta = "|";
    QString major, teacherid, studentid;
    QTableWidgetItem *itemStu;
    QTableWidgetItem *itemStuchk;

    QCheckBox *chk;

    qDebug() << ItemStuCnt <<"students";
    for (i=0; i<ItemStuCnt; i++) {
        QString stuid;
        itemStu = ui->tabWidget_stu->item(i, 1);
        itemStuchk = ui->tabWidget_stu->item(i, 2);

        if (itemStu == NULL || itemStuchk == NULL) {
            continue;
        }
        if (itemStuchk->checkState() != Qt::Checked) {
            qDebug() << i << "student is not select";
            continue;
        }
        stuid = itemStu->text();
        ui->tabWidget_stu->removeRow(i);

        qDebug() << "Del student " << i << stuid;
        ItemStuCnt--;
        qDebug()<<stuid;
        sendcmd(stuid, logincode);
    }
}

void adminpersonal::on_btnDelTea_clicked()
{
    bool flag = false;
    QString type;
    int i, j;
    QSqlQuery query;

    QString logincode = "54";
    QString delta = "|";
    QString major, teacherid;
    QTableWidgetItem *itemTeacher;
    QTableWidgetItem *itemTeacherchk;
    QTableWidgetItem *itemStu;
    QTableWidgetItem *itemStuchk;
    QCheckBox *chk;
    major = ui->addpapername->text();
    qDebug() << ItemTeacherCnt <<"Teacher";
    for (i=0; i<ItemTeacherCnt; i++) {
        QString teacherid;
        itemTeacher = ui->tabWidget_teacher->item(i, 1);
        itemTeacherchk = ui->tabWidget_teacher->item(i, 2);
        if (itemTeacherchk == NULL) {
            continue;
        }
        if (itemTeacherchk->checkState() != Qt::Checked) {
            qDebug() << i << "Teacher is not select";
            continue;
        }
        teacherid = itemTeacher->text();

        if (itemTeacher != NULL) {
            ui->tabWidget_teacher->removeRow(i);
        }

        qDebug() << "Del " << i;
        ItemTeacherCnt--;
        qDebug()<<teacherid;
        sendcmd(teacherid, logincode);

    }
}

void adminpersonal::on_btnRefreshStu_clicked()
{
    QString type = "40";
    sendcmd(userid, type);

    qDebug() << "Refresh .....";
}

void adminpersonal::on_btnsubmit_clicked()
{
    bool flag = false;
    QString type;
    QSqlQuery query;

    QString logincode = "50";
    QString delta = "|";
    QString major, teacherid, studentid;
    QTableWidgetItem *itemTeacher;
    QTableWidgetItem *itemTeacherchk;
    QTableWidgetItem *itemStu;
    QTableWidgetItem *itemStuchk;
    QCheckBox *chk;
    major = ui->addpapername->text();

    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd-hh:mm:ss"); //设置显示格式

    qDebug() << ItemTeacherCnt << ItemStuCnt;
    if(major.isEmpty()) {
        flag = true;
        goto TAR_OUT;
    }

    int i, j;

    itemTeacher = ui->tabWidget_teacher->item(0, 1);
    qDebug() << itemTeacher->text();
    for (i=0; i<ItemTeacherCnt; i++) {
        itemTeacher = ui->tabWidget_teacher->item(i, 1);
        itemTeacherchk = ui->tabWidget_teacher->item(i, 2);
        if (itemTeacherchk->checkState() != Qt::Checked) {
            qDebug() << i << "Teacher is not select";
            continue;
        }
        itemTeacherchk->setCheckState(Qt::Unchecked);
        for (j=0; j<ItemStuCnt; j++) {
            itemStu = ui->tabWidget_stu->item(j, 1);
            itemStuchk = ui->tabWidget_stu->item(j, 2);
            if (itemStuchk->checkState() != Qt::Checked) {
                qDebug() << j << " Student is not select";
                continue;
            }
            itemStuchk->setCheckState(Qt::Unchecked);
            QByteArray  tmp_in (major.toUtf8());
            QString name_base64 = QString(tmp_in.toBase64());
            QString senddata ="";
            senddata = str +  delta + name_base64 + delta + tr("100") + delta + itemTeacher->text() + delta +\
                    itemStu->text()  +  delta + tr("60") + delta;

            qDebug()<<senddata;
            sendcmd(senddata, logincode);
        }
    }


    QTableWidgetItem *item0, *item1, *item2, *item3;
    QTableWidgetItem *checkBox;
    checkBox = new QTableWidgetItem;
    item0 = new QTableWidgetItem;
    item1 = new QTableWidgetItem;
    //QString major = ui->addpapername->text();;

    checkBox->setCheckState(Qt::Unchecked);

    item0->setText(str);
    item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
    item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
    //int rowNum = pktcnt + 1;
    ui->tableWidget_exam->setRowCount(ItemExamCnt+1);

    item1->setText(major);
    ui->tableWidget_exam->setItem(ItemExamCnt, 0, item0);
    ui->tableWidget_exam->setItem(ItemExamCnt, 1, item1);
    ui->tableWidget_exam->setItem(ItemExamCnt, 2, checkBox);
    ItemExamCnt++;
    ui->addpapername->clear();
TAR_OUT:
    if(flag) {
        QMessageBox::warning(this,tr("Warning"),tr("科目名称不能为空!"));
    }

}

