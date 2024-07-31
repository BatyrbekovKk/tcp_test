#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void startListening();
    void newConnection();
    void readClient();
    void clientDisconnected(); // Добавлено

private:
    Ui::ServerWindow *ui;
    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
};

#endif // SERVERWINDOW_H
