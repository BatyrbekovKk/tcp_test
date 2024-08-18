#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QDebug>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), tcpSocket(new QTcpSocket(this)),
      logModel(new QStringListModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Client");

    ui->listView->setModel(logModel);  // Connect the ListView to the model

    connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::toggleConnection);

    connect(tcpSocket, &QTcpSocket::connected, this, &ClientWindow::updateConnectionStatus);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientWindow::updateConnectionStatus);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientWindow::readServer);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &ClientWindow::handleError);

    connect(ui -> pingButton, &QPushButton::clicked, this, &ClientWindow::sendPing);

}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::toggleConnection()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        disconnectFromServer();
    } else {
        connectToServer();
    }
}

void ClientWindow::connectToServer()
{
    QString address = ui->addressLineEdit->text().isEmpty() ? "127.0.0.1" : ui->addressLineEdit->text();
    quint16 port = ui->portLineEdit->text().isEmpty() ? 12345 : ui->portLineEdit->text().toUShort();

    logMessage("Attempting to connect to server at " + address + ":" + QString::number(port));
    tcpSocket->connectToHost(address, port);
}

void ClientWindow::disconnectFromServer()
{
    logMessage("Disconnecting from server.");
    tcpSocket->disconnectFromHost();
}

void ClientWindow::updateConnectionStatus()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        ui->statusLabel->setText("Connected");
        ui->connectButton->setText("Disconnect");
        logMessage("Connected to server.");
    } else {
        ui->statusLabel->setText("Disconnected");
        ui->connectButton->setText("Connect");
        logMessage("Disconnected from server.");
    }
}

void ClientWindow::sendPing()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write("ping");
        logMessage("Sent 'ping' to server.");
    } else {
        logMessage("Cannot send 'ping' - not connected to server.");
    }
}

void ClientWindow::readServer()
{
    QByteArray data = tcpSocket->readAll();
    logMessage("Received from server: " + QString(data.trimmed()));
}

void ClientWindow::handleError(QAbstractSocket::SocketError socketError)
{
    logMessage("Socket error: " + tcpSocket->errorString());
}

void ClientWindow::logMessage(const QString &message)
{
    QStringList log = logModel->stringList();
    log.append(message);
    logModel->setStringList(log);
    qDebug() << message;
}
