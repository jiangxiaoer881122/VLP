#include "ads1015.h"

// //这里就是直接IIC的读取一个字节
// static uint8_t i2cread(void) {
// }
// //这里就是直接IIC的写一个字节
// static void i2cwrite(uint8_t x) {
// }

// //这里就是IIC对特定的地址与特定的寄存器进行写16位的数据
// static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
// }

// //这里就是IIC对特定的地址与特定的寄存器进行读16位的数据
// static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
// }

/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
void Ads1015init(uint8_t channel) {
  if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_3300SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= ADS1015_REG_CONFIG_PGA_4_096V;

  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC（这个是16位的）
  writeRegister(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONFIG, config);

}

uint16_t Ads1015read(void){
  // Read the conversion results 
   // 读 config register to the ADC（这个是16位的）
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(ADS1015_ADDRESS, ADS1015_REG_POINTER_CONVERT) >> 4;  
}





