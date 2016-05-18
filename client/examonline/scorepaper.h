#ifndef SCOREPAPER_H
#define SCOREPAPER_H

#include <QDialog>
#include<QtNetwork>
#include<QtNetwork/QTcpSocket>

namespace Ui {
    class scorepaper;
}

class scorepaper : public QDialog
{
    Q_OBJECT

public:
    explicit scorepaper(QWidget *parent = 0);
    ~scorepaper();

private:
    Ui::scorepaper *ui;
    void showPaper();
};
#endif // SCOREPAPER_H
