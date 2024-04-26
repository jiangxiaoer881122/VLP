#include "ble.h"
/*
 * Datalength is an integer, so BT_MFG_DATA_LEN can not be larger than 255.
 * To ensure that we need to chain PDUs we therefore add manufacturer data
 * twice when chaining is enabled
 */
extern u_int16_t big_time ;
extern int fft_out[6];
extern int fft_index[6];
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
struct bt_le_adv_param adv_param = {
			.id = BT_ID_DEFAULT,
			.sid = 0U, /* Supply unique SID when creating advertising set */
			.secondary_max_skip = 0U,
			.options = (BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_NAME),
			.interval_min = 0x0020,
			.interval_max = 0x0020,
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

	// //时间戳的高低位设置
	// mfg_data[0]=(big_time>>8)&0xFF;
	// mfg_data[1]=big_time&0xFF;
	// //移动标志与设备类型一个字节
	// // mfg_data[2]=0;
	// //电压一个字节
	// mfg_data[3]=0xFF;
	// //VLP包
	// mfg_data[4]=(fft_index[0]>>2)&0xFF;
	// mfg_data[5]=((fft_index[0]&0x03)<<6)|((fft_out[0]>>8)&0x3F);	
	// mfg_data[6]=fft_out[0]&0xFF;

	// mfg_data[7]=(fft_index[1]>>2)&0xFF;
	// mfg_data[8]=((fft_index[1]&0x03)<<6)|((fft_out[1]>>8)&0x3F);	
	// mfg_data[9]=fft_out[1]&0xFF;

	// mfg_data[10]=(fft_index[2]>>2)&0xFF;
	// mfg_data[11]=((fft_index[2]&0x03)<<6)|((fft_out[2]>>8)&0x3F);	
	// mfg_data[12]=fft_out[2]&0xFF;

	// mfg_data[13]=(fft_index[3]>>2)&0xFF;
	// mfg_data[14]=((fft_index[3]&0x03)<<6)|((fft_out[3]>>8)&0x3F);	
	// mfg_data[15]=fft_out[3]&0xFF;

	// mfg_data[16]=(fft_index[4]>>2)&0xFF;
	// mfg_data[17]=((fft_index[4]&0x03)<<6)|((fft_out[4]>>8)&0x3F);	
	// mfg_data[18]=fft_out[4]&0xFF;

	// mfg_data[19]=(fft_index[5]>>2)&0xFF;
	// mfg_data[20]=((fft_index[5]&0x03)<<6)|((fft_out[5]>>8)&0x3F);	
	// mfg_data[21]=fft_out[5]&0xFF;

	// int a=2;
	// //时间戳的高低位设置
	// mfg_data[0+a]=(big_time>>8)&0xFF;
	// mfg_data[1+a]=big_time&0xFF;
	// //移动标志与设备类型一个字节
	// mfg_data[2+a]=0x01;
	// //电压一个字节
	// mfg_data[3+a]=0xFF;
	// //VLP包
	// mfg_data[4+a]=(fft_index[0]>>2)&0xFF;
	// mfg_data[5+a]=((fft_index[0]&0x03)<<6)|((fft_out[0]>>8)&0x3F);	
	// mfg_data[6+a]=fft_out[0]&0xFF;

	// mfg_data[7+a]=(fft_index[1]>>2)&0xFF;
	// mfg_data[8+a]=((fft_index[1]&0x03)<<6)|((fft_out[1]>>8)&0x3F);	
	// mfg_data[9+a]=fft_out[1]&0xFF;

	// mfg_data[10+a]=(fft_index[2]>>2)&0xFF;
	// mfg_data[11+a]=((fft_index[2]&0x03)<<6)|((fft_out[2]>>8)&0x3F);	
	// mfg_data[12+a]=fft_out[2]&0xFF;

	// mfg_data[13+a]=(fft_index[3]>>2)&0xFF;
	// mfg_data[14+a]=((fft_index[3]&0x03)<<6)|((fft_out[3]>>8)&0x3F);	
	// mfg_data[15+a]=fft_out[3]&0xFF;

	// mfg_data[16+a]=(fft_index[4]>>2)&0xFF;
	// mfg_data[17+a]=((fft_index[4]&0x03)<<6)|((fft_out[4]>>8)&0x3F);	
	// mfg_data[18+a]=fft_out[4]&0xFF;

	// mfg_data[19+a]=(fft_index[5]>>2)&0xFF;
	// mfg_data[20+a]=((fft_index[5]&0x03)<<6)|((fft_out[5]>>8)&0x3F);	
	// mfg_data[21+a]=fft_out[5]&0xFF;
		int a=2;
	//时间戳的高低位设置
	mfg_data[0+a]=(big_time>>8)&0xFF;
	mfg_data[1+a]=big_time&0xFF;
	//移动标志与设备类型一个字节
	mfg_data[2+a]=0x01;
	//电压一个字节
	mfg_data[3+a]=0xFF;
	//开始进行常数设置 213,437,589,777,943,985
	fft_index[0]=213;
	fft_index[1]=437;
	fft_index[2]=589;
	fft_index[3]=777;
	fft_index[4]=943;
	fft_index[4]=985;
	fft_out[0]=213;
	fft_out[1]=437;
	fft_out[2]=589;
	fft_out[3]=777;
	fft_out[4]=943;
	fft_out[4]=985;
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
