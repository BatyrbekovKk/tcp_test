#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QStringListModel>

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void toggleConnection();
    void connectToServer();
    void disconnectFromServer();
    void updateConnectionStatus();
    void sendPing();
    void readServer();
    void handleError(QAbstractSocket::SocketError socketError);
    void logMessage(const QString &message);

private:
    Ui::ClientWindow *ui;
    QTcpSocket *tcpSocket;
    QStringListModel *logModel;
};

#endif // CLIENTWINDOW_H
