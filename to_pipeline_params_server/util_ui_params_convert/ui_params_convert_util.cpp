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


#include "ui_params_convert_util.h"

const float probe_angle  = 59.5f; //degree
const float probe_inner_radius  = 6.0f; //cm

ParamsConvertUtil::ParamsConvertUtil(QObject *):
_is_active(false) {
}

void ParamsConvertUtil::active(configuru::Config & cfg) {
  if (_is_active) return;
  _active_mutex.lock();
  // add convert things here.
  convert_lined_params();
  cfg.judge_with_create_key("dev_sync_tag")["probe_angle"] = probe_angle;
  cfg["dev_sync_tag"]["probe_inner_radius"] = probe_inner_radius;
  _is_active = true;
  _active_mutex.unlock();
}

void ParamsConvertUtil::convert_lined_params() {
  // todo
}
