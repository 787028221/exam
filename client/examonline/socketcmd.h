#ifndef SOCKETCMD_H
#define SOCKETCMD_H

#include <QString>

class socketcmd
{
public:
    socketcmd();
    ~socketcmd();
    int sendmsg(const int type, const QString data);

public:

};

#endif // SOCKETCMD_H
