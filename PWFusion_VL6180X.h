/***************************************************************************
* File Name: PWFusion_VL6180X.h
* Processor/Platform: Arduino Uno R3 (tested)
* Development Environment: Arduino 1.6.5
*
* Designed for use with with Playing With Fusion VL6180X Time of Flight
* and Ambient Light Sensor Breakout: SEN-36001
*
*   SEN-36001 (universal applications)
*   ---> http://www.playingwithfusion.com/productview.php?pdid=78
*
* Copyright © 2016 Playing With Fusion, Inc.
* SOFTWARE LICENSE AGREEMENT: This code is released under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
* **************************************************************************
* REVISION HISTORY:
* Author        Date      Comments
* J. Steinlage  2016May   Original version
* 
* Playing With Fusion, Inc. invests time and resources developing open-source
* code. Please support Playing With Fusion and continued open-source 
* development by buying products from Playing With Fusion!
**************************************************************************/

#ifndef PWFusion_VL6180X_H
#define PWFusion_VL6180X_H

#include "Arduino.h"
#include "I2C.h"
#include "PWFusion_VL6180X_STRUCT.h"

#define VL6180X_ID_MODEL_ID					0x0000
#define VL6180X_ID_MODEL_REV_MAJ			0x0001
#define VL6180X_ID_MODEL_REV_MIN			0x0002
#define VL6180X_ID_MODULE_REV_MAJ			0x0003
#define VL6180X_ID_MODULE_REV_MIN			0x0004
#define VL6180X_ID_DATE_HIGH				0x0006
#define VL6180X_ID_DATE_LO					0x0007
#define VL6180X_ID_TIME_SINCE_MIDNIGHT		0x0008	/*2 byte*/
#define VL6180X_SYS_MODE_GPIO0				0x0010
#define VL6180X_SYS_MODE_GPIO1				0x0011
#define VL6180X_SYS_HISTORY_CTRL			0x0012
#define VL6180X_SYS_INT_CONFIG_GPIO			0x0014
#define VL6180X_SYS_INT_CLEAR				0x0015
#define VL6180X_SYS_FRESH_OUT_OF_RESET		0x0016
#define VL6180X_SYS_GROUPED_PARAM_HOLD		0x0017
#define VL6180X_SYSRANGE_START				0x0018
#define VL6180X_SYSRANGE_THRESH_HIGH		0x0019
#define VL6180X_SYSRANGE_THRESH_LOW			0x001A
#define VL6180X_SYSRANGE_INTRMEAS_PERIOD	0x001B
#define VL6180X_SYSRANGE_MAX_CONVG_TIME		0x001C
#define VL6180X_SYSRANGE_XTALK_COMP_RT		0x001E	/*2 byte*/
#define VL6180X_SYSRANGE_XTALK_VALID_HGHT	0x0021
#define VL6180X_SYSRANGE_EARLY_CONV_EST		0x0022	/*2 byte*/
#define VL6180X_SYSRANGE_PT2PT_RNG_OFFSET	0x0024
#define VL6180X_SYSRANGE_RNG_IGNORE_VAL_HT	0x0025
#define VL6180X_SYSRANGE_RNG_IGNORE_THRESH	0x0026	/*2 byte*/
#define VL6180X_SYSRANGE_MAX_AMB_LVL_MULT	0x002C
#define VL6180X_SYSRANGE_RNG_CHK_ENABLES	0x002D
#define VL6180X_SYSRANGE_VHV_RECAL			0x002E
#define VL6180X_SYSRANGE_VHV_REPEAT_RT		0x0031
#define VL6180X_SYSALS_START				0x0038
#define VL6180X_SYSALS_THRESH_HIGH			0x003A	/*2 byte*/
#define VL6180X_SYSALS_THRESH_LOW			0x003C	/*2 byte*/
#define VL6180X_SYSALS_INTRMEAS_PERIOD		0x003E
#define VL6180X_SYSALS_ANALOG_GAIN			0x003F
#define VL6180X_SYSALS_INTEGRATION_PERIOD	0x0040	/*2 byte*/
#define VL6180X_RESULT_RANGE_STATUS			0x004D
#define VL6180X_RESULT_ALS_STATUS			0x004E
#define VL6180X_RESULT_INT_STATUS_GPIO		0x004F
#define VL6180X_RESULT_ALS_VAL				0x0050	/*2 byte*/
#define VL6180X_RESULT_HISTORY_BUFF_X		0x0052	/*2 byte - see datasheet for info (pg 63)*/
#define VL6180X_RESULT_RANGE_VAL			0x0062
#define VL6180X_RESULT_RANGE_RAW			0x0064
#define VL6180X_RESULT_RANGE_RET_RATE		0x0066	/*2 byte*/
#define VL6180X_RESULT_RANGE_REF_RATE		0x0068	/*2 byte*/
#define VL6180X_RESULT_RANGE_RET_SIG_CNT	0x006C	/*4 byte*/
#define VL6180X_RESULT_RANGE_REF_SIG_CNT	0x0070	/*4 byte*/
#define VL6180X_RESULT_RANGE_RET_AMB_CNT	0x0074	/*4 byte*/
#define VL6180X_RESULT_RANGE_REF_AMB_CNT	0x0078	/*4 byte*/
#define VL6180X_RESULT_RANGE_RET_CONV_TIME	0x007C	/*4 byte*/
#define VL6180X_RESULT_RANGE_REF_CONV_TIME	0x0080	/*4 byte*/
#define VL6180X_READOUT_AVG_SAMP_PERIOD		0x010A
#define VL6180X_FIRMWARE_BOOTUP				0x0119
#define VL6180X_FIRMWARE_RESULT_SCALAR		0x0120
#define VL6180X_I2C_SLAVE_DEV_ADD			0x0212
#define VL6180X_INTERLEAVED_MODE_EN			0x02A3

// ALS sensitivity: 0.32 LUX/coutnt (typical), see datasheet, table 14
enum vl6180x_als_gain{
ALS_GAIN_20 = 0x40,		// actual gain 20
ALS_GAIN_10 = 0x41,		// actual gain 10.32
ALS_GAIN_5 = 0x42,		// actual gain 5.21
ALS_GAIN_2_5 = 0x43,	// actual gain 2.60
ALS_GAIN_1_67 = 0x44,	// actual gain 1.72
ALS_GAIN_1_25 = 0x45,	// actual gain 1.28
ALS_GAIN_1 = 0x46,		// actual gain 1.01
ALS_GAIN_40 = 0x47,		// actual gain 40
};

class PWFusion_VL6180X
{
public:	
  PWFusion_VL6180X(uint8_t address);
  void Init(struct var_vl6180x *prx_str, vl6180x_als_gain ALS_GAIN);
  void updateDistance(struct var_vl6180x *prx_str);
  void updateAmbLight(struct var_vl6180x *prx_str, vl6180x_als_gain ALS_GAIN);
  // change address function excluded as it doesn't last over power cycle
  
private:
  uint8_t _dev_add;
  uint8_t read_reg_byte(uint16_t RegAdd);
  uint16_t read_reg_2byte(uint16_t RegAdd);
  void write_reg_byte(uint16_t RegAdd, uint8_t RegData);
  void write_reg_2byte(uint16_t RegAdd, uint16_t RegData);

};
#endif // PWFusion_VL6180X_H
