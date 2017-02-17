/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 * Author: Andi Shyti <andi.shyti@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Samsung Gear VR device driver
 */

#include <linux/hidraw.h>
#include <linux/module.h>

#include "hid-ids.h"

static int gearvr_probe(struct hid_device *hdev, const struct hid_device_id *id)
{
	int err;

	err = hid_parse(hdev);
	if (err)
		return err;

	err = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	if (err)
		return err;

	err = hid_hw_power(hdev, PM_HINT_FULLON);
	if (err)
		goto fail_dev_stop;

	err = hid_hw_open(hdev);
	if (err)
		goto fail_power_off;

	return 0;

fail_power_off:
	hid_hw_power(hdev, PM_HINT_NORMAL);
fail_dev_stop:
	hid_hw_stop(hdev);

	return err;
}

static void gearvr_remove(struct hid_device *hdev)
{
	hid_hw_close(hdev);
	hid_hw_power(hdev, PM_HINT_NORMAL);
	hid_hw_stop(hdev);
}

static const struct hid_device_id gearvr_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_SAMSUNG_ELECTRONICS,
			USB_DEVICE_ID_SAMSUNG_GEARVR) },
	{ }
};

MODULE_DEVICE_TABLE(hid, gearvr_devices);

static struct hid_driver gearvr_driver = {
	.name = "gearvr",
	.id_table = gearvr_devices,
	.probe = gearvr_probe,
	.remove = gearvr_remove,
};

module_hid_driver(gearvr_driver);

MODULE_AUTHOR("Andi Shyti <andi.shyti@samsung.com>");
MODULE_DESCRIPTION("Samsung Gear VR device driver");
MODULE_LICENSE("GPL v2");
