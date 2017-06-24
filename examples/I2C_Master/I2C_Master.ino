/*
###############################################################################
# Copyright (c) 2017, PulseRain Technology LLC 
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License (LGPL) as 
# published by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
# or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################
*/

#include "M10I2C.h"

//============================================================================
// Example:
//     I2C master operation. You can verify the I2C transactions by using
// an I2C host adapter, such as the Aardvark adapter from Total Phase
//============================================================================

// I2C master address
uint8_t i2c_master_addr = 0x35;

//============================================================================
// setup()
//============================================================================

void setup() 
{
  Serial.begin (115200);
  delay(1000);
} // End of setup()

//============================================================================
// loop()
//============================================================================

uint8_t start_value = 0;
void loop() 
{
  uint8_t i, t, data[10] = {0};
  
  for (i = 0; i < 10; ++i) {
    data[i] = start_value + i + 1;
  } // End of for loop
  ++start_value;
  
  data[0] = 0x8e;
  
  Serial.write ("loop: I2C master device\n");
  I2C.masterWrite(i2c_master_addr, data, 10);
  I2C.masterRead (i2c_master_addr, 0,  data, 8);
   
  for (i = 0; i < 10; ++i) {
    Serial.write ("i = ");
    Serial.print (i);
    Serial.write (" data= ");
    Serial.println(data[i], HEX);
  } // End of for loop
  
  Serial.write("\n");
  delay (1000);

} // End of loop()

