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

#ifndef M10I2C_H
#define M10I2C_H

#include "Arduino.h"

#define I2C_CSR_SYNC_RESET             1
#define I2C_CSR_START                  2
#define I2C_CSR_STOP                   0

#define I2C_CSR_READ                   4
#define I2C_CSR_WRITE                  0

#define I2C_CSR_MASTER                 8
#define I2C_CSR_SLAVE                  0

#define I2C_CSR_RESTART               16

#define I2C_CSR_IRQ_ENABLE           128
#define I2C_CSR_IRQ_DISABLE            0


#define I2C_MASTER_IDLE_FLAG         128
#define I2C_MASTER_NO_ACK_FLAG        64
#define I2C_MASTER_DATA_READY         32
#define I2C_MASTER_DATA_REQ           16

#define I2C_SLAVE_NO_ACK_FLAG          8
#define I2C_SLAVE_DATA_READY           4
#define I2C_SLAVE_DATA_REQ             2
#define I2C_SLAVE_ADDR_MATCH           1


#define I2C_TEN_BIT_HEADER            0x1e


typedef  void (*I2C_SLAVE_FUNC)(uint8_t addr, void (*call_back_func)(void)) __reentrant;

typedef struct {
   uint8_t (*masterWrite)(uint16_t addr, uint8_t buf[], uint8_t buf_size) __reentrant;
   uint8_t (*masterRead)(uint16_t i2c_addr, uint8_t reg_addr, uint8_t buf[], uint8_t buf_size) __reentrant;
   I2C_SLAVE_FUNC slave;
} I2C_STRUCT;


extern const I2C_STRUCT I2C;

#endif
