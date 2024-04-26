/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/hci.h>

/* Maximum supported AD data length, use a value supported by the Controller,
 * Bluetooth Core Specification define minimum of 31 bytes will be supported by
 * all Controllers, can be a maximum of 1650 bytes when supported.
 */
#if defined(CONFIG_BT_CTLR_ADV_DATA_LEN_MAX)
#define BT_AD_DATA_LEN_MAX CONFIG_BT_CTLR_ADV_DATA_LEN_MAX
#else
#define BT_AD_DATA_LEN_MAX 31U
#endif

/* Size of AD data format length field in octets */
#define BT_AD_DATA_FORMAT_LEN_SIZE 1U

/* Size of AD data format type field in octets */
#define BT_AD_DATA_FORMAT_TYPE_SIZE 1U

/* Maximum value of AD data format length field (8-bit) */
#define BT_AD_DATA_FORMAT_LEN_MAX 255U

/* Device name length, size minus one null character */
#define BT_DEVICE_NAME_LEN (sizeof(CONFIG_BT_DEVICE_NAME) - 1U)

/* Device name length in AD data format, 2 bytes for length and type overhead */
#define BT_DEVICE_NAME_AD_DATA_LEN (BT_AD_DATA_FORMAT_LEN_SIZE + \
				    BT_AD_DATA_FORMAT_TYPE_SIZE + \
				    BT_DEVICE_NAME_LEN)

/* Maximum manufacturer data length, considering ad data format overhead and
 * the included device name in ad data format.
 */
#define BT_MFG_DATA_LEN_MAX       (MIN((BT_AD_DATA_FORMAT_LEN_MAX - \
					BT_AD_DATA_FORMAT_TYPE_SIZE), \
				       (BT_AD_DATA_LEN_MAX - \
					BT_AD_DATA_FORMAT_LEN_SIZE - \
					BT_AD_DATA_FORMAT_TYPE_SIZE)))
#define BT_MFG_DATA_LEN           (MIN(BT_MFG_DATA_LEN_MAX, \
				       (BT_AD_DATA_LEN_MAX - \
					BT_AD_DATA_FORMAT_LEN_SIZE - \
					BT_AD_DATA_FORMAT_TYPE_SIZE - \
					BT_DEVICE_NAME_AD_DATA_LEN)))
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
bt_addr_t addr;
char addr_str[18];
int broadcaster_multiple(void)
{
	struct bt_le_adv_param adv_param = {
		.id = BT_ID_DEFAULT,
		.sid = 0U, /* Supply unique SID when creating advertising set */
		.secondary_max_skip = 0U,
		.options = (BT_LE_ADV_OPT_EXT_ADV | BT_LE_ADV_OPT_USE_NAME),
		.interval_min = 0x0050,
		.interval_max = 0x0050,
		.peer = NULL,
	};//间隔为50ms
	int err;

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return err;
	}
	//设置广播格式
	mfg_data[235]=0xEF;
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
		err = bt_le_ext_adv_start(adv[index],
					  BT_LE_EXT_ADV_START_DEFAULT);
		if (err) {
			printk("Failed to start extended advertising set %d "
			       "(err %d)\n", index, err);
			return err;
		}

		printk("Started Extended Advertising Set %d.\n", index);
		bt_addr_to_str(&addr, addr_str, sizeof(addr_str));
		printk("AAA %s\n",addr_str);
	}

	return 0;
}
