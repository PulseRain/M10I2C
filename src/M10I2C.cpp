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

//----------------------------------------------------------------------------
// _I2C_wait()
//
// Parameters:
//      None
//
// Return Value:
//      return 0 if it runs ok. Otherwise it will return the CSR value
//
// Remarks:
//      Function to wait for I2C_MASTER_DATA_REQ. Internal use only 
//----------------------------------------------------------------------------

static uint8_t _I2C_wait()
{
  uint8_t t;

   do {
        t = I2C_CSR;
        nop_delay(2);
   } while (!(t & (I2C_MASTER_DATA_REQ | I2C_MASTER_IDLE_FLAG)));

        
   if ((t & I2C_MASTER_IDLE_FLAG) || (t & I2C_MASTER_NO_ACK_FLAG)) {
        return t;
   }

   return 0;
} // End of _I2C_wait()


//----------------------------------------------------------------------------
// I2C_Master_Write()
//
// Parameters:
//      addr        : I2C address, 7 bit or 10 bit
//      buf         : pointer to data buffer that will be sent to slave
//      buf_size    : buffer size in bytes
//
// Return Value:
//      return 0 if it runs ok. Otherwise it will return the CSR value
//
// Remarks:
//      Function to write data to I2C slave 
//----------------------------------------------------------------------------

static uint8_t I2C_Master_Write(uint16_t addr, uint8_t buf[], uint8_t buf_size) __reentrant
{
   uint8_t t, i;

   EA = 0;
   
   I2C_CSR = I2C_CSR_MASTER | I2C_CSR_WRITE | I2C_CSR_SYNC_RESET;
   nop_delay(2);

   if (addr > 127) {
      t = (uint8_t)(addr >> 7) & 0x3;
      t += I2C_TEN_BIT_HEADER * 4;
      t <<= 1;

      I2C_ADDR_DATA = t;
      I2C_CSR = I2C_CSR_START | I2C_CSR_WRITE | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
   
      if (t = _I2C_wait()) {
        EA = 1;
        return t;
      }
   }

   t = ((uint8_t)addr) & 127;
   t <<= 1;
   
   I2C_ADDR_DATA = t;

   if (addr <= 127) {
      I2C_CSR = I2C_CSR_START | I2C_CSR_WRITE | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
      nop_delay(2);
   }
   
   for (i = 0; i < buf_size; ++i) {
     
     if (t = _I2C_wait()) {
        EA = 1;
        return t;
      }

     I2C_ADDR_DATA = buf[i];
     nop_delay(2);

   } // End of for loop   

   do {
        t = I2C_CSR;
        nop_delay(2);
   } while (!(t & (I2C_MASTER_DATA_REQ | I2C_MASTER_IDLE_FLAG)));
  
   I2C_CSR = I2C_CSR_STOP | I2C_CSR_WRITE | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
   nop_delay(2);
   
   do {
        t = I2C_CSR;
        nop_delay(2);
   } while (!(t & (I2C_MASTER_IDLE_FLAG)));

     
   EA = 1;
   return 0;
} // End of I2C_Master_Write()

//----------------------------------------------------------------------------
// I2C_Master_Read()
//
// Parameters:
//      i2c_addr    : I2C address, 7 bit or 10 bit
//      reg_addr    : address for the register to be read from the slave device
//      buf         : data buffer to be filled
//      buf_size    : read length in bytes
//
// Return Value:
//      return 0 if it runs ok. Otherwise it will return the CSR value
//
// Remarks:
//      Function to read data from I2C slave 
//----------------------------------------------------------------------------

static uint8_t I2C_Master_Read(uint16_t i2c_addr, uint8_t reg_addr, uint8_t buf[], uint8_t buf_size) __reentrant
{
   uint8_t t, i;

   EA = 0;
   
   I2C_CSR = I2C_CSR_MASTER | I2C_CSR_WRITE | I2C_CSR_SYNC_RESET;
   nop_delay(2);

   if (i2c_addr > 127) {
      t = (uint8_t)(i2c_addr >> 7) & 0x3;
      t += I2C_TEN_BIT_HEADER * 4;
      t <<= 1;

      I2C_ADDR_DATA = t;
      I2C_CSR = I2C_CSR_START | I2C_CSR_WRITE | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;

      if (t = _I2C_wait()) {
          EA = 1;
          return t;
      }
   }

   t = ((uint8_t)i2c_addr) & 127;
   t <<= 1;
   
   I2C_ADDR_DATA = t;

   if (i2c_addr <= 127) {
      I2C_CSR = I2C_CSR_START | I2C_CSR_WRITE | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
      nop_delay(2);
   }

   if (t = _I2C_wait()) {
        EA = 1;
        return t;
   }
   
   I2C_ADDR_DATA = reg_addr;

   if (t = _I2C_wait()) {
        EA = 1;
        return t;
   }

   if (i2c_addr > 127) {
      t = (uint8_t)(i2c_addr >> 7) & 0x3;
      t += I2C_TEN_BIT_HEADER * 4;
      t <<= 1;

      I2C_ADDR_DATA = t;
      nop_delay(2);
   
      I2C_CSR = I2C_CSR_RESTART | I2C_CSR_STOP | I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;

      do {
        t = I2C_CSR;
        nop_delay(2);
      } while (!(t & (I2C_MASTER_IDLE_FLAG)));
  
      I2C_CSR = I2C_CSR_START| I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;

      if (t = _I2C_wait()) {
        EA = 1;
        return t;
      }

      t = ((uint8_t)i2c_addr) & 127;
      t <<= 1;
   
      I2C_ADDR_DATA = t;
      
      if (t = _I2C_wait()) {
        EA = 1;
        return t;
      }
   
   } else {
      t = ((uint8_t)i2c_addr) & 127;
      t <<= 1;
   
      I2C_ADDR_DATA = t;

      I2C_CSR = I2C_CSR_RESTART | I2C_CSR_STOP | I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;

      do {
        t = I2C_CSR;
        nop_delay(2);
      } while (!(t & (I2C_MASTER_IDLE_FLAG)));
  
      I2C_CSR = I2C_CSR_START| I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;

      if (t = _I2C_wait()) {
        EA = 1;
        return t;
      }
   }
   
   for (i = 0; i < buf_size; ++i) {
     
     do {
        t = I2C_CSR;
        nop_delay(2);
     } while (!(t & (I2C_MASTER_DATA_READY | I2C_MASTER_IDLE_FLAG)));

     if ((i + 1) == buf_size) {
        I2C_CSR = I2C_CSR_STOP | I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
     }
     
     if ((t & I2C_MASTER_IDLE_FLAG) || (t & I2C_MASTER_NO_ACK_FLAG)) {
        EA = 1;
        return t;
     }

     buf[i] = I2C_ADDR_DATA;
     I2C_ADDR_DATA = 0;
     nop_delay(2);

   } // End of for loop   

  //== I2C_CSR = I2C_CSR_STOP | I2C_CSR_READ | I2C_CSR_MASTER | I2C_CSR_IRQ_DISABLE;
  //== nop_delay(2);
   
   do {
        t = I2C_CSR;
        nop_delay(2);
   } while (!(t & (I2C_MASTER_IDLE_FLAG)));

     
   EA = 1;
   return 0;
} // End of I2C_Master_Read()


//----------------------------------------------------------------------------
// I2C_Slave()
//
// Parameters:
//      addr             : I2C address, 7 bit
//      call_back_func   : call back function to be registered
//
// Return Value:
//      None
//
// Remarks:
//      Function to register a call back function for I2C slave device
//----------------------------------------------------------------------------

static void I2C_Slave(uint8_t addr, void (*call_back_func)(void)) __reentrant
{
   EA = 0;
   
   I2C_CSR = I2C_CSR_SLAVE | I2C_CSR_WRITE | I2C_CSR_SYNC_RESET;
   nop_delay(2);

   I2C_ADDR_DATA = addr;
   attachIsrHandler(INT1_I2C_INT_INDEX, call_back_func);
   I2C_CSR = I2C_CSR_START | I2C_CSR_WRITE | I2C_CSR_SLAVE | I2C_CSR_IRQ_ENABLE;

   EA = 1;
 } // End of I2C_Slave()

const I2C_STRUCT I2C = {
  .masterWrite  = I2C_Master_Write,
  .masterRead   = I2C_Master_Read,
  .slave = (I2C_SLAVE_FUNC)I2C_Slave
};
