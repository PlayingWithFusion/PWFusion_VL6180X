/***************************************************************************
* File Name: PWFusion_VL6180X.cpp
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
#include "PWFusion_VL6180X.h"

PWFusion_VL6180X::PWFusion_VL6180X(uint8_t address)
{
	// device address... default is 0x29, can be changed with SW, saved in NVM
	_dev_add = address;
}

void PWFusion_VL6180X::Init(struct var_vl6180x *prx_str, vl6180x_als_gain ALS_GAIN)
{
	// use this function at power up or when GPIO-0 is toggled
	// check system out of reset register (optional), write 0x00 to this register (power up should be 0x01)
	// then load settings (section 9 of appnote)
	// apply other specific settings (handled in config function)
	// ready to start range measurmenet
	// Configure private registers as recommended in "SR03 Settings": AN4545, Section 9
	uint8_t reset, chip_ID, temp, day, month, year;
	uint16_t temp_16;
	reset = read_reg_byte(VL6180X_SYS_FRESH_OUT_OF_RESET);
	chip_ID = read_reg_byte(VL6180X_ID_MODEL_ID);
	
	// Verify VL1680X is connected - read ID register
	if(0xB4 == chip_ID)
	{
		// found VL6180X
		prx_str->IC_found = 1;	// 1 = found, 0 = not found
		Serial.println("VL6180X Found!");
		// Get model ID information
		Serial.print("Model Major Rev: ");
		temp = read_reg_byte(VL6180X_ID_MODEL_REV_MAJ) & 0x07;	// get IC major revision
		Serial.print(temp);										// print major revision
		prx_str->ID_ModelRevMaj = temp;							// save info to ch-specific struct
		Serial.print("; Minor Rev: ");
		temp = read_reg_byte(VL6180X_ID_MODEL_REV_MIN) & 0x07;	// get IC minor revision
		Serial.println(temp);
		prx_str->ID_ModelRevMin = temp;							// save info to ch-specific struct
		Serial.print("Module Major Rev: ");
		temp = read_reg_byte(VL6180X_ID_MODULE_REV_MAJ) & 0x07;	// get IC module rev major
		Serial.print(temp);										// print module major revision
		prx_str->ID_ModuleRevMaj = temp;						// save info to ch-specific struct
		Serial.print("; Minor Rev: ");
		temp = read_reg_byte(VL6180X_ID_MODULE_REV_MIN) & 0x07;	// get IC module rev minor
		Serial.println(temp);									// print module minor revision
		prx_str->ID_ModuleRevMin = temp;						// save info to ch-specific struct
		// manufacture date (can be used as ID, since it logs down to the second)
		Serial.print("IC Manuf. Date: 201");
		temp = read_reg_byte(VL6180X_ID_DATE_HIGH);				// read high date byte
		year = (temp>>4) & 0x0F;								// calculate year (last digit of year)
		month = temp & 0x0F;									// calculate month
		temp = read_reg_byte(VL6180X_ID_DATE_LO);				// read lo date byte
		day = (temp>>3) & 0x1F;									// calculate day of month
		Serial.print(year);										// print year
		Serial.print("/");										// print format "/"
		Serial.print(month);									// print month
		Serial.print("/");										// print format "/"
		Serial.print(day);									// print day of month
		temp_16 = read_reg_2byte(VL6180X_ID_TIME_SINCE_MIDNIGHT);
		Serial.print(", ");
		Serial.print(temp_16);
		Serial.println(" seconds");
		// save date info to ch-specific struct
		prx_str->ID_year = year;
		prx_str->ID_month = month;
		prx_str->ID_day = day;	
		prx_str->ID_seconds = temp_16;
		
		// now start initialization process
		Serial.println("POR OK, initializing chip...");
		// load settings based on application note (mandatory/private registers)
		// based on AN4545, pg 24/27 (rev 1) and 'Design Tip DT0037'
		write_reg_byte(0x0207, 0x01);
		write_reg_byte(0x0208, 0x01);
		write_reg_byte(0x0133, 0x01);
		write_reg_byte(0x0096, 0x00);
		write_reg_byte(0x0097, 0xfd);
		write_reg_byte(0x00e3, 0x00);
		write_reg_byte(0x00e4, 0x04);
		write_reg_byte(0x00e5, 0x02);
		write_reg_byte(0x00e6, 0x01);
		write_reg_byte(0x00e7, 0x03);
		write_reg_byte(0x00f5, 0x02);
		write_reg_byte(0x00d9, 0x05);
		write_reg_byte(0x00db, 0xce);
		write_reg_byte(0x00dc, 0x03);
		write_reg_byte(0x00dd, 0xf8);
		write_reg_byte(0x009f, 0x00);
		write_reg_byte(0x00a3, 0x3c);
		write_reg_byte(0x00b7, 0x00);
		write_reg_byte(0x00bb, 0x3c);
		write_reg_byte(0x00b2, 0x09);
		write_reg_byte(0x00ca, 0x09);
		write_reg_byte(0x0198, 0x01);
		write_reg_byte(0x01b0, 0x17);
		write_reg_byte(0x01ad, 0x00);
		write_reg_byte(0x00ff, 0x05);
		write_reg_byte(0x0100, 0x05);
		write_reg_byte(0x0199, 0x05);
		write_reg_byte(0x0109, 0x07);
		write_reg_byte(0x010a, 0x30);
		write_reg_byte(0x003f, 0x46);
		write_reg_byte(0x01a6, 0x1b);
		write_reg_byte(0x01ac, 0x3e);
		write_reg_byte(0x01a7, 0x1f);
		write_reg_byte(0x0103, 0x01);
		write_reg_byte(0x0030, 0x00);
		write_reg_byte(0x001b, 0x0A);
		write_reg_byte(0x003e, 0x0A);
		write_reg_byte(0x0131, 0x04);
		write_reg_byte(0x0011, 0x10);
		write_reg_byte(0x0014, 0x24);
		write_reg_byte(0x0031, 0xFF);
		write_reg_byte(0x00d2, 0x01);
		write_reg_byte(0x00f2, 0x01);
		write_reg_byte(VL6180X_SYS_FRESH_OUT_OF_RESET, 0x00);	// clear fresh out of reset byte
		
		/**** Set public registers, again, based on AN4545 (pg 24-25/27) ****/
		Serial.println("Init complete, loading user values...");
		// enables polling for new sample when meas completes
		write_reg_byte(VL6180X_SYS_MODE_GPIO1, 0x10);	// sets GPIO1 high when complete
		// set averaging samp period, compromise between noise and execution time.
		write_reg_byte(VL6180X_READOUT_AVG_SAMP_PERIOD, 0x30);	
		// set light and dark gain (upper nibble, dark gain shouldn't be changed).
		write_reg_byte(VL6180X_SYSALS_ANALOG_GAIN, ALS_GAIN);	
		// sets # of range measurements after which auto-cal of system is performed.
		write_reg_byte(VL6180X_SYSRANGE_VHV_REPEAT_RT, 0xFF);	
		// set ALS integration time to 100ms
		write_reg_2byte(VL6180X_SYSALS_INTEGRATION_PERIOD, 0x0064);	
		// perform single temperature calibration of the ranging sensor
		write_reg_byte(VL6180X_SYSRANGE_VHV_RECAL, 0x01);	
		
		/**** Set optional public registers, again, based on AN4545 (pg 25/27) ****/
		// set default ranging inter-measurement register to 100ms
		write_reg_byte(VL6180X_SYSRANGE_INTRMEAS_PERIOD, 0x09);
		// set default ALS inter-measurement register to 500ms
		write_reg_byte(VL6180X_SYSALS_INTRMEAS_PERIOD, 0x32);
		// configures interrupt on 'new sample ready threshold events'
		write_reg_byte(VL6180X_SYS_INT_CONFIG_GPIO, 0x24);
		
		/**** Additional public registers, AN4545, Section 6 ****/
		// sets maximum time a measurement is allowed to complete, recommended 30ms as starting point
		// if convergence hasn't been reached, 255 will be reported as range
		write_reg_byte(VL6180X_SYSRANGE_MAX_CONVG_TIME, 0x1E); // x31 is POR value
		// enable additional system ranging checks
		write_reg_byte(VL6180X_SYSRANGE_RNG_CHK_ENABLES, 0x11); // enable sig 2 noise and early convergence
		// 10ms/bit, must be = max convg time + 5ms
		write_reg_byte(VL6180X_SYSRANGE_INTRMEAS_PERIOD, 0x03); // set to 40ms
		// ECE threshold based on datasheet, pg 22/79
		write_reg_2byte(VL6180X_SYSRANGE_EARLY_CONV_EST, 0x0080); // 50% of convergence threshold at 30ms
		// set result scaling (1x per bit)
		write_reg_byte(VL6180X_FIRMWARE_RESULT_SCALAR, 0x01); // set to 1x scaling
		
		/**** Further additional settings, AN4545, Section 5 ****/
		// crosstalk compensation can be added for cover glass, etc
		// See section 5, page 17/27 in AN4545 for details
		
		Serial.println("VL6180X Registers loaded, initialization complete!");
		prx_str->chip_init = 1;	// 1 = init OK, 0 = init not OK

	}
	else
	{
		prx_str->IC_found = 0;	// 1 = found, 0 = not found
		prx_str->chip_init = 0;	// 1 = init OK, 0 = init not oK
		Serial.println("VL6180X not found");
		return;
	}
}

void PWFusion_VL6180X::updateDistance(struct var_vl6180x *prx_str)
{
	write_reg_byte(VL6180X_SYSRANGE_START, 0x01);					// using single shot mode
	while(!(read_reg_byte(VL6180X_RESULT_INT_STATUS_GPIO) | 0xFB));	// wait for result to complete
	write_reg_byte(VL6180X_SYS_INT_CLEAR, 0x07);					// clear all interrupts (dist, als, error)
	prx_str->dist_val = read_reg_byte(VL6180X_RESULT_RANGE_VAL);	// save distance result to struct
}

void PWFusion_VL6180X::updateAmbLight(struct var_vl6180x *prx_str, vl6180x_als_gain ALS_GAIN)
{
	uint16_t als_val, als_int_per;
	float temp_fl, als_gain;
	// set light and dark gain (upper nibble, dark gain shouldn't be changed), if desired
	write_reg_byte(VL6180X_SYSALS_ANALOG_GAIN, ALS_GAIN);
	write_reg_byte(VL6180X_SYSALS_START, 0x01);						// using single shot mode
	delay(200);
//	while(!(read_reg_byte(VL6180X_RESULT_INT_STATUS_GPIO) | 0xDF));	// wait for result to complete
	write_reg_byte(VL6180X_SYS_INT_CLEAR, 0x07);					// clear all interrupts (dist, als, error)
	
	// read 2-byte raw value register
	als_val = read_reg_2byte(VL6180X_RESULT_ALS_VAL);
	//  read integration period (in milliseconds)
	als_int_per = read_reg_2byte(VL6180X_SYSALS_INTEGRATION_PERIOD);
	// look up gain
	switch(ALS_GAIN){
		case ALS_GAIN_20: 	als_gain = 20.0; 	break;
		case ALS_GAIN_10: 	als_gain = 10.32; 	break;
		case ALS_GAIN_5: 	als_gain = 5.21;	break;
		case ALS_GAIN_2_5: 	als_gain = 2.60; 	break;
		case ALS_GAIN_1_67:	als_gain = 1.72; 	break;
		case ALS_GAIN_1_25: als_gain = 1.28; 	break;
		case ALS_GAIN_1: 	als_gain = 1.01; 	break;
		case ALS_GAIN_40: 	als_gain = 40.0; 	break;
	}
	// calculate LUX with raw value, gain and integration period
	// based on ALS measurement example... LUX = als_val * ALS cal value * 100 /(gain * int time)
	//                                         = als_val * 32 / (gain * int time)
	temp_fl = (float)als_val * (float)32 / (als_gain * (float)als_int_per);
	prx_str->LUX_val = temp_fl;
}

uint8_t PWFusion_VL6180X::read_reg_byte(uint16_t RegAdd)
{
	uint8_t data;
	I2c.write(_dev_add, ((RegAdd >> 8) & 0xFF), (RegAdd & 0xFF)); 	// write register address to chip
	I2c.read(_dev_add, (uint8_t)1);					// read 1 byte
	data = I2c.receive();
	return data;
}

uint16_t PWFusion_VL6180X::read_reg_2byte(uint16_t RegAdd)
{
	uint8_t data_arr[2];
	uint16_t data;
	I2c.write(_dev_add, ((RegAdd >> 8) & 0xFF), (RegAdd & 0xFF)); 	// write register address to chip
	I2c.read(_dev_add, (uint8_t)2, data_arr);						// read 2 bytes
	data = (data_arr[0] << 8) | data_arr[1];
	return data;
}

void PWFusion_VL6180X::write_reg_byte(uint16_t RegAdd, uint8_t RegData)
{
	uint8_t write_data[2];
	uint8_t reg1 = (RegAdd >> 8) & 0xFF;		// MSB of address
	write_data[0] = (RegAdd & 0xFF);			// LSB of address
	write_data[1] = (RegData & 0xFF);			// data to write
	I2c.write(_dev_add, reg1, write_data, 2);	// write register data
}

void PWFusion_VL6180X::write_reg_2byte(uint16_t RegAdd, uint16_t RegData)
{
	uint8_t write_data[3];
	uint8_t reg1 = (RegAdd >> 8) & 0xFF;		// MSB of address
	write_data[0] = RegAdd & 0xFF;				// LSB of address
	write_data[1] = (RegData >> 8) & 0xFF;		// MSB of data to write
	write_data[2] = RegData & 0xFF;				// LSB of data to write
	I2c.write(_dev_add, reg1, write_data, 3);	// write register data
}