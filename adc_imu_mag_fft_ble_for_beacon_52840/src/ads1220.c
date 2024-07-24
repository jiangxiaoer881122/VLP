#include "ads1220.h"
//#define BOARD_SENSYTHING ST_1_3
//创建了一个 SPISettings 对象，配置了 SPI 通信的时钟频率为 2 MHz，
//数据传输顺序为最高有效位优先，SPI 模式为模式 1。
//这个设置可以在 SPI 传输开始时使用，以确保主设备和从设备之间的通信参数一致


#ifdef _BV
#undef _BV
#endif

//定义
#define _BV(bit) (1<<(bit))

//spi的初始化
void ads_1015_spi_init(void)
{
        //配置SPI
        nrfx_spi_config_t spi_config = NRFX_SPI_DEFAULT_CONFIG(SPI_SCK_PIN,MOSI_PIN,MISO_PIN,SPI_CS_PIN);
        //根据需求开始更改自己的设置 时钟与数据的高低为都已弄好
        // spi_config.ss_pin = SPI_CS_PIN;
        // spi_config.miso_pin = MISO_PIN;
        // spi_config.mosi_pin = MOSI_PIN;
        // spi_config.sck_pin = SPI_SCK_PIN;
        spi_config.frequency=NRF_SPI_FREQ_8M;
        spi_config.mode = NRF_SPI_MODE_1;
        
        //开始初始化 开始阻塞读写
         nrfx_spi_init(&spi,&spi_config,NULL,NULL);
        // printk("520today%d",a);
}

//用于写一个八位的寄存器
void writeRegister2(uint8_t address, uint8_t value)
{
    //发送与输出的TX与RX的缓冲
    uint8_t spi_tx_buf[2];
    uint8_t spi_rx_buf[2];

    spi_tx_buf[0]=WREG|(address<<2);
    spi_tx_buf[1]=value;
    nrfx_spi_xfer_desc_t  s_w =NRFX_SPI_XFER_TRX(spi_tx_buf,sizeof(spi_tx_buf),NULL,0);
    // nrfx_spi_xfer_desc_t  s_w =NRFX_SPI_XFER_TRX(spi_tx_buf,sizeof(spi_tx_buf),spi_rx_buf,sizeof(spi_rx_buf));
    nrfx_spi_xfer(&spi,&s_w, 0);
}
//用于读一个八位的寄存器
uint8_t readRegister2(uint8_t address)
{
    uint8_t data;
    //发送与输出的TX与RX的缓冲
    uint8_t spi_tx_buf[2];
    uint8_t spi_rx_buf[2];
    spi_tx_buf[0]=RREG|(address<<2);
    spi_tx_buf[1]=0xFF;
    nrfx_spi_xfer_desc_t  s_w =NRFX_SPI_XFER_TRX(spi_tx_buf,sizeof(spi_tx_buf),spi_rx_buf,sizeof(spi_rx_buf));
    nrfx_spi_xfer(&spi,&s_w, 0);
    data =spi_rx_buf[0];
    return data;
}

// ads配置初始化
void ads_begin(void)
{
    //进行重启
    ads1220_Reset();
    printk("aaa");
    //加延时
    k_msleep(50);

    //开始写入配置
    uint8_t m_config_reg0 = 0x80;   //settings: AINP=A0, AINN=GND, Gain 1, PGA enabled
    uint8_t m_config_reg1 = 0xD4;   //settings: DR=2000 SPS, Mode=Trbuo, Conv mode=con shot, Temp Sensor disabled, Current Source off
    uint8_t m_config_reg2 = 0x10;   //settings: Vref internal, 50/60Hz rejection, power open, IDAC off
    uint8_t m_config_reg3 = 0x00;   //settings: IDAC1 disabled, IDAC2 disabled, DRDY pin only

    writeRegister2( CONFIG_REG0_ADDRESS , m_config_reg0);
    writeRegister2( CONFIG_REG1_ADDRESS , m_config_reg1);
    writeRegister2( CONFIG_REG2_ADDRESS , m_config_reg2);
    writeRegister2( CONFIG_REG3_ADDRESS , m_config_reg3);
}
//直接发送一个短的字符
void SPI_Command(uint8_t data_in)
{
    //发送与输出的TX与RX的缓冲
    uint8_t spi_tx_buf[1];
    spi_tx_buf[0]=data_in;
    nrfx_spi_xfer_desc_t  s_w =NRFX_SPI_XFER_TRX(spi_tx_buf,sizeof(spi_tx_buf),NULL,0);
    nrfx_spi_xfer(&spi,&s_w, 0);

}
void ads1220_Reset(void)
{
    SPI_Command(RESET);
}
void Start_Conv(void)
{
    SPI_Command(START);
}

int  Read_Data(void){
    int  result = 0;
    uint8_t spi_tx_buf[1]={0x10};
    uint8_t spi_rx_buf[4]={0};
    nrfx_spi_xfer_desc_t  s_w =NRFX_SPI_XFER_TRX(spi_tx_buf,sizeof(spi_tx_buf),spi_rx_buf,sizeof(spi_rx_buf));
    nrfx_spi_xfer(&spi,&s_w, 0);
    result = spi_rx_buf[1];
    result = (result << 8) | spi_rx_buf[2];
    result = (result << 8) | spi_rx_buf[3];
    // if (spi_rx_buf[0] & (1<<7)) {
    //     result |= 0xFF000000;
    // }

    return result;
}

