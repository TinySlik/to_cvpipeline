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

#ifndef UI_PARAMS_CONVERT_H
#define UI_PARAMS_CONVERT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include "../configuru.hpp"
#include <mutex>
#include "ParameterServer/parameterserver.h"

class ParamsConvertUtil : public QObject {
  Q_OBJECT
  Q_DISABLE_COPY(ParamsConvertUtil)

 private:
  explicit ParamsConvertUtil(QObject *parent = nullptr);
  ~ParamsConvertUtil() {
  }
  std::mutex _active_mutex;
  bool _is_active;

 public:
  static ParamsConvertUtil *instance() {
    static ParamsConvertUtil *_this = nullptr;
    if (_this == nullptr) {
      _this = new ParamsConvertUtil;
    }
    return _this;
  }
  void active(configuru::Config & cfg);
  void convert_lined_params();
};

#endif // UI_PARAMS_CONVERT_H
