/****************************************************************************
**
** Copyright (C) 2020 The ZHIZUO Company Ltd.
**
** This file is part of the ZHIZUO Ultrasound SDK.
**
** Private License
** All right include copy change are reserved.
**
****************************************************************************/

#ifndef PARAMETERSERVER_H
#define PARAMETERSERVER_H

#include <QObject>
#include <QThread>
#include <QScriptValue>
#include <QScriptEngine>
#include "configuru.hpp"
#include "pipelinedomsingleton.h"
#include "ctrltableutil.h"
#include <mutex>

class ParameterServer : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(ParameterServer)
 private:
  explicit ParameterServer(QObject *parent = nullptr);
  ~ParameterServer() {
    m_processCenter->quit();
  }

  QThread *m_processCenter;
  configuru::Config _cfgRoot;
  CtrlTableUtil _tableUtil;

 public:
  inline configuru::Config &GetCfgRoot() {
    return _cfgRoot;
  }
  inline configuru::Config &GetCfgCtrlRoot() {
    _cfgRoot.judge_or_create_key("dev_ctrl");
    return _cfgRoot["dev_ctrl"];
  }
  static ParameterServer *instance() {
    static ParameterServer *_this = nullptr;
    if (_this == nullptr) {
      _this = new ParameterServer;
      if (_this->m_processCenter) {
        _this->init();
        _this->m_processCenter->start();
      }
    }
    return _this;
  }
  inline bool is_debug() {return debug_;}
  void init();
  pipeline_fpga_table m_ctrlTable;
  configuru::Config GetFromCtrlTable(const pipeline_fpga_table &table_in);
  pipeline_fpga_table GetFromCfgTable(const configuru::Config &table_in);
  pipeline_fpga_table GetFromCfgTable(const pipeline_fpga_table &src, const configuru::Config &cfg_table);
  void initCtrlTableParamServ();

  bool debug_;
};

#endif // PARAMETERSERVER_H
