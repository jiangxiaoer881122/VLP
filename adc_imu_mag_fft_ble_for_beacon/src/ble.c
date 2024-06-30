#include "ble.h"
#include "timer.h"
/*
 * Datalength is an integer, so BT_MFG_DATA_LEN can not be larger than 255.
 * To ensure that we need to chain PDUs we therefore add manufacturer data
 * twice when chaining is enabled
 */
extern u_int16_t big_time ;
extern int fft_out[6];
extern int fft_index[6];
extern SensorData imu_10_2[10];

static uint8_t mfg_data[BT_MFG_DATA_LEN-18]  = { 0xFF,0xFF,0x01};
// static uint8_t mfg_data1[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
// static uint8_t mfg_data2[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x03};
// static uint8_t mfg_data3[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x04};
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, sizeof(mfg_data)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data1, sizeof(mfg_data1)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data2, sizeof(mfg_data2)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data3, sizeof(mfg_data3)),
};
//广播参数实现
static struct bt_le_ext_adv *adv[CONFIG_BT_EXT_ADV_MAX_ADV_SET];
//广播的mac地址
static const bt_addr_le_t fixed_addr = {
    .type = BT_ADDR_LE_PUBLIC,  // 地址类型，使用公共地址
    .a = {
        .val = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB }  // 固定的地址
    }
};
struct bt_le_adv_param adv_param = {
			.id = BT_ID_DEFAULT,
			.sid = 0U, /* Supply unique SID when creating advertising set */
			.secondary_max_skip = 0U,
			.options = (BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_NAME|BT_LE_ADV_OPT_USE_IDENTITY),
			.interval_min = 0x0100,
			.interval_max = 0x0100,
			.peer = NULL,
};//间隔为50ms
int err;
int broadcaster_multiple(void)
{

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return err;
	}
	//设置蓝牙的mac地址

	//设置广播数据
	mfg_data[235]=0xAF;
	for (int index = 0; index < CONFIG_BT_EXT_ADV_MAX_ADV_SET; index++) {
		/* Use advertising set instance index as SID */
		adv_param.sid = index;
		/* Create a non-connectable non-scannable advertising set */
		err = bt_le_ext_adv_create(&adv_param, NULL, &adv[index]);
		if (err) {
			printk("Failed to create advertising set %d (err %d)\n",
			       index, err);
			return err;
		}
		/* Set extended advertising data */
		err = bt_le_ext_adv_set_data(adv[index], ad, ARRAY_SIZE(ad),
					     NULL, 0);
		if (err) {
			printk("Failed to set advertising data for set %d "
			       "(err %d)\n", index, err);
			return err;
		}
		/* Start extended advertising set */   
		err = bt_le_ext_adv_start(adv[index],BT_LE_EXT_ADV_START_DEFAULT);
		if (err) {
			printk("Failed to start extended advertising set %d "
			       "(err %d)\n", index, err);
			return err;
		}
		printk("Started Extended Advertising Set %d.\n", index);
	}
	return 0;
}
//进行数据的改变
void ble_data_change()
{
	int a=2;	
	int i=0;
	//时间戳的高低位设置
	mfg_data[0+a]=(big_time>>8)&0xFF;
	mfg_data[1+a]=big_time&0xFF;
	//移动标志与设备类型一个字节
	mfg_data[2+a]=0x01;
	//电压一个字节
	mfg_data[3+a]=0xFF;
	//VLP包
	mfg_data[4+a]=(fft_index[0]>>2)&0xFF;
	mfg_data[5+a]=((fft_index[0]&0x03)<<6)|((fft_out[0]>>8)&0x3F);	
	mfg_data[6+a]=fft_out[0]&0xFF;

	mfg_data[7+a]=(fft_index[1]>>2)&0xFF;
	mfg_data[8+a]=((fft_index[1]&0x03)<<6)|((fft_out[1]>>8)&0x3F);	
	mfg_data[9+a]=fft_out[1]&0xFF;

	mfg_data[10+a]=(fft_index[2]>>2)&0xFF;
	mfg_data[11+a]=((fft_index[2]&0x03)<<6)|((fft_out[2]>>8)&0x3F);	
	mfg_data[12+a]=fft_out[2]&0xFF;

	mfg_data[13+a]=(fft_index[3]>>2)&0xFF;
	mfg_data[14+a]=((fft_index[3]&0x03)<<6)|((fft_out[3]>>8)&0x3F);	
	mfg_data[15+a]=fft_out[3]&0xFF;

	mfg_data[16+a]=(fft_index[4]>>2)&0xFF;
	mfg_data[17+a]=((fft_index[4]&0x03)<<6)|((fft_out[4]>>8)&0x3F);	
	mfg_data[18+a]=fft_out[4]&0xFF;

	mfg_data[19+a]=(fft_index[5]>>2)&0xFF;
	mfg_data[20+a]=((fft_index[5]&0x03)<<6)|((fft_out[5]>>8)&0x3F);	
	mfg_data[21+a]=fft_out[5]&0xFF;
	
	//vlp包imu 		
	//先进行imu时间戳的对齐10bit 四个为一组
	mfg_data[22+a]=(imu_10_2[0].time>>2)&0xFF;
	//2+6
	mfg_data[23+a]=((imu_10_2[0].time&0x03)<<6)|((imu_10_2[1].time>>4)&0x3F);
	//4+4	
	mfg_data[24+a]=((imu_10_2[1].time&0x0F)<<4)|((imu_10_2[2].time>>6)&0x0F);
	//6+2
	mfg_data[25+a]=((imu_10_2[2].time&0x3F)<<2)|((imu_10_2[3].time>>8)&0x03);
	mfg_data[26+a]=imu_10_2[3].time&0xFF;

	// //第二组时间戳
	mfg_data[27+a]=(imu_10_2[4].time>>2)&0xFF;
	//2+6
	mfg_data[28+a]=((imu_10_2[4].time&0x03)<<6)|((imu_10_2[5].time>>4)&0x3F);
	//4+4	
	mfg_data[29+a]=((imu_10_2[5].time&0x0F)<<4)|((imu_10_2[6].time>>6)&0x0F);
	//6+2
	mfg_data[30+a]=((imu_10_2[6].time&0x3F)<<2)|((imu_10_2[7].time>>8)&0x03);
	mfg_data[31+a]=imu_10_2[7].time&0xFF;

	//第三组时间戳
	mfg_data[32+a]=(imu_10_2[8].time>>2)&0xFF;
	//2+6
	mfg_data[33+a]=((imu_10_2[8].time&0x03)<<6)|((imu_10_2[9].time>>4)&0x3F);
	//4+4	
	mfg_data[34+a]=((imu_10_2[9].time&0x0F)<<4)|0x00;
	a=a+1;
	// //在进行imu的数据传递 xyz
	for(i=0;i<10;i++)
	{
		//acc x
		mfg_data[34+a]=imu_10_2[i].icm[0];
		mfg_data[35+a]=imu_10_2[i].icm[1];
		//acc y
		mfg_data[36+a]=imu_10_2[i].icm[2];
		mfg_data[37+a]=imu_10_2[i].icm[3];
		//acc z
		mfg_data[38+a]=imu_10_2[i].icm[4];
		mfg_data[39+a]=imu_10_2[i].icm[5];

		a=a+6;

		//gyro x
		mfg_data[34+a]=imu_10_2[i].icm[6];
		mfg_data[35+a]=imu_10_2[i].icm[7];
		//gyro y
		mfg_data[36+a]=imu_10_2[i].icm[8];
		mfg_data[37+a]=imu_10_2[i].icm[9];
		//gyro z
		mfg_data[38+a]=imu_10_2[i].icm[10];
		mfg_data[39+a]=imu_10_2[i].icm[11];

		a=a+6;
		//mag x
		mfg_data[34+a]=imu_10_2[i].lis[1];
		mfg_data[35+a]=imu_10_2[i].lis[0];
		//mag y
		mfg_data[36+a]=imu_10_2[i].lis[3];
		mfg_data[37+a]=imu_10_2[i].lis[2];
		//mag z
		mfg_data[38+a]=imu_10_2[i].lis[5];
		mfg_data[39+a]=imu_10_2[i].lis[4];
       
	   a=a+6;
	}


// 在进行imu的数据传递 xyz
// a=a+1;
// 	for(i=0;i<10;i++)
// 	{
// 		//acc x
// 		mfg_data[34+a]=0x11;
// 		mfg_data[35+a]=0x22;
// 		//acc y
// 		mfg_data[36+a]=0x33;
// 		mfg_data[37+a]=0x44;
// 		//acc z
// 		mfg_data[38+a]=0x55;
// 		mfg_data[39+a]=0x66;

// 		a=a+6;

// 		//gyro x
// 		mfg_data[34+a]=0x77;
// 		mfg_data[35+a]=0x88;
// 		//gyro y
// 		mfg_data[36+a]=0x99;
// 		mfg_data[37+a]=0xAA;
// 		//gyro z
// 		mfg_data[38+a]=0xBB;
// 		mfg_data[39+a]=0xCC;

// 		a=a+6;
// 		//mag x
// 		mfg_data[34+a]=0xDD;
// 		mfg_data[35+a]=0x11;
// 		//mag y
// 		mfg_data[36+a]=0x22;
// 		mfg_data[37+a]=0x33;
// 		//mag z
// 		mfg_data[38+a]=0x44;
// 		mfg_data[39+a]=0x55;
       
// 	   a=a+6;
// 	}


}
//进行更新数据
void ble_data_update()
{
	//先暂停广播
	bt_le_ext_adv_stop(adv[0]);
	//开始更新广播
	ble_data_change();
	//重新填写广播
	bt_le_ext_adv_set_data(adv[0], ad, ARRAY_SIZE(ad),NULL, 0);
	//重新开启广播
	bt_le_ext_adv_start(adv[0],BT_LE_EXT_ADV_START_DEFAULT);
}
