#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QDebug>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ServerWindow), tcpServer(new QTcpServer(this)), clientSocket(nullptr),
      logModel(new QStringListModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Server");

    ui->listView->setModel(logModel);  // Connect the ListView to the model

    connect(ui->startButton, &QPushButton::clicked, this, &ServerWindow::startListening);
    connect(ui->stopButton, &QPushButton::clicked, this, &ServerWindow::stopListening);
    connect(ui->pingButton, &QPushButton::clicked, this, &ServerWindow::sendPing);
    connect(tcpServer, &QTcpServer::newConnection, this, &ServerWindow::newConnection);
}

ServerWindow::~ServerWindow()
{
    if (tcpServer->isListening()) {
        stopListening();
    }
    delete ui;
}

void ServerWindow::startListening()
{
    QString address = ui->addressLineEdit->text().isEmpty() ? "127.0.0.1" : ui->addressLineEdit->text();
    quint16 port = ui->portLineEdit->text().isEmpty() ? 12345 : ui->portLineEdit->text().toUShort();

    if (tcpServer->listen(QHostAddress(address), port)) {
        ui->statusLabel->setText("Listening on " + address + ":" + QString::number(port));
        logMessage("Server started. Listening on " + address + ":" + QString::number(port));
    } else {
        ui->statusLabel->setText("Failed to start the server.");
        logMessage("Server failed to start.");
    }
}

void ServerWindow::stopListening()
{
    if (tcpServer->isListening()) {
        tcpServer->close();
        ui->statusLabel->setText("Server stopped.");
        logMessage("Server stopped.");
        disconnect(tcpServer, &QTcpServer::newConnection, this, &ServerWindow::newConnection);

        if (clientSocket && clientSocket->isOpen()) {
            clientSocket->disconnectFromHost();
        }
    }
}

void ServerWindow::newConnection()
{
    clientSocket = tcpServer->nextPendingConnection();
    logMessage("New client connection attempt.");

    if (clientSocket) {
        connect(clientSocket, &QTcpSocket::readyRead, this, &ServerWindow::readClient);
        connect(clientSocket, &QTcpSocket::disconnected, this, &ServerWindow::clientDisconnected);

        QString clientAddress = clientSocket->peerAddress().toString();
        quint16 clientPort = clientSocket->peerPort();
        logMessage("Client connected from " + clientAddress + ":" + QString::number(clientPort));

        clientSocket->write("Welcome to the server.\n");
        ui->statusLabel->setText("Client connected.");
    } else {
        logMessage("Failed to get the client connection.");
    }
}

void ServerWindow::readClient()
{
    QByteArray data = clientSocket->readAll();
    logMessage("Received from client: " + QString(data.trimmed()));

    if (data.trimmed() == "ping") {
        clientSocket->write("pong");
        logMessage("pong");
    }
}

void ServerWindow::clientDisconnected()
{
    QString clientAddress = clientSocket->peerAddress().toString();
    quint16 clientPort = clientSocket->peerPort();
    logMessage("Client disconnected from " + clientAddress + ":" + QString::number(clientPort));

    ui->statusLabel->setText("Client disconnected.");
    clientSocket->deleteLater();
    clientSocket = nullptr;
}

void ServerWindow::sendPing()
{
    if (clientSocket && clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write("ping");
        logMessage("Sent 'ping' to client.");
    } else {
        logMessage("No client connected to send 'ping'.");
    }
}

void ServerWindow::logMessage(const QString &message)
{
    QStringList log = logModel->stringList();
    log.append(message);
    logModel->setStringList(log);
    qDebug() << message;
}
