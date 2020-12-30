#ifndef PARAMETERSERVER_H
#define PARAMETERSERVER_H

#include "paramsserver_global.h"
#include <QObject>
#include "configuru.hpp"
#include "simplethread.h"
#include <mutex>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <iostream>

class PARAMSSERVERSHARED_EXPORT ParameterServer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ParameterServer)
private:
    explicit ParameterServer(QObject *parent = nullptr);
    ~ParameterServer() {
        stop_server();
    }

    std::shared_ptr<Runnable> m_ServerThreadContext;
    std::shared_ptr<Thread> m_ServerThread;
    configuru::Config _cfgRoot;

public:
    inline configuru::Config &GetCfgRoot() {
        return _cfgRoot;
    }
    inline configuru::Config &GetCfgCtrlRoot() {
        _cfgRoot.judge_or_create_key("dev_ctrl");
        return _cfgRoot["dev_ctrl"];
    }
    static ParameterServer *instance()
    {
        static ParameterServer *_this = nullptr;
        if (_this == nullptr)
        {
            _this = new ParameterServer;
            _this->init();
            _this->start_server();
        }
        return _this;
    }
    inline bool is_debug() {return debug_;}
    void init();
    void start_server();
    void stop_server();
    bool debug_;
};

#endif // PARAMETERSERVER_H
