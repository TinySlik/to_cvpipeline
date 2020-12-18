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

#ifndef PIPELINEDOMSINGLETON_H
#define PIPELINEDOMSINGLETON_H

#include <QObject>
#include "ctrltableutil.h"

class PipelineEngineSingleton : public QObject {
 Q_OBJECT
 Q_DISABLE_COPY(PipelineEngineSingleton)

 typedef std::function<void(std::shared_ptr<std::vector<std::uint8_t>>)> data_func;
 public:
  static PipelineEngineSingleton *instance() {
    static PipelineEngineSingleton *_this = nullptr;
    if (_this == nullptr) {
      _this = new PipelineEngineSingleton;
    }
    return _this;
  }
  void setRecvCallback(data_func func);
  void setRecvCtrlTbCallback(std::function<void(std::shared_ptr<pipeline_fpga_table>)> func);
  void setCRecvCallback(data_func func);
  void setPWRecvCallback(data_func func);
  void removeCallback(size_t index);
  void removeCCallback(size_t index);
  void removePWCallback(size_t index);
 private:
  explicit PipelineEngineSingleton(QObject *parent = nullptr);
  std::vector<data_func> m_FuncProcesses;
  std::vector<data_func> m_CFuncProcesses;
  std::vector<data_func> m_PWFuncProcesses;
  /**
   * @brief m_CtrlProcess
   * single!
   */
  std::function<void(std::shared_ptr<pipeline_fpga_table>)> m_CtrlProcess;
 public Q_SLOTS:
  void onRecvPipelineTissueData(std::shared_ptr<std::vector<std::uint8_t>> data);
  void onRecvPipelineColorData(std::shared_ptr<std::vector<std::uint8_t>> data);
  void onRecvPipelinePWData(std::shared_ptr<std::vector<std::uint8_t>> data);
  void onSyncDevCtrlTable(void *table);
 Q_SIGNALS:
  void syncDevCtrlTable(void *table);
};

#endif // PIPELINEDOMSINGLETON_H
