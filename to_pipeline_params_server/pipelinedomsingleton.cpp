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

#include "pipelinedomsingleton.h"

PipelineEngineSingleton::PipelineEngineSingleton(QObject *parent) :
    QObject(parent),
    m_CtrlProcess(nullptr) {}

void PipelineEngineSingleton::onRecvPipelineTissueData(std::shared_ptr<std::vector<std::uint8_t>> data) {
  for (size_t i = 0; i < m_FuncProcesses.size(); i++) {
    m_FuncProcesses[i](data);
  }
}
void PipelineEngineSingleton::onRecvPipelineColorData(std::shared_ptr<std::vector<std::uint8_t>> data) {
  for (size_t i = 0; i < m_FuncProcesses.size(); i++) {
    m_CFuncProcesses[i](data);
  }
}
void PipelineEngineSingleton::onRecvPipelinePWData(std::shared_ptr<std::vector<std::uint8_t>> data) {
  for (size_t i = 0; i < m_FuncProcesses.size(); i++) {
    m_PWFuncProcesses[i](data);
  }
}

void PipelineEngineSingleton::setRecvCallback(std::function<void(std::shared_ptr<std::vector<std::uint8_t>>)> func) {
  m_FuncProcesses.push_back(func);
}
void PipelineEngineSingleton::setCRecvCallback(data_func func) {
  m_CFuncProcesses.push_back(func);
}
void PipelineEngineSingleton::setPWRecvCallback(data_func func) {
  m_PWFuncProcesses.push_back(func);
}

void PipelineEngineSingleton::removeCallback(size_t index) {
  if (index >= m_FuncProcesses.size()) {
    return;
  }
  m_FuncProcesses.erase(m_FuncProcesses.begin()+index);
}
void PipelineEngineSingleton::removeCCallback(size_t index) {
  if (index >= m_CFuncProcesses.size()) {
    return;
  }
  m_CFuncProcesses.erase(m_CFuncProcesses.begin()+index);
}
void PipelineEngineSingleton::removePWCallback(size_t index) {
  if (index >= m_PWFuncProcesses.size()) {
    return;
  }
  m_PWFuncProcesses.erase(m_PWFuncProcesses.begin()+index);
}

void PipelineEngineSingleton::setRecvCtrlTbCallback(std::function<void(std::shared_ptr<pipeline_fpga_table>)> func) {
  if (m_CtrlProcess) {
    qDebug() <<  __FUNCTION__ << "m_CtrlProcess already exist.";
    return;
  }
  m_CtrlProcess = func;
}
void PipelineEngineSingleton::onSyncDevCtrlTable(void *table) {
  if (m_CtrlProcess != nullptr) {
      auto tmp = (pipeline_fpga_table *)table;
      auto tmp_o = *tmp;
      m_CtrlProcess(std::make_shared<pipeline_fpga_table>(tmp_o));
  }
}
