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
//     I2C slave operation. You can verify the I2C transactions by using
// an I2C host adapter, such as the Aardvark adapter from Total Phase
//============================================================================

// I2C slave address
uint8_t i2c_slave_addr = 0x29;


// circular buffer for data input from master
uint8_t i2c_slave_buffer [256];
uint8_t slave_buffer_read_pointer;
uint8_t slave_buffer_write_pointer;


// data out, starting value
uint8_t data_out = 0x66;


//----------------------------------------------------------------------------
// i2c_isr_handler()
//
// Parameters:
//      None
//
// Return Value:
//      None
//
// Remarks:
//      isr handler to be registered for I2C slave
//----------------------------------------------------------------------------
void i2c_isr_handler()
{
    uint8_t t;

    t = I2C_CSR;
    if (t & I2C_SLAVE_DATA_READY) { // Master write, data coming from master
      t = I2C_ADDR_DATA;
      i2c_slave_buffer [slave_buffer_write_pointer++] = t;
      I2C_ADDR_DATA = 0;
    } else if (t & I2C_SLAVE_DATA_REQ) { // Master read, data request from master
      I2C_ADDR_DATA = data_out++; 
   }
} // End of i2c_isr_handler()


//============================================================================
// setup()
//============================================================================

void setup() 
{
  Serial.begin (115200);
  delay(1000);

  slave_buffer_read_pointer = 0;
  slave_buffer_write_pointer = 0;
  
  I2C.slave (i2c_slave_addr, i2c_isr_handler);
  
} // End of setup()

//============================================================================
// loop()
//============================================================================

void loop() 
{
  uint8_t i;
  
  Serial.write ("loop: I2C slave device\n");
  
  for (i = slave_buffer_read_pointer; i < slave_buffer_write_pointer; ++i) {
    Serial.write (" i = ");
    Serial.print (i);
    Serial.write ("receive data = ");
    Serial.println(i2c_slave_buffer[i], HEX);    
  } // End of for loop
  
  slave_buffer_read_pointer = slave_buffer_write_pointer;

  Serial.write("\n");
  delay (1000);

} // End of loop()

