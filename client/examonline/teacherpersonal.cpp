#include "teacherpersonal.h"
#include "ui_teacherpersonal.h"
#include "scorepaper.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QStringList>

extern QString userid;
extern QString global_passwd;

teacherpersonal::teacherpersonal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::teacherpersonal)
{
    ui->setupUi(this);
    init();
    connect(ui->tabWidget_teacher,SIGNAL(cellDoubleClicked(int,int)),this, SLOT(openexam(int,int)));
}


teacherpersonal::~teacherpersonal()
{
    delete ui;
}

void teacherpersonal::init()
{

    QString type = "22";

    tcpSocket = new QTcpSocket(this);

    new_tcp_connect();
    questionType = 0;
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

    sendcmd(userid, type);

    QStringList headers;
    headers << tr( "姓名" ) << tr( "试卷号" ) << tr( "科目" ) <<  tr( "成绩" )<<  tr( "总分" );
    ui->tabWidget_teacher->setColumnCount(5);
    ui->tabWidget_teacher->setHorizontalHeaderLabels(headers);
    ui->tabWidget_teacher->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    connect(ui->cbx_select, SIGNAL(currentIndexChanged(int)), this, SLOT(choice(int)));
    headers.clear();
    headers << tr( "类型" ) << tr( "题目" ) << tr( "答案" ) <<  tr( "选项" );
    ui->labQuestionList->setColumnCount(4);
    ui->labQuestionList->setHorizontalHeaderLabels(headers);
    ui->labQuestionList->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    headers.clear();
    headers << tr( "类型" ) << tr( "题目" ) << tr( "答案" );
    ui->tabWExam->setColumnCount(3);
    ui->tabWExam->setHorizontalHeaderLabels(headers);
    ui->tabWExam->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
    ui->otherQuestion->hide();
    ui->chbA->hide();
    ui->chbB->hide();
    ui->chbC->hide();
    ui->chbD->hide();
    dbQuestionNum = 0;
}

void teacherpersonal::choice(int index)
{
    qDebug() << index;

    questionType = index;
    switch(index) {
    case 0:
        ui->cbx_ans->show();
        ui->otherQuestion->hide();
        ui->chbA->hide();
        ui->chbB->hide();
        ui->chbC->hide();
        ui->chbD->hide();
        isJudge = false;
        break;
    case 1:
        ui->cbx_ans->hide();
        ui->otherQuestion->hide();
        ui->chbA->setText(tr("A"));
        ui->chbA->show();
        ui->chbB->show();
        ui->chbB->setText(tr("B"));
        ui->chbC->show();
        ui->chbD->show();
        isJudge = false;
        break;
    case 2:
        ui->cbx_ans->hide();
        ui->otherQuestion->show();
        ui->chbA->hide();
        ui->chbB->hide();
        ui->chbC->hide();
        ui->chbD->hide();
        isJudge = false;
        break;
    case 3:
        ui->cbx_ans->hide();
        ui->otherQuestion->hide();
        ui->chbA->show();
        ui->chbA->setText(tr("对"));
        ui->chbB->show();
        ui->chbB->setText(tr("错"));
        ui->chbC->hide();
        ui->chbD->hide();
        isJudge = true;
        break;
    default:
        break;
    }
}

void  teacherpersonal::on_chbA_clicked()
{
    if (!isJudge) {
        return;
    }
    if (ui->chbA->isChecked()) {
        ui->chbB->hide();
    } else {
        ui->chbB->show();
    }
}

void  teacherpersonal::on_chbB_clicked()
{
    if (!isJudge) {
        return;
    }
    if (ui->chbB->isChecked()) {
        ui->chbA->hide();
    } else {
        ui->chbA->show();
    }

}

void teacherpersonal::revData()
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


        if (type == "2") {
            if (result == "21") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                if (list_size < 2) {
                    return;
                }

                QString name = strlist.at(0);
                QString id = strlist.at(1);
                QString type = "41";
                sendcmd(userid, type);
            } else if (result == "20") {
                QMessageBox::information(this, "info", "个人信息提取失败");
            } else {
                QMessageBox::information(this, "info", datas);
            }
        } else if (type == "3") {
            if (result == "11") {
                QMessageBox::information(this, "info", "修改密码成功");
                ui->leOldPwd->clear();
                ui->leNewPwd->clear();
                ui->leNewPwd_chk->clear();
            } else if (result == "10") {
                QMessageBox::information(this, "info", "修改密码失败");
            } else {
                QMessageBox::information(this, "info", datas);
            }
        } else if (type == "4") {
            if (result == "11") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1, *item2, *item3, *item4;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;
                item2 = new QTableWidgetItem;
                item3 = new QTableWidgetItem;
                item4 = new QTableWidgetItem;

                int pktcnt = strlist.at(0).toInt();
                QString paperid = strlist.at(1);
                QString major = strlist.at(2);
                QString score = strlist.at(3);
                QString fullscore = strlist.at(4);
                QString name = strlist.at(5);

                item1->setText(paperid);
                item2->setText(major);
                item3->setText(score);
                item4->setText(fullscore);
                item0->setText(name);

                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                item4->setFlags(item4->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                qDebug() << rowNum << paperid << major << score << fullscore << name;
                ui->tabWidget_teacher->setRowCount(rowNum);
                ui->tabWidget_teacher->setItem(pktcnt, 0, item0);
                ui->tabWidget_teacher->setItem(pktcnt, 1, item1);
                ui->tabWidget_teacher->setItem(pktcnt, 2, item2);
                ui->tabWidget_teacher->setItem(pktcnt, 3, item3);
                ui->tabWidget_teacher->setItem(pktcnt, 4, item4);
            } else if (result == "12") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                ui->cbxType_major->addItem(strlist.at(2));
                qDebug() << strlist.at(2) << strlist.at(1);
                majormap.insert(strlist.at(2), strlist.at(1));
                qDebug() << userdata;
            } else if (result == "13") {
                QStringList strlist = userdata.split("|");
                list_size = strlist.size();
                QTableWidgetItem *item0, *item1, *item2, *item3;
                QTableWidgetItem *checkBox;

                checkBox = new QTableWidgetItem;
                item0 = new QTableWidgetItem;
                item1 = new QTableWidgetItem;
                item2 = new QTableWidgetItem;
                item3 = new QTableWidgetItem;

                int pktcnt = strlist.at(0).toInt();
                QString questiontype = strlist.at(1);
                QString major = strlist.at(2);
                QString question = strlist.at(3);
                QString answer = strlist.at(4);
                //QString name = strlist.at(5);

                item0->setText(questiontype);
                //item1->setText(major);
                item1->setText(question);
                item2->setText(answer);
                //item0->setText(name);
                checkBox->setCheckState(Qt::Unchecked);
                item0->setFlags(item0->flags() & (~Qt::ItemIsEditable));
                item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
                item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
                item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
                int rowNum = pktcnt + 1;
                ui->labQuestionList->setRowCount(rowNum);
                ui->labQuestionList->setItem(pktcnt, 0, item0);
                ui->labQuestionList->setItem(pktcnt, 1, item1);
                ui->labQuestionList->setItem(pktcnt, 2, item2);
                ui->labQuestionList->setItem(pktcnt, 3, checkBox);
            }
        } else if (type == "6") {
            if (result == "11") {
                QMessageBox::information(this, "info", "添加成功");
            } else {
                QMessageBox::information(this, "info", "添加失败");
            }
        }
    }
}

void teacherpersonal::on_btnOk_clicked()
{

    bool flag = false;
    QSqlQuery query;
    QString old_pwd;
    QString new_pwd;
    QString new_pwd_chk;
    QString senddata;
    QString logincode = "31";
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
    QMessageBox::warning(this,tr("Warning"), senddata);

TAR_OUT:
    if(flag)
        QMessageBox::warning(this,tr("Warning"),tr("密码不能为空!"));

}

void teacherpersonal::on_btnQuit_clicked()
{
    ui->leOldPwd->clear();
    ui->leNewPwd->clear();
    ui->leNewPwd_chk->clear();
}

void teacherpersonal::on_btnAdd_clicked()
{
    QString paperid;
    QString type;
    QString question;
    QString answer;
    QString cmd = "61";
    QString delta = "|";
    QString senddata;
    QString quesType;
    QString major;

    qDebug() << ui->cbxType_major->currentText();
    QMap<QString,QString>::iterator it = majormap.find(ui->cbxType_major->currentText());
    paperid = it.value();
    qDebug() << paperid;
    type = ui->cbx_select->currentText();
    qDebug() << type;
    question = ui->te_question->toPlainText();
    qDebug() << question;

    switch(questionType) {
    case SINGLE_CHOICE:
        answer = ui->cbx_ans->currentText();
        break;
    case MULTI_CHOICE:
        if (ui->chbA->isChecked()) {
            answer += tr("A&");
        }
        if (ui->chbB->isChecked()) {
            answer += tr("B&");
        }
        if (ui->chbC->isChecked()) {
            answer += tr("C&");
        }
        if (ui->chbD->isChecked()) {
            answer += tr("D");
        }
        break;

    case BLANK_FILL:
        answer = ui->otherQuestion->text();
        break;
    case JUDGE_QUES:
        if (ui->chbA->isChecked()) {
            answer = tr("YES");
        } else if (ui->chbB->isChecked()) {
            answer = tr("NO");
        } else {
            qDebug() << "Choice is err, Please check";
        }
        break;

    default:
        break;

    }
    answer = ui->cbx_ans->currentText();
    qDebug() << answer;

    quesType = QString::number(questionType, 10);
    major = ui->cbxType_major->currentText();
    QByteArray  tmp_in1 (major.toUtf8());
    QString str_base641 = QString(tmp_in1.toBase64());
    QByteArray  tmp_in (question.toUtf8());
    QString str_base64 = QString(tmp_in.toBase64());
    QByteArray  tmp_in2 (answer.toUtf8());
    QString str_base642 = QString(tmp_in2.toBase64());
    senddata = quesType + delta + str_base641 + delta + str_base64 + delta + str_base642;
    qDebug() << questionType << senddata;
    sendcmd(senddata, cmd);

    set_show_local(quesType, question, answer);
}

void teacherpersonal::on_btnGenrate_clicked()
{
    int i, j;
    QTableWidgetItem *checkBox;
    QTableWidgetItem *item;
    examLine = 0;

    for (i=0; i<ui->labQuestionList->rowCount(); i++) {
        checkBox = ui->labQuestionList->item(i, 3);
        if (checkBox->checkState() == Qt::Checked) {
            qDebug() << i << ui->labQuestionList->rowCount() << "Add it";
            ui->tabWExam->setRowCount(examLine+1);
            for (j=0; j<3; j++) {
                item = new QTableWidgetItem;
                item->setText(ui->labQuestionList->item(i, j)->text());
                qDebug() << i << j << ui->labQuestionList->item(i, j)->text();
                ui->tabWExam->setItem(examLine, j, item);
            }
            examLine++;
        }
    }
}

void teacherpersonal::set_show_local(QString quesType, QString question, QString answer)
{
    QTableWidgetItem *item1, *item2, *item3;
    QTableWidgetItem *checkBox;

    checkBox = new QTableWidgetItem;
    //item0 = new QTableWidgetItem;
    item1 = new QTableWidgetItem;
    item2 = new QTableWidgetItem;
    item3 = new QTableWidgetItem;

    checkBox->setCheckState(Qt::Unchecked);

    item1->setText(quesType);
    item2->setText(question);
    item3->setText(answer);
    item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
    item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
    item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));

    ui->labQuestionList->setRowCount(dbQuestionNum+1);
    ui->labQuestionList->setItem(dbQuestionNum, 0, item1);
    ui->labQuestionList->setItem(dbQuestionNum, 1, item2);
    ui->labQuestionList->setItem(dbQuestionNum, 2, item3);
    ui->labQuestionList->setItem(dbQuestionNum, 3, checkBox);
    qDebug()<< dbQuestionNum << quesType << question << answer;

    dbQuestionNum++;
}

void teacherpersonal::new_tcp_connect()
{

   tcpSocket->abort();

   tcpSocket->connectToHost("192.168.0.108", 1234);

}

void teacherpersonal::displayError(QAbstractSocket::SocketError)

{
    QString err = "服务器连接失败";
    qDebug()<<tcpSocket->errorString();
    tcpSocket->close();

}

int teacherpersonal::sendcmd(QString data, QString type)
{
    QString senddata;

    senddata.append(type);
    senddata.append(QString::fromStdString("|"));
    senddata.append(data);

    tcpSocket->write(senddata.toLatin1().data(), senddata.size());

    return 0;
}

void teacherpersonal::openexam(int row, int cloume)
{
    scorepaper *p = new scorepaper;
    p->show();
}
