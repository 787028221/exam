#include "tcpsendmsg.h"
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
#include<QtNetwork>

tcpsendMsg::tcpsendMsg(QObject *parent):
    QObject(parent)
{
    init();
//    socket_ = new QTcpSocket(this);
//    tcpPort = 8888;


//    pkt_num = 0;

//    pkt_head.append("EAEA");
//    pkt_tail.append("EB");

//    connect(socket_, SIGNAL(connected()), this, SLOT(connectedSlot()));
//    connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
//    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
//    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
//            SLOT(errorSlot(QAbstractSocket::SocketError)));

}



tcpsendMsg::~tcpsendMsg()
{
    socket_->close();
}



