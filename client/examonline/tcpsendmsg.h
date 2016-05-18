#ifndef TCPSENDMSG_H
#define TCPSENDMSG_H

#include <QtNetwork/qtcpsocket.h>
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QAbstractSocket>
#include <QByteArray>

class tcpsendMsg : public QObject
{
public:
    explicit tcpsendMsg(QObject *parent = 0);
    ~tcpsendMsg();

private:
    int tcpPort;
    QByteArray pkt_head;
    QByteArray pkt_tail;
    QTcpSocket *socket_;
    bool isConnected_;
    qint64 pkt_num;

public slots:
    void ReadDataFromSocket();
    void onDisConnection();
    void sendmsg(QString msg, QString funcode);
    void processMSG(QString &MSG);
    void connectedSlot();
    void disconnectedSlot();
    void revData(); //接收来自服务端的数据
    QByteArray readyReadSlot();
    void errorSlot(QAbstractSocket::SocketError);

public:
    int sendcmd(QString data, QString type);
    enum {
        ADMIN = 0,
        TEACHER = 1,
        STUDENT = 2,
    };
    QTcpSocket *tcpSocket;
    int user_type;
    void init();
protected:


    void new_tcp_connect();
  // void processAddedClient(QTcpSocket *client);
};

#endif // TCPSENDMSG_H
