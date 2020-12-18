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

#ifndef CTRLTABLEUTIL_H
#define CTRLTABLEUTIL_H

#include "configuru.hpp"

/* control table of FPGA  */
typedef struct FPGA__table {
  unsigned char blk_0 : 8;
  unsigned char ARGS_B_MULTIPLE_BEAM:3;
  unsigned char ARGS_C_MULTIPLE_BEAM:3;
  unsigned char ARGS_D_WALLFILTER:2;
  unsigned char ARGS_C_ACCUMULATION:4;
  unsigned char ARGS_B_RANGE:4;
  unsigned char ARGS_SAMPLING_INTERVAL:5;
  unsigned char ARGS_C_FILTER:2;
  unsigned char ARGS_CPA:1;
  unsigned char ARGS_BM_FREQUENCY_DIVISION_RATIO:6;
  unsigned char ARGS_WIDE_BAND:1;
  unsigned char ARGS_FREEZE:1;
  unsigned char ARGS_FOCAL_DEPTH:4;
  unsigned char ARGS_M:1;
  unsigned char ARGS_D:1;
  unsigned char ARGS_C:1;
  unsigned char ARGS_B:1;
  unsigned char ARGS_BM_RECEIVER_FREQUENCY_RATIO:6;
  unsigned char ARGS_B_HARMONIC_1:1;
  unsigned char ARGS_B_HARMONIC_2:1;
  unsigned char ARGS_C_SCAN_START:7;
  unsigned char ARGS_KEY_MODE:1;
  unsigned char ARGS_C_SCAN_END:7;
  unsigned char blk_2:1;
  unsigned char ARGS_PROBE_SOFT_CODE:3;
  unsigned char ARGS_LINEAR_DEFLECTION_ANGLE:5;
  unsigned char ARGS_B_GAIN:8;
  unsigned char ARGS_TGC_1:8;
  unsigned char ARGS_TGC_2:8;
  unsigned char ARGS_TGC_3:8;
  unsigned char ARGS_TGC_4:8;
  unsigned char ARGS_TGC_5:8;
  unsigned char ARGS_TGC_6:8;
  unsigned char ARGS_TGC_7:8;
  unsigned char ARGS_TGC_8:8;
  unsigned char ARGS_C_GAIN:8;
  unsigned char ARGS_SOUND_POWER:4;
  unsigned char ARGS_ID:2;
  unsigned char ARGS_PROBE:2;
  unsigned char ARGS_C_FACTOR:5;
  unsigned char ARGS_B_STEP:3;
  unsigned char ARGS_XCWS_LOW_8:8;
  unsigned char ARGS_XDWS_HIGH_4:4;
  unsigned char ARGS_XCWS_HIGH_4:4;
  unsigned char ARGS_XDWS_LOW_8:8;
  unsigned char ARGS_D_SAMPLING_INTERVAL:4;
  unsigned char ARGS_M_D_SPEED:3;
  unsigned char ARGS_BCD_OR_D:1;
  unsigned char ARGS_MD_LINE:7;
  unsigned char blk_3:1;
  unsigned char ARGS_C_FREQUENCY_DIVISION_RATIO:6;
  unsigned char ARGS_FREQUENCY_CURRENT:2;
  unsigned char ARGS_B_FACTOR:5;
  unsigned char ARGS_D_SAMPLING:3;
  unsigned char ARGS_SPEED_THRESHOLD:5;
  unsigned char ARGS_WEIGHT:3;
  unsigned char ARGS_RANGE_THRESHOLD:5;
  unsigned char ARGS_5805_CONFIG:3;
  unsigned char ARGS_M_GAIN:8;
  unsigned char ARGS_D_GAIN:8;
  unsigned char ARGS_D_TRANSMISSIONFREQUENCY:6;
  unsigned char blk_4:2;
  unsigned char ARGS_SERIAL_POWER:8;
  unsigned char blk_5:8;
  unsigned char ARGS_SOUND_SPEED:7;
  unsigned char ARGS_CONTRAST_MODE : 1;
  unsigned char ARGS_A_TGC_1:8;
  unsigned char ARGS_A_TGC_2:8;
  unsigned char ARGS_A_TGC_3:8;
  unsigned char ARGS_A_TGC_4:8;
  unsigned char ARGS_A_TGC_5:8;
  unsigned char ARGS_A_TGC_6:8;
  unsigned char ARGS_A_TGC_7:8;
  unsigned char ARGS_A_TGC_8:8;
  unsigned char ARGS_B_TGC_1:8;
  unsigned char ARGS_B_TGC_2:8;
  unsigned char ARGS_B_TGC_3:8;
  unsigned char ARGS_B_TGC_4:8;
  unsigned char ARGS_B_TGC_5:8;
  unsigned char ARGS_B_TGC_6:8;
  unsigned char ARGS_B_TGC_7:8;
  unsigned char ARGS_B_TGC_8:8;
  unsigned char ARGS_C_TRANSMIT_FOCUS:4;
  unsigned char ARGS_B_TRANSMIT_FOCUS:4;
  unsigned char ARGS_C_FRAME_UP:8;
  unsigned char ARGS_C_FRAME_DOWN:8;
  unsigned char blk_7:8;
  unsigned char blk_8:8;
  unsigned char ARGS_RestTime: 8;
  unsigned char ARGS_UDP_OR_TCP_NUMBER:8;
  unsigned char ARGS_RESET:1;
  unsigned char ARGS_START:1;
  unsigned char ARGS_TI:2;
  unsigned char ARGS_MOTOR_TYPE:2;
  unsigned char blk_10:2;
  unsigned char blk_11:8;
  unsigned char ARGS_GREEN_LED:1;
  unsigned char ARGS_RED_LED:1;
  unsigned char ARGS_IDLE:1;
  unsigned char ARGS_OFF:1;
  unsigned char ARGS_US_CHG:1;
  unsigned char ARGS_USB_IF:1;
  unsigned char ARGS_WIFI_IF:1;
  unsigned char ARGS_OTA:1;
  unsigned char blk_15:8;
} pipeline_fpga_table;

class CtrlTableUtil {
 public:
  CtrlTableUtil();
  configuru::Config GetFromCtrlTable(const pipeline_fpga_table &table_in);
  pipeline_fpga_table GetFromConfigTable(const pipeline_fpga_table &src, const configuru::Config &cfg_table);
  pipeline_fpga_table GetFromConfigTable(const configuru::Config &cfg_table);
};

#endif // CTRLTABLEUTIL_H
