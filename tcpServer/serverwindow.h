#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringListModel>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void startListening();
    void stopListening();
    void newConnection();
    void readClient();
    void clientDisconnected();
    void sendPing();
    void logMessage(const QString &message);

private:
    Ui::ServerWindow *ui;
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    QStringListModel *logModel;  // Model to hold log entries
};

#endif // SERVERWINDOW_H
