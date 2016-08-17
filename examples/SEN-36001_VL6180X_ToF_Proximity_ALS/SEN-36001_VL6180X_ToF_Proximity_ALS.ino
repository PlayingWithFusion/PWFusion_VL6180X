/***************************************************************************
* File Name: SEN-36001_VL6180X_ToF_Proximity_ALS.ino
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
*
* **************************************************************************
* APPLICATION SPECIFIC NOTES (READ THIS!!!):
* The VL6180X is a unique Time-of-Flight proximity sensor that uses a Class 1
* IR laser. The range of the sensor is 0-200mm. The VL6180X also contains a
* built-in ambient light sensor. It is used in many cell phone and motion
* gesture applications.
* - This code configures an Arduino to interface with a VL6180X from ST Microelectronics
*    - Configure Arduino (I2C, UART, etc)
*    - Perform initial verification and configuration of the VL6180X
*    - read status/info from sensor
*    - Write formatted information to serial port
* - I2C specific note: This example uses the I2C interface via the I2C lib, not
*   the 'Wire' lib included with the Arduino IDE.
* 
* Circuit:
*    Arduino Uno   Arduino Mega  -->  SEN-36001: VL6180X Breakout
*    SDA:    SDA        SDA      -->  SDA
*    SCL:    SCL        SCL      -->  SCL
*    GND:    GND        ''       -->  GND
*    5V:     5V         ''       -->  VDD 
*    GPIO0/1 are not used in this example. They could be used for interrupts
*    (note: VDD should match voltage of IO, and can be between 3.3 and 5V)
**************************************************************************/
// The VL6180X communicates via I2C. 
// This example uses the I2C interface via the I2C lib, not Wire lib
#include "I2C.h"
// include Playing With Fusion VL6180X libraries
#include <PWFusion_VL6180X.h>
#include <PWFusion_VL6180X_STRUCT.h>

// declare sensor object (see PWFusion_VL6180X.h file for definitions)
PWFusion_VL6180X vl6180x_snsr0(0x29);

// declare sensor structure
struct var_vl6180x vl6180x_str_ch0;

void setup()
{
  struct var_vl6180x *vl6180x_ptr;
  vl6180x_ptr = &vl6180x_str_ch0;
  
  Serial.begin(115200);
  Serial.println("Playing With Fusion: VL6180X Time-of-Flight / ALS Sensor, SEN-36001");

  delay(500);
  // need to start I2C comms
  I2c.begin();

  // give the Arduino time to start up
  delay(1000);
  // setup for the proximity sensor
  // see PWFusion_VL6180X.h file for definitions
  vl6180x_snsr0.Init(vl6180x_ptr, ALS_GAIN_1);
  /* Gain settings
  ALS_GAIN_20 = 0x40,   // actual gain 20
  ALS_GAIN_10 = 0x41,   // actual gain 10.32
  ALS_GAIN_5 = 0x42,    // actual gain 5.21
  ALS_GAIN_2_5 = 0x43,  // actual gain 2.60
  ALS_GAIN_1_67 = 0x44, // actual gain 1.72
  ALS_GAIN_1_25 = 0x45, // actual gain 1.28
  ALS_GAIN_1 = 0x46,    // actual gain 1.01
  ALS_GAIN_40 = 0x47,   // actual gain 40
  */
}

void loop()
{
  delay(500);
  struct var_vl6180x *vl6180x_ptr;
  vl6180x_ptr = &vl6180x_str_ch0;

  vl6180x_snsr0.updateDistance(vl6180x_ptr);
  delay(100);
  vl6180x_snsr0.updateAmbLight(vl6180x_ptr, ALS_GAIN_1);

  Serial.print("Distance, mm: ");
  Serial.println(vl6180x_ptr->dist_val);
  Serial.print("Ambient Light, LUX: ");
  Serial.println(vl6180x_ptr->LUX_val);

}

