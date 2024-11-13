#pragma once
#include <QDialog>
#include <QtPlugin>
#include <thread>
#include "PlotJuggler/datastreamer_base.h"
#include "PlotJuggler/messageparser_base.h"
#include "ui_datastream_zmq_multi.h"
#include "zmq.hpp"

class StreamZMQMultiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StreamZMQMultiDialog(QWidget* parent = nullptr);
    ~StreamZMQMultiDialog();

    Ui::DataStreamZMQMulti* ui;
};

class DataStreamZMQMulti : public PJ::DataStreamer
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "facontidavide.PlotJuggler3.DataStreamer")
    Q_INTERFACES(PJ::DataStreamer)

public:
    DataStreamZMQMulti();

    virtual ~DataStreamZMQMulti() override;

    virtual bool start(QStringList*) override;

    virtual void shutdown() override;

    virtual bool isRunning() const override
    {
        return _running;
    }

    virtual const char* name() const override
    {
        return "ZMQ Multi-Connection Subscriber";
    }

    virtual bool isDebugPlugin() override
    {
        return false;
    }

private:
    bool _running;
    zmq::context_t _zmq_context;  
    std::vector<zmq::socket_t> _zmq_sockets;  
    std::vector<std::thread> _receive_threads; 
    PJ::MessageParserPtr _parser;
    std::vector<std::string> _socket_addresses; 
    std::vector<std::vector<std::string>> _topic_filters; 

    void receiveLoop(int socket_index);  
    bool parseMessage(const PJ::MessageRef& msg, double& timestamp);
    void parseTopicFilters(const QString& filters, int socket_index);
    void subscribeTopics(int socket_index);
    void unsubscribeTopics(int socket_index);
    void cleanupThreads();
};

