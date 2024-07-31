#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::connectToServer);
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientWindow::updateConnectionStatus);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &ClientWindow::updateConnectionStatus);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::connectToServer()
{
    QString address = ui->addressLineEdit->text().isEmpty() ? "127.0.0.1" : ui->addressLineEdit->text();
    quint16 port = ui->portLineEdit->text().isEmpty() ? 12345 : ui->portLineEdit->text().toUShort();

    tcpSocket->connectToHost(address, port);
    connect(tcpSocket, &QTcpSocket::connected, this, &ClientWindow::sendPing);
}


void ClientWindow::disconnectFromServer()
{
    tcpSocket->disconnectFromHost();
}

void ClientWindow::updateConnectionStatus()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        ui->statusLabel->setText("Connected");
        ui->connectButton->setText("Disconnect");
        connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::disconnectFromServer);
    } else {
        ui->statusLabel->setText("Disconnected");
        ui->connectButton->setText("Connect");
        connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::connectToServer);
    }
}

void ClientWindow::sendPing()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write("ping");
    }
}
