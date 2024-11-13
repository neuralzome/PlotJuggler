#include "zmq_multi_connect.h"
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>


ZmqMultiConnectPlugin::ZmqMultiConnectPlugin()
    : context(nullptr), running(false) {}

ZmqMultiConnectPlugin::~ZmqMultiConnectPlugin() {
    shutdown();
}

void ZmqMultiConnectPlugin::init(PlotJuggler::MainWindow* mainWindow) {
    
    context = new zmq::context_t(1);
    running = true;

    
    QWidget* widget = new QWidget(mainWindow);
    QVBoxLayout* layout = new QVBoxLayout(widget);

   
    QLabel* label = new QLabel("Enter ZMQ Ports (comma-separated)", widget);
    QLineEdit* portsInput = new QLineEdit(widget);
    QPushButton* connectButton = new QPushButton("Connect", widget);
    layout->addWidget(label);
    layout->addWidget(portsInput);
    layout->addWidget(connectButton);

    connect(connectButton, &QPushButton::clicked, [this, portsInput]() {
        QStringList ports = portsInput->text().split(",", QString::SkipEmptyParts);
        configureSockets(ports);
    });

    
    widget->setLayout(layout);
    widget->show();
}

void ZmqMultiConnectPlugin::shutdown() {
    running = false;
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    sockets.clear();
    delete context;
}

void ZmqMultiConnectPlugin::configureSockets(const QStringList& ports) {
    for (const auto& port : ports) {
        zmq::socket_t socket(*context, zmq::socket_type::sub);
        socket.connect(port.toStdString());
        sockets.push_back(std::move(socket));
    }
}

void ZmqMultiConnectPlugin::receiveFromSocket(int socketIndex) {
    zmq::message_t msg;
    while (running) {
        sockets[socketIndex].recv(msg, zmq::recv_flags::dontwait); 
        if (msg.size()) {
            
            qDebug() << "Received message from socket " << socketIndex << ": " << QString::fromStdString(msg.to_string());
        }
    }
}

void ZmqMultiConnectPlugin::receiveData() {
    // Placeholder for potential UI interactions or data handling logic
}
