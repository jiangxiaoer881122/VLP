#include "ble.h"
/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */


/*
 * Datalength is an integer, so BT_MFG_DATA_LEN can not be larger than 255.
 * To ensure that we need to chain PDUs we therefore add manufacturer data
 * twice when chaining is enabled
 */
static uint8_t mfg_data[BT_MFG_DATA_LEN-18]  = { 0xFF, 0xFF,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
// static uint8_t mfg_data1[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
// static uint8_t mfg_data2[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x03};
// static uint8_t mfg_data3[BT_MFG_DATA_LEN] = { 0xFF, 0xFF,0x04};
static const struct bt_data ad[] = {
	BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, sizeof(mfg_data)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data1, sizeof(mfg_data1)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data2, sizeof(mfg_data2)),
	// BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data3, sizeof(mfg_data3)),
};

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
	// mfg_data1[BT_MFG_DATA_LEN-1]=0xEF;
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
//进行更新数据
void ble_data_update()
{
	//先暂停广播
	bt_le_ext_adv_stop(adv[0]);
	//开始更新广播
	mfg_data[235]=0x66;
	bt_le_ext_adv_set_data(adv[0], ad, ARRAY_SIZE(ad),NULL, 0);
	//重新开启广播
	bt_le_ext_adv_start(adv[0],BT_LE_EXT_ADV_START_DEFAULT);
}
