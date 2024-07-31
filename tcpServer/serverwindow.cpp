#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ServerWindow), tcpServer(new QTcpServer(this)), clientSocket(nullptr)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &ServerWindow::startListening);
    connect(tcpServer, &QTcpServer::newConnection, this, &ServerWindow::newConnection);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::startListening()
{
    QString address = ui->addressLineEdit->text().isEmpty() ? "127.0.0.1" : ui->addressLineEdit->text();
    quint16 port = ui->portLineEdit->text().isEmpty() ? 12345 : ui->portLineEdit->text().toUShort();

    if (tcpServer->listen(QHostAddress(address), port)) {
        ui->statusLabel->setText("Listening on " + address + ":" + QString::number(port));
    } else {
        ui->statusLabel->setText("Failed to listen.");
    }
}

void ServerWindow::newConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ServerWindow::readClient);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ServerWindow::clientDisconnected);
    ui->statusLabel->setText("Client connected.");
}

void ServerWindow::readClient()
{
    QByteArray data = clientSocket->readAll();
    if (data.trimmed() == "ping") {
        clientSocket->write("pong");
    }
}

void ServerWindow::clientDisconnected()
{
    ui->statusLabel->setText("Client disconnected.");
    clientSocket->deleteLater();
    clientSocket = nullptr;
}
