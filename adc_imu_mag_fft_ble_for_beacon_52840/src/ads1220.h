#ifndef _ADS1220_H
#define _ADS1220_H

#include "stdio.h"
#include "nrfx_gpiote.h"
#include "nrfx_spi.h"
#include <stdio.h>
#include <stdint.h>
#include <nrfx_gpiote.h>
#include <zephyr/kernel.h>

//ADS1220的几个管脚与spi的历程
#define SPI_INSTANCE 2
// #define SPI_CS_PIN      NRF_GPIO_PIN_MAP(0, 30)
// #define MOSI_PIN        NRF_GPIO_PIN_MAP(0, 28)    
// #define MISO_PIN		NRF_GPIO_PIN_MAP(0, 27)	
// #define SPI_SCK_PIN     NRF_GPIO_PIN_MAP(0, 29)
//nrf832+imu
#define SPI_CS_PIN      NRF_GPIO_PIN_MAP(0, 27)
#define MOSI_PIN        NRF_GPIO_PIN_MAP(0, 29)    
#define MISO_PIN		NRF_GPIO_PIN_MAP(0, 30)	
#define SPI_SCK_PIN     NRF_GPIO_PIN_MAP(0, 28)

//spi的使用配置头
static const nrfx_spi_t spi = NRFX_SPI_INSTANCE(SPI_INSTANCE);


//ADS1220 SPI commands
#define SPI_MASTER_DUMMY    0xFF
#define RESET               0x06   //Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define START               0x08    //Send the START/SYNC command (08h) to start converting in continuous conversion mode
#define WREG  0x40
#define RREG  0x20

//Config registers
#define CONFIG_REG0_ADDRESS 0x00
#define CONFIG_REG1_ADDRESS 0x01
#define CONFIG_REG2_ADDRESS 0x02
#define CONFIG_REG3_ADDRESS 0x03

#define REG_CONFIG3_IDAC1routing_MASK    0xE0
#define REG_CONFIG3_IDAC2routing_MASK    0x1C
#define REG_CONFIG2_VREF_MASK            0xC0
#define REG_CONFIG2_FIR_MASK             0x30
#define REG_CONFIG2_IDACcurrent_MASK     0x07
#define REG_CONFIG1_MODE_MASK            0x18
#define REG_CONFIG1_DR_MASK       0xE0
#define REG_CONFIG0_PGA_GAIN_MASK 0x0E
#define REG_CONFIG0_MUX_MASK      0xF0

#define IDAC1_disable     0x00  
#define IDAC1_AIN0        0x20  
#define IDAC1_AIN1        0x40  
#define IDAC1_AIN2        0x60  
#define IDAC1_AIN3        0x80  
#define IDAC1_REFP0       0xA0  
#define IDAC1_REFN0       0xC0  
#define IDAC1_reserved    0xE0  

#define IDAC2_disable     0x00  
#define IDAC2_AIN0        0x04  
#define IDAC2_AIN1        0x08  
#define IDAC2_AIN2        0x0C  
#define IDAC2_AIN3        0x10  
#define IDAC2_REFP0       0x14  
#define IDAC2_REFN0       0x18  
#define IDAC2_reserved    0x1C  

#define IDAC_OFF     0x00  
#define IDAC_10      0x01  
#define IDAC_50      0x02  
#define IDAC_100     0x03  
#define IDAC_250     0x04  
#define IDAC_500     0x05  
#define IDAC_1000    0x06  
#define IDAC_1500    0x07  

#define FIR_OFF      0x00  
#define FIR_5060     0x10  
#define FIR_50Hz     0x20  
#define FIR_60Hz     0x30  

#define VREF_2048       0x00  
#define VREF_REFP0      0x40  
#define VREF_AIN0       0x80  
#define VREF_ANALOG     0xC0  

#define MODE_NORMAL     0x00  
#define MODE_DUTYCYCLE  0x08  
#define MODE_TURBO      0x10  
#define MODE_RESERVED   0x18  
//这个根据文档进行操作
#define DR_20SPS    0x00
#define DR_45SPS    0x20
#define DR_90SPS    0x40
#define DR_175SPS   0x60
#define DR_330SPS   0x80
#define DR_600SPS   0xA0
#define DR_1000SPS  0xC0

#define PGA_GAIN_1   0x00
#define PGA_GAIN_2   0x02
#define PGA_GAIN_4   0x04
#define PGA_GAIN_8   0x06
#define PGA_GAIN_16  0x08
#define PGA_GAIN_32  0x0A
#define PGA_GAIN_64  0x0C
#define PGA_GAIN_128 0x0E

#define MUX_AIN0_AIN1   0x00
#define MUX_AIN0_AIN2   0x10
#define MUX_AIN0_AIN3   0x20
#define MUX_AIN1_AIN2   0x30
#define MUX_AIN1_AIN3   0x40
#define MUX_AIN2_AIN3   0x50
#define MUX_AIN1_AIN0   0x60
#define MUX_AIN3_AIN2   0x70
#define MUX_AIN0_AVSS   0x80
#define MUX_AIN1_AVSS   0x90
#define MUX_AIN2_AVSS   0xA0
#define MUX_AIN3_AVSS   0xB0

#define MUX_SE_CH0      0x80
#define MUX_SE_CH1      0x90
#define MUX_SE_CH2      0xA0
#define MUX_SE_CH3      0xB0

//对应的spi初始化
void ads_1015_spi_init(void);
void ads_begin(void);
void Start_Conv(void);
int  Read_Data(void);

#endif