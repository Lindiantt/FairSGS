#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include <QDialog>
#include "pch.h"

namespace Ui {
class DialogRegister;
}

class DialogRegister : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegister(QWidget *parent = 0);
    ~DialogRegister();
    void init(int reg);
    QString ip;
    quint16 port;

private slots:
    void on_lineEditPassword_textChanged(const QString &arg1);

    void on_pushButtonOK_clicked();
    void handleConnected();
    void handleError(QAbstractSocket::SocketError);
    void handleRead();
    void handleTimeout();
    void on_pushButtonCancel_clicked();

private:
    void closeEvent(QCloseEvent *);
    void regOver();
    Ui::DialogRegister *ui;
    QByteArray savedPassword,user,password;
    int reg;
    QTcpSocket *socket;
    QTimer timer;
    bool ok;
};

#endif // DIALOGREGISTER_H
